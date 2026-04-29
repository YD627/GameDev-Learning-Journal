## 光栅化(Rasterization)
### 核心目的：确定几何图形（三角形）覆盖了屏幕上的哪些像素
1. 视锥：
    - 假设有个点(0,t,n), 其对于的tan(FOV/2) = t/|n|, aspect ratio = r/t

2. Rasterize: 把东西画到屏幕上的像素点上
3. Pixel: 像素点，屏幕上的最小单位(red,green,blue)
    - 坐标:
        - 左下角点为(0, 0)
        - 右上角点为(width - 1, height -1)
        - 每个像素点的中心位置为(x + 0.5, y + 0.5)
        - 屏幕的覆盖范围是(0，0) ~ (width, height)
4. Canonical Cube to Screen(规范坐标到屏幕坐标):
    - 变换xy平面: [-1, 1]² -> [0,width] * [0,height]
    ```
    视口变换矩阵，用于投影变换之后，光栅化之前的阶段
    将标准化设备坐标（NDC）转换到屏幕空间坐标。
    M_viewport = [width/2,  0，       0， width/2 ]
                 [0，       height/2, 0， height/2]
                 [0，       0，       1， 0       ]
                 [0，       0，       0， 1       ]
    ```
5. 最基础的方法：采样法
    - 三角形：
        - 定义一个函数inside(tri, x, y)来判断点(x, y)是否在三角形tri内
        ```C++
        for(int x = 0; x < xmax; x++)
            for(int y =0; y < ymax; y++)
                image[x][y] = inside(tri, x + 0.5, y + 0.5);
        ```
        - inside(tri, x, y)是通过3次叉积来实现的, 假设要判断的点是Q(x, y), 三角形的顶点是A, B, C
            - 计算(QA, AB), (QB, BC), (QC, CA)的叉积, 只有这三个叉积的方向都相同, 才说明点Q在三角形tri内
            - 否则, 点Q不在三角形tri内
        - 包围盒(Bounding Box): 三角形tri的最小矩形框, 用于快速判断点是否在三角形tri内, 只有这个矩形框内的点, 才需要判断点是否在三角形tri内
        ```C++
        for(int x = xmin; x <= xmax; x++)
            for(int y = ymin; y <= ymax; y++)
                image[x][y] = inside(tri, x + 0.5, y + 0.5);
        ```
    - 结果：每个像素都显示一个颜色时，得到的图案锯齿状比较严重
6. 抗锯齿(Antialiasing)
    - Aliasing锯齿(走样)：当对一个连续信号（如图形的平滑边缘）进行离散采样（如转换为屏幕像素）时，如果采样频率低于信号最高频率的两倍（奈奎斯特采样定理），就无法准确重建原始信号，导致信息丢失和错误的重建结果。
    - Aliasing Artifact: 信号变化的很快，但是采样频率低
7. Antialiasing Sampling(抗锯齿采样)
    - Pre-Filter: 先对图像进行模糊处理然后再采样
    - 使用1-pixel box-blur滤波器：每个像素的值等于其周围像素的平均值
        - 核心思想是：通过平均每个像素与其直接邻居的颜色，来柔化因像素离散采样而产生的生硬边缘，从而减少视觉上的锯齿感
    - SuperSampling(超采样)-MSAA：
        - 通过在每个像素中心进行多个采样，来减少锯齿状
        - 例如，将每个像素中心进行4个样：
        - 将每个像素划分成4个子像素，然后判断每个子像素是否在三角形tri内，最后按在的个数来计算每个像素的颜色值
        - 这样可以减少锯齿状，但是会增加渲染时间
        
8. 傅里叶变换(Fourier Transform) - 还未学习
9. 反傅里叶变换(Inverse Fourier Transform) - 还未学习
10. 卷积(Convolution) - 了解了一点
11. 低通录波器(Low-Pass Filter) = 盒子滤波器(Box Filter)
    - 盒子滤波器：
    ```
    3 * 3 盒子滤波器：每个像素的值等于其周围像素的平均值
    1/9 * [1, 1, 1]
          [1, 1, 1]
          [1, 1, 1]
    盒子越大的话，模糊度越高
    ```
12. FXAA(Fast Approximate AA) - 初步了解
    - 核心思想：一种完全基于屏幕空间图像的、与场景复杂度无关的抗锯齿技术。它不处理几何边缘，而是识别最终渲染图像中的颜色对比度高的区域（即“边缘”），并对这些区域进行智能的模糊处理。
13. TAA(Temporal AA) - 初步了解
    - 核心思想：利用时间连续性（即相邻帧之间的信息）来累积超采样本。它假设当前帧的画面与上一帧的画面在内容上是高度相关的，通过将历史帧的像素信息与当前帧混合，来达到用多帧数据“模拟”超高采样率的效果。

### 可视/阻挡
Z-Buffering
- 画家算法：先绘制远处的物体，再绘制近处的物体(覆盖原先远处的物体)
- Z_Buffer算法
    ```
    Initialize depth buffer to ∞
    During rasterization:
        for (each triangle T)
            for (each sample(x, y, z) in T)
                if (z < zbuffer[x][y])
                    framebuffer[x][y] = color   // 更新像素点的颜色值
                    zbuffer[x][y] = z           // 更新像素点的深度值
                else
                    continue                    // 什么都不做，因为被遮挡了
    // 时间复杂度：O(n) -- 只是不断求每个像素点的最小值
    ```