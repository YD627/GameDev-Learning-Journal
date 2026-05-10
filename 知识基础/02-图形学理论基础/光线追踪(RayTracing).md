## 光线追踪(RayTracing)
### 光线追踪和光栅化的区别
- 光栅化是一种很快的渲染方法，但是质量比较低
    - 实时渲染
- 光线追踪是一种准确的方法，但是非常耗时
    - 离线渲染

### 基础光线追踪算法
三个关于光线的ideas：
- 光沿直线传播
- 光线不会与其它相交的光线发生碰撞
- 光线从光源出发最终到达人眼，人眼也可以模拟光线的传播，发射”感知射线“最后也能到达光源(光路的可逆性)

Pinhole Camera Model
1. 从眼睛(点)看向每个像素的方向，找到最近的交点
2. 从交点往光源(点)方向连线，判断该点是否在阴影中
3. 如果不在阴影中，对该点进行着色计算
4. 写回像素

#### Whitted-style Ray Tracing
理论基础：从眼睛看向每个像素的方向，找到最近的交点，并进行折射反射计算，继续找新的交点，直到找不到，同时将全部交点都连向光源，判断点是否在阴影中。最后把所有交点的着色结果相加，得到最终的像素颜色。
- 递归调用: 从每个交点出发，继续找新的交点，直到找不到，同时将全部交点都连向光源，判断点是否在阴影中
- Ray equation: r(t) = o + t * d
    - o: origin, 起点
    - t: time, 时间(0<= t < ∞)
    - d: direction, 单位方向向量

- Ray Intersection with Square
    - ray: r(t) = o + td
    - square: p：(p - c)² - R² = 0
    - (o + td - c)² - R² = 0
    - 解方程：t = (c - o) · d / d²

- Ray Intersection with Implicit Surface
    - ray: r(t) = o + td
    - p: f(p) = 0
    - f(o + td) = 0
    - 解得实数，正数的根值

- 射线还能判断一个点是否在几何物体内部(与光源做射线看和几何物体的交点个数)
    - 奇数交点: 点在几何物体内部
    - 偶数交点: 点在几何物体外部

