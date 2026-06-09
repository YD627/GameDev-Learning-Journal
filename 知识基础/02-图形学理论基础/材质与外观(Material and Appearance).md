## 材质与外观(Material and Appearance)
### 材质(Material)
**材质 == BRDF**
#### Difuse / Lambertian Material
光均匀反射到任何方向 - f_r = ρ / π 其中(ρ - albedo(color))

#### Glossy Material
有一些镜面反射的材质

#### Ideal reflective / refractive material(BSDF*)
水或者玻璃等材质（既有反射又有折射）

#### 菲涅耳项
Sclick's approximation
R(θ) = R0 + (1 - R0) * (1 - cos(θ))^5
R0 = ((n1 - n2) / (n1 + n2))^2

#### 微表面模型
Rough surface（粗糙表面）
- Macroscale : flat & rough
- Microscale : bumpy & specular
我们认为微表面是一个镜面\
核心公式: f(i, o) = F(i, h) G(i, o, h) D(h) / 4(n, i)(n, o)
- F(i, h): 总共有多少能量要被反射
- G(i, o, h): 修正边界
- D(h): 法线分布

#### 各向同性/各向异性材质
核心区别：
- 各向同性材质：光在不同方向反射的强度相同
- 各向异性材质：光在不同方向反射的强度不同

#### Properties of BRDFs（BRDF的性质）
1. 非负性: f_r(w_i -> w_r) >= 0
2. 线性性质
3. 可逆性：交换入射方向和出射方向，得到的值相同
4. 能量守恒

#### 测量BRDF
方法：枚举所有的入射和出射方向进行测量
- MERL BRDF Database