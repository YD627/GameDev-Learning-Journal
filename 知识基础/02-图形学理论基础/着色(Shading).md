## 着色(Shading)
1. shading points
    - input
        - Viewer Direction: v
        - Surface Normal: n
        - Light Direction: L
        - Surface Parameters(color, shininess, ...)

2. Diffuse Shading(漫反射着色)
    - 漫反射着色：表面颜色(surface color)从任意的视角看，都是相同的。
    - 光强度: I = k_d * dot(l, n)
        - l和n都是单位向量, l是入射光方向, n是法线方向
        - k_d是漫反射系数, 0 <= k_d <= 1, 表示表面吸收光的强弱, 0表示完全吸收, 1表示完全反射
    - Light Falloff(光衰减)
        - 光衰减：光从光源到表面的强度变化, 通常用指数衰减模型表示
        - 光衰减模型：I = I0 / r²
    - Lambertian (Diffuse) Shading(兰伯特漫反射着色)
        - L_d = k_d(I / r²) max(0, dot(l, n))
        - 漫反射强度: L_d
        - 漫反射系数: k_d
        - 着色点的能量接收: max(0, dot(l, n))
            - 表示光从光源到表面的夹角, 0表示垂直, 1表示水平
            - 光从光源到表面的夹角越小, 表示光的强度越强
        - 和观测方向无关

3. Specular Shading(镜面反射着色)
    - Blinn-Phong
    - V(视角方向)靠经mirror direction -> half vector(半程向量)靠经法向量n
    - half vector: h = normalize(v + l)
    - L_s = k_s(I / r²) max(0, cosα)^p
          = k_s(I / r²) max(0, dot(h, n))^p

4. Ambient Shading(环境光着色)
    - L_a = k_a * I_a

5. Blinn-Phong Reflection Mode
    - L = L_a + L_d + L_s
        = k_a * I_a + k_d(I / r²) max(0, dot(l, n)) + k_s(I / r²) max(0, dot(h, n))^p

## Shading Frequency(着色频率)
1. Flat Shading
    - 三角面是平面，只有一个法线方向
    - 对于smooth surface(光滑表面)不好

2. Gouraud Shading
    - 每个顶点都有一个法线方向
    - 着色效果一般

3. Phong Shading
    - 插值出每个像素的法线方向
    - 着色效果最好

4. 如何计算三角形的每个顶点的法线的方法
    - 基本方法，对该顶点相关的三角形的法线求平均值
    - N_v = ΣN_i / |ΣN_i|
    - 进阶一点是算加权平均值

5. 如何计算每个像素点的法线
    - 基于三角形重心坐标进行插值(要归一化)

## Real-Time Rendering Pipeline(实时渲染管线)
1. Application Input: Vertices in 3D space
2. Vertex Processing: Vertices positioned in screen space
3. Triangle Processing: Triangles positioned in screen space
4. Rasterization: Fragments (one per covered sample)
5. Fragment Processing: Shaded Fragment
6. FrameBuffer Operations
7. Display: Output: image(pixels)

## 物体的表面是2D的


## 纹理映射计算 (Texture Mapping)
纹理映射的核心是将2D纹理图像“贴”到3D模型的表面上。此过程涉及从纹理图像（由纹素/Texel组成的网格）到模型表面（由顶点和三角形网格定义），再到最终屏幕像素的一系列坐标转换和采样计算。

---

#### 1. 在三角形内部插值：重心坐标 (Barycentric Coordinates)
重心坐标是定义在三角形平面上的一种坐标系统，用于平滑地插值三角形顶点上的任何属性（如颜色、法线、纹理坐标等）到三角形内部的任意一点。

*   **定义**：对于三角形平面上的任意一点 \((x, y)\)，都可以表示为三个顶点 \(A\), \(B\), \(C\) 的加权和：
    \[(x, y) = α A + β B + γ C\]
    其中 \((α, β, γ)\) 称为该点的重心坐标，且满足 α + β + γ = 1。
*   **几何意义**：
    *   当 α, β, γ 均 **非负** 时，点 \((x, y)\) 位于**三角形内部**。
    *   重心坐标的值可以看作该点到对边所做垂线所分割的面积比。