- 光线和三角形的交点
    - 三角形是一个平面
        - 光线和平面是否相交
        - 测试交点是否在三角形内部
    - 平面的定义：通过平面上的任意一个点和法线向量
        - p：(p - p') · n = 0
        - p': 平面上的点
        - n: 法线向量
    - 交点计算: (o + td - p') · n = 0; t = (p' - o) · n / d · n(检查t >= 0)

#### Moller Trumbore Algorithm
核心公式: o + td = (1 - d_1 - d_2)P0 + d_1P1 + d_2P2
- 解线性方程组(克拉默公式): 只有1-d_1-d_2, d_1, d_2,t都大于0时，才会有交点

### 包围盒(Bounding Box)
定义: 用一个简单的形状来包围一个复杂的形状，例如一个三角形包围一个球
- 作用: 减少光线追踪的计算量(如果光线连包围盒都碰不到，就更不可能碰到复杂的形状)
- 通常使用: Axis-Aligned Bounding Box(AABB)轴对齐包围盒

#### 光线和AABB求交点
2D情况下:
- 求射线和x0,x1两个平面的交点: 得到t_min和t_max
- 再求射线和y0,y1两个平面的交点: 得到t_min和t_max
- 取交集: 最终的交点时间范围为[t_min, t_max]
3维空间中:
- 求射线和x0,x1,y0,y1,z0,z1六个平面的交点: 得到6个t_min和t_max
- 取交集: 最终的交点时间范围为[t_min, t_max], 其中t_min为所有t_min中的最大值，t_max为所有t_max中的最小值
- 当t_min < t_max时，光线与AABB相交
    - 如果t_max < 0, 说明盒子在光线背后，光线不会与盒子相交
    - 如果t_min < 0, t_max > 0, 说明光线的起点在盒子内部，光线会与盒子相交
- 反之，光线与AABB不相交
用AABB的方法一方面也是方便t的计算
- 例如和x-axis的交点: t = (x0 - o_x) / d_x(x0与光源的x坐标的差值除以x轴方向向量就是光线到达x0的时间)

#### 预处理-构建加速网格(build acceleration grid)
1. 寻找包围盒
2. 创建网格
3. 将每个物体存入相互重叠的网络格子单元中

#### 光线与场景求交(Ray-Scene intersection)
1. 按照遍历顺序，逐个穿过网格格子
2. 当格子内包含物体时，检测光线与该单元内的物体是否相交
3. 格子数量不能太稀疏，也不能太密集。
    - Heuristic: 格子数量 = 场景中物体数量 * 27

#### 空间划分(Spatial partitioning)
例子：
- Oct-Tree(八叉树)
- KD-Tree(K维树)
    - KD树（K-Dimensional Tree）是一种用于组织多维空间中点数据的数据结构，主要用于高效地进行范围搜索和最近邻查找
    - 核心思想: 递归地将空间划分为两个半空间。构建时，它会选择当前数据集中方差最大的维度（或按维度轮流选择），并以该维度上的中位数作为分割点，将数据划分为左右两个子集，并递归构建左右子树。
    - 数据存储在叶节点中
- BSP-Tree(二叉空间分区树)

- Traversing a KD-tree（遍历KD树）
    - 先和最大的盒子相交(根节点)
    - 判断左右子树是否相交
    - 如果叶子节点和光线相交，就要判断叶子节点中的物体是否与光线相交

#### 物体划分(Object partitioning) & Bouding Volume Hierarchy(BVH)
Bounding Volume Hierarchy(BVH)的基础思想:
- 递归地将三角形分成两堆
- 重新求这两堆三角形的包围盒(找最小和最大的x,y,z坐标即可确定一个包围盒)
- 一个物体只可能出现在一个包围盒中
- 包围盒有可能相交
- 中间节点只存包围盒和子节点的指针
- 叶子节点存物体
```
伪代码
Intersect(Ray ray, BVH node) {
    if(ray missed node.box) return;

    if(node is a leaf node) {
        test intersection with all objects;
        return closest intersection;
    }

    hit1 = Intersect(ray, node.child1);
    hit2 = Intersect(ray, node.child2);

    return the closer of hit1 and hit2;
}
```

Heuristic(启发式):
1. 总是选择节点中最长的轴作为分割轴
2. 在中间物体的位置来分割节点(让两个节点尽量平衡)
    - 算法: 找第n大的物体-快速划分算法

---
### 辐射度量学(Radiometry)
Accurately measure the spatial properties of light
- New terms: Radiant flux, intensity, irradiance, radiance
- Radiant Intensity：Light emitted from a source 
- Irradiance: Light falling on a surface 
- Radiance: Light traveling along a ray

#### Radiant Intensity
定义: 辐射（光通量/视觉）强度是指点光源在单位立体角内发出的功率。
- I(ω) = P / ω (强度 = 功率 / 立体角)

立体角: 角度在3维空间中的延伸; 
- Ω = A / r² (立体角 = 面积 / 半径的平方)

#### Irradiance
定义: 每个单位面积上的能量

#### Radiance
定义: 每立体角上的Irradiance/每单位面积上的Intensity

#### Bidirectional Reflectance Distribution Function (BRDF)

#### Rendering Equation(渲染方程)

### 全局光照
直接光照和间接光照的集合

### Path Tracing(路径追踪)
```
shade(p, wo)
    Randomly choose N directions wi~pdf
    Lo = 0.0
    For each wi:
        Trace a ray r(p, wi)
        If ray r hit the light
            Lo += (1/N) * L_i * f_r * cosine / pdf(wi)
        Else If ray hit a object at q
            Lo += (1/N) * shade(q, -wi) * f_r * cosine / pdf(wi)
    Return Lo

因为N随机的话会指数增加，所以要限制N的范围-N = 1
shade(p, wo)
    Randomly choose One direction wi~pdf(w)
    Trace a ray r(p, wi)
    If ray r hit the light
        Return L_i * f_r * cosine / pdf(wi)
    Else If ray hit a object at q
        Return shade(q, -wi) * f_r * cosine / pdf(wi)
```
如果N选择1的话噪声会很大，但是通过1个像素的路径可以有很多，我们只要对多条路径的值求平均就可以减少噪声
```
ray_generation(cameraPos, pixel)
    Uniformly choose N sample positions within the pixel
    pixel_radiance = 0.0
    For each sample in the pixel:
        Shoot a ray r(cameraPos, camera_to_sample)
        If ray r hit the scene at p
            pixel_radiance += 1/N * shade(p, sample_to_camera)
    Return pixel_radiance
```

目前还有个问题：路径追踪算法没有结束，会无限递归\
解决方法: 俄罗斯轮盘赌(Russian Roulette) - RR
- 设置一个可能值P
- 在P的概率下，发射射线ray并且返回结果Lo/P
- 在1-P的概率下，返回0
- E = P * Lo/P + (1-P) * 0 = Lo(expectation)
```
shade(p, wo)
    Manually specify a probability P_RR
    Randomly select ksi in a uniform dist. in [0, 1]
    If ksi > P_RR return 0.0


    Randomly choose One direction wi~pdf(w)
    Trace a ray r(p, wi)
    If ray r hit the light
        Return L_i * f_r * cosine / pdf(wi) / P_RR
    Else If ray hit a object at q
        Return shade(q, -wi) * f_r * cosine / pdf(wi) / P_RR
```
但是物体向随机方向发射射线会有浪费，所有转化为光源采样\
做换元dw = dA cos ϕ / ||x' - x||² (x是物体上的点，x'是光源上的点)\
将光线传播分成两个部分:
1. 光源 - 直接对光源采样
2. 其他物体反射 - 递归调用shade函数
```
shade(p, wo)
    Uniformly sample the ligth at x' (pdf_light = 1/A)
    Shoot a ray r(x', p)
    If ray r hit the scene at p
        L_dir = 0.0
    Else
        L_dir = L_i * f_r * cosθ * cosθ‘ / |x' - p|² / pdf_light

    L_indir = 0.0
    Test Russian Roulette with probability P_RR
    Uniformly sample the hemisphere towards wi (pdf_hemi = 1/2pi)
    Trace a ray r(p, wi)
    If ray r hit a non-emitting object at q
        L_indir = shade(q, -wi) * f_r * cosine / pdf_hemi / P_RR
    
    Return L_dir + L_indir
```

