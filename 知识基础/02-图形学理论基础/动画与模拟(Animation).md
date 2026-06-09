## 动画与模拟(Animation)
动画的形成：把很多的图按顺序播放，就形成了动画。
- Film: 24fps
- Video: 30fps
- Game: 60fps
- Virtual Reality: 90fps

### 关键帧(Key Frame)
关键帧：在动画中，一些重要的帧，需要手动设置，才能形成动画。
核心就是在关键帧之间进行插值，形成动画。

### 物理模拟(Physics Simulation)
#### 质点弹簧系统(Mass Spring System)
- 基础单元是一个弹簧连接着两个质点
    - fa->b = ks*(b - a)  --- 假设弹簧长度为0
    - fb->a = -fa->b

    - fa->b = ks (b-a)*(||b-a||-ls)/||b-a||  --- 假设弹簧长度为ls

- 引入摩擦力: f = -kd*v
    - 问题：描述不了弹簧系统内部的力，只能描述弹簧系统外部的力。

- fb = -kd * ((b-a)*(v_b-v_a)/||b-a||) * (b-a)/||b-a||
    - 其中b的速度减去a的速度就是二者的相对速度，再点乘a到b的方向，就是二者的相对速度在弹簧方向上的分量，这个分量产生的阻力就会影响弹簧内部的运动。

### 粒子系统(Particle System)

### Forward Kinemation(前向运动学)
能简单地描述一个关节骨架的运动。知道关节的角度，就可以计算出关节的位置。
Articulated Skeleton(关节骨架)
- Topology(拓扑结构：什么连接着什么)
- Geometric relations from joints(关节之间的几何关系)
- Tree structure(树状结构)

Joint types
- Pin(1D rotation)
- Ball(2D rotation)
- Prismatic joint(translation)

### Inverse Kinemation(反向运动学)


### Rigging
Rigging is a set of higher level controls on a character that allow more rapid & intuitive modification of pose，deformations，expression，etc.

### Blend Shapes
Instead of skeleton, interpolate directly between surfaces.

### Motion Capture(运动捕获)
Strength:
- Can capture large amount of real data quickly
- Realism can be high

Weakness:
- Complex and costly set-ups
- Captured animation may not meet artistic needs，requiring alterations.