*   **计算与应用**：
    1.  **计算给定点的重心坐标**：通过比较子三角形与原始三角形的面积比来计算。
    2.  **属性插值**：一旦得到点 \(P\) 的重心坐标 \((α, β, γ)\)，就可以用它来插值顶点属性。例如，插值纹理坐标 \((u, v)\)：
        \[u_P = α u_A + β u_B + γ u_C\]
        \[v_P = α v_A + β v_B + γ v_C\]
        同样地，法线、颜色等属性也可以按此方式插值，这是实现 `Phong Shading`（逐像素着色）和纹理映射的基础。

---

#### 2. 纹理过滤 (Texture Filtering)
当纹理坐标 \((u_P, v_P)\) 不是整数时（即对应纹素之间的位置），或一个屏幕像素覆盖多个/部分纹素时，需要进行纹理过滤以获得平滑、高质量的图像。

##### A. 纹理放大 (Texture Magnification)
当一个纹素对应多个屏幕像素时，会发生纹理放大，导致图像出现块状像素化（马赛克）。
*   **最近邻过滤 (Nearest Neighbor)**：选择最近的纹素颜色。速度快，但会产生锯齿。
*   **双线性插值 (Bilinear Interpolation)**：
    *   **原理**：取纹理坐标点周围最近的4个纹素，在水平方向进行两次线性插值，再在垂直方向进行一次线性插值，共两次线性插值，得到一个平滑的过渡颜色。
    *   **使用场景**：解决纹理放大时的锯齿问题，是实时渲染中最基础的纹理过滤方式，常作为更高级过滤技术的基础步骤。

##### B. 纹理缩小 (Texture Minification) 与 Mipmap
当一个屏幕像素覆盖纹理上的一大片区域（多个纹素）时，会发生纹理缩小。简单采样会导致走样（摩尔纹、闪烁）。
*   **Mipmap 原理**：
    1.  **预处理**：为原始纹理生成一个图像金字塔。原始层为第0层，后续每一层图像的长宽均为前一层的1/2，直到1x1。存储开销约为原纹理的1/3。
    2.  **层级选择**：在渲染时，GPU计算当前像素在纹理空间中的覆盖范围，根据其面积估算出最合适的Mipmap层级（LOD, Level of Detail）。层级越高，图像越模糊，抗锯齿效果越好。
*   **三线性插值 (Trilinear Interpolation)**：
    *   **原理**：为了解决在相邻Mipmap层级之间切换时产生的视觉跳跃，在计算出的（非整数）LOD对应的上下两个Mipmap层级上，分别进行**双线性插值**，然后再对这两个插值结果进行一次**线性插值**。
    *   **使用场景**：使不同层级的Mipmap过渡平滑，是高质量实时渲染的标配。

##### C. 各向异性过滤 (Anisotropic Filtering)
Mipmap假设像素在纹理空间的覆盖区域是近似正方形的。但在透视投影下（如看向地面），这个区域通常是一个被拉长的矩形（各向异性），使用正方形过滤会过度模糊。
*   **原理**：沿纹理空间内像素覆盖区域的主要拉伸方向，在多个不同比例和层级的Mipmap中进行采样，最后将结果混合。这能在长边方向保持抗锯齿效果，同时在短边方向保留更多细节。
*   **使用场景**：显著改善具有透视表面的纹理清晰度（如游戏中的地面、墙面）。性能开销高于Mipmap，可通过显卡设置选择倍数（如2x, 4x, 16x AF）。

##### D. EWA 过滤 (Elliptical Weighted Average Filtering)
这是目前质量最高的纹理过滤算法之一，能最精确地处理像素在纹理空间中任意形状（椭圆）的覆盖区域。
*   **原理**：将像素反向投影到纹理空间，精确拟合其覆盖区域为一个椭圆。以此椭圆为滤波核，计算核内所有纹素的加权平均值。
*   **使用场景**：主要用于对画质有极致要求的**离线渲染**（如电影、静帧）。因其计算量巨大，一般不用于实时图形学。