---
### 辐射度量学详解

辐射度量学的核心目标是**为光的测量建立一个精确、基于物理的量化体系**。在它出现之前，我们对“明暗”、“亮度”的描述是相对和主观的。辐射度量学引入了一系列精确定义的物理量，使得我们能够像描述“长度是多少米”、“质量是多少千克”一样，去描述“光”的能量、强度和分布。

#### 1. 立体角
这是理解后续所有概念的基础。
*   **定义**：**立体角是角度在三维空间中的延伸**。平面角衡量的是一个方向在二维平面上张开的“大小”（单位是弧度），而立体角衡量的是一个锥面在三维空间中张开的“大小”（单位是球面度，sr）。
*   **直观理解**：想象在空间中的一个点，向四周发射一个锥体，这个锥体在单位球面（半径为1的球面）上“切”下来的那块面积的大小，就是这个锥体所对应的立体角的数值。
*   **公式**：对于一个在球面上投影面积为A的小面元，其对于球心所张的立体角为 **Ω = A / r²**。当球半径为1时，立体角在数值上就等于投影面积A。

#### 2. Radiant Flux
*   **中文常译**：辐射通量/光通量
*   **定义**：**单位时间内通过某个面或由光源发出的总能量**。可以简单理解为光的“功率”，单位是瓦特。
*   **理解**：它描述的是光源的“总发电量”或一个区域光的“总流量”，但不关心能量是如何分布的。例如，一个100瓦的白炽灯和一个100瓦的LED灯，它们的 Radiant Flux 是相同的，但亮度和光色分布可能完全不同。

#### 3. Radiant Intensity
*   **中文常译**：辐射强度
*   **定义**：**描述点光源在单位立体角内发出的功率**。公式为 **I = dΦ / dω**， 即单位立体角内的辐射通量。
*   **理解**：它描述了光源能量在**方向**上的分布密度。对于一个各向同性的理想点光源（向所有方向均匀发光），其强度 I = Φ / 4π（因为整个球面的立体角是4π）。它解决了“光源在某个特定方向上有多亮”的问题。

#### 4. Irradiance
*   **中文常译**：辐照度
*   **定义**：**单位面积上所接收到的辐射通量**。公式为 **E = dΦ / dA**， 即单位面积接收到的功率。
*   **理解**：它描述的是**一个表面接收到了多少光能**，是照射到表面的量。这是着色计算中一个关键输入。
*   **重要性质**：对于一个点光源，**Irradiance 与距离的平方成反比**。这是因为从光源发出的总通量 Φ 会均匀分布在一个不断变大的球面上（面积与 r² 成正比），所以单位面积接收到的能量（E）自然与 r² 成反比。同时，**Irradiance 与光线入射角的余弦（cosθ）成正比**（即兰伯特余弦定律），表面越“正对”光线，接收到的能量越多。

