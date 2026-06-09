## 实时阴影(Real-Time-Shadows)
Shadow Mapping的基本实现：先从光源出发渲染一边场景，输出light能看到的最浅或最近的深度作为Shadow Map。接着，从相机视角渲染场景，结合Shadow Map，判断每个像素是否在阴影中。

Shadow Mapping 的主要问题
1. Shadow Acne（阴影痤疮）
    - 深度精度误差导致的：本来应该亮的地方被误判为阴影。
    - 解决方法：Depth Bias比较时添加一个偏移值
        - Bias 太大会导致：阴影和物体分离

Shadow Mapping技术：
1. Second-depth shadow mapping：记录两张深度贴图最小深度和次小深度贴图，然后实际比较时是取二者的中间值来比较深度。(实际上没人用)

### 软阴影
#### Percentage Closer Filtering(PCF)
- 核心做法：对周围多个 Shadow Map texel 做深度测试，再求平均。
    1. 在 Shadow Map 周围采样多个点
    2. 每个点判断是否在阴影中
    3. 将结果平均

#### Percentage Closer Soft Shadows(PCSS)
目的：让阴影模糊程度随着距离变化，从而模拟真实世界的软阴影。

核心公式
```
w_penumbra = (z_r - z_b) * w_ligth / z_b
```
- z_r：receiver 深度
- z_b：blocker 深度
- w_ligth：光源尺寸

#### Variance Soft Shadow Mapping(VSSM)
关键思想：快速计算一片区域内的平均值和方差

平均值：
- MIPMAPing
- Summed Area Tables(SAT)

方差:
- VAR(X) = E(X²) - E(X)²
- 仅仅需要再生成一张深度平法的Shadow Map，就可以快速计算出任意区域内的方差。

核心公式：切比雪夫不等式
```
VAR(X) <= C * E(|X - E(X)|)
```
- C：常数，取值范围为[1, 2]

关键思想：
- Blocker(z < t),avg.z_occ(我们希望计算的)
- Non-blocker(z >= t),avg.z_unocc
```
N1 * z_unocc / N  +  N2 * z_occ / N = z_Avg
```
- 近似: N1/N = P(x>t),切比雪夫
- 近似: N2/N = 1 - P(x>t)
- Z_unocc我们并不知道，但是可以近似为Z_unocc = t(阴影接受是一个平面)
