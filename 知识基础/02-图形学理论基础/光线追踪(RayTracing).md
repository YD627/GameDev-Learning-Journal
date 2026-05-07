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

立体角: 角度在3维空间中的延申; 
- Ω = A / r² (立体角 = 面积 / 半径的平方)