#### 5. Radiance
*   **中文常译**：辐射率
*   **定义**：这是**辐射度量学中最核心、最基础的物理量**。它衡量的是**在单位立体角、单位投影面积上的辐射通量**。公式为 **L = d²Φ / (dω dA cosθ)**。可以从两个等价角度理解：
    1.  对于一个微小表面 dA， Radiance 是它在某个方向（dω）上发射/反射/透射的光强。
    2.  对于在空间传播的一束光线，Radiance 精确描述了这束光线的“亮度”。
*   **理解**：**Irradiance 描述“表面收到多少光”，而 Radiance 描述“光在空间中传播时的状态”**。它是连接光源、物体表面和相机（眼睛）的桥梁。
*   **重要性质**：在真空中沿直线传播时，**Radiance 保持不变**。这意味着，当我们从不同距离观察同一束光时，虽然它的 Irradiance 会随距离平方衰减，但其本质的“亮度”（Radiance）是不变的。这保证了视觉的一致性。

#### 6. BRDF
*   **全称**：Bidirectional Reflectance Distribution Function
*   **中文**：双向反射分布函数
*   **定义**：描述了**当一束光从某个方向入射到一个表面点时，能量如何被反射到各个出射方向**。其数学定义基于 Radiance 和 Irradiance：**BRDF = dL_r / dE_i**。即，出射的 Radiance 与入射产生的 Irradiance 的比值。
*   **理解**：**BRDF 定义了一个材质的外观**。它回答了“一束光从这个角度打过来，从这个角度看过去，这个点应该有多亮？”这个问题。不同的 BRDF 模型（如漫反射的兰伯特模型、镜面反射的模型、复杂的微表面模型）对应了不同的材质，如石膏、金属、塑料、丝绸等。

#### 7. The Rendering Equation
*   **中文**：渲染方程
*   **定义**：由 Jim Kajiya 于1986年提出的方程，**完美地描述了光线在场景中达到稳态分布时，一个点向某个方向发出的 Radiance**。其标准形式为：
    **L_o(p, ω_o) = L_e(p, ω_o) + ∫_Ω f_r(p, ω_i, ω_o) L_i(p, ω_i) cosθ_i dω_i**
*   **解读**：
    *   **L_o**: 从点 p 向方向 ω_o 出射的 Radiance（即我们最终想求的颜色/亮度）。
    *   **L_e**: 点 p 自身作为光源发出的 Radiance（自发光）。
    *   **积分部分**: 来自场景中**所有方向** ω_i 的入射光 Li，经过该点材质的 BRDF 作用后，对出射方向 ω_o 的贡献总和。`cosθ_i` 是入射角因子。
*   **意义**：**这是全局光照的数学描述，是几乎所有现代物理渲染算法的终极目标**。它表达了：一个点的出射光 = 它的自发光 + 来自环境中所有其他点的反射光。这是一个递归的、能量守恒的方程。文档后面提到的 Path Tracing 算法，就是通过随机采样来求解这个积分方程的一种蒙特卡洛方法。

### 概念串联与总结
现在，让我们把这些概念放入一个完整的光线传播故事中：

1.  **光源**：一个灯泡以一定的 **Radiant Intensity** 向四周发射光能。
2.  **光的传播**：这束光在空间中以 **Radiance** 来度量其传播状态。
3.  **到达表面**：当这束光照射到一个微小表面 dA 上时，我们通过 **Irradiance** 来度量这个表面接收到了多少光能。
4.  **表面交互**：表面根据其材质属性（由 **BRDF** 定义），决定将接收到的能量（Irradiance）如何重新分配，向各个方向反射出去，形成新的出射 **Radiance**。
5.  **全局平衡**：场景中每一个点都在重复步骤3和4，所有点的入射和出射光共同构成了一个复杂的能量交换网络，这个网络最终达到的平衡状态，就由 **渲染方程** 所描述。
6.  **进入人眼/相机**：从物体表面指向相机像素方向的那束出射 **Radiance**，经过一系列传播后进入相机，最终决定了像素的颜色。

**因此，辐射度量学为我们提供了描述这个“光的故事”中每一个环节的精确“度量衡”，而渲染方程则是这个故事的完整剧本。** 理解这些概念是掌握Path Tracing等现代渲染算法的必备理论基础。