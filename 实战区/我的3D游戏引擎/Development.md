# 项目目录

```text
Engine
│
├── Core
│   ├── Application.h
│   ├── Application.cpp
│   ├── Window.h
│   ├── Window.cpp
│   ├── Time.h
│   ├── Time.cpp
│   ├── Log.h
│   └── Log.cpp
│
└── main.cpp
```

---

# 开发日志

## 2026-06-09

### 今日目标

开始将原有 OpenGL 游戏项目重构为可长期维护的个人图形学/游戏引擎。

本次开发重点为 Core 模块的基础搭建。

---

## 完成内容

### 1. Application 模块

新增：

```cpp
Application.h
Application.cpp
```

职责：

* 引擎生命周期管理
* 主循环管理
* 初始化核心模块
* 程序退出管理

目前主程序入口已简化为：

```cpp
int main()
{
    Application app;
    app.Run();

    return 0;
}
```

实现了引擎入口与业务逻辑解耦。

---

### 2. Window 模块

新增：

```cpp
Window.h
Window.cpp
```

职责：

* GLFW 窗口创建
* OpenGL Context 初始化
* 事件轮询
* Buffer Swap

封装前：

```cpp
glfwCreateWindow(...)
glfwSwapBuffers(...)
glfwPollEvents(...)
```

直接出现在 main.cpp。

封装后：

```cpp
m_Window->Update();
```

Application 不再依赖 GLFW 具体实现。

架构关系：

```text
Application
    ↓
 Window
    ↓
  GLFW
```

---

### 3. Time 模块

新增：

```cpp
Time.h
Time.cpp
```

职责：

* DeltaTime 计算
* 总运行时间统计
* 未来 FPS 统计支持

封装前：

```cpp
float deltaTime;
float lastFrame;
```

全局变量管理。

封装后：

```cpp
Time::DeltaTime();
Time::TotalTime();
```

实现统一时间系统。

示例：

```cpp
position += velocity * Time::DeltaTime();
```

---

### 4. Log 模块

新增：

```cpp
Log.h
Log.cpp
```

职责：

* 统一日志输出
* 调试信息记录
* 错误信息记录

支持：

```cpp
Log::Info(...)
Log::Warn(...)
Log::Error(...)
```

输出格式：

```text
[INFO][14:23:15] Window Created
[WARN][14:23:18] Texture Missing
[ERROR][14:23:20] Shader Compile Failed
```

重构内容：

将重复的时间获取逻辑封装至：

```cpp
GetCurrentTime()
```

将重复的日志输出逻辑封装至：

```cpp
Print(...)
```

减少代码重复，提高可维护性。

---

## 架构变化

原始结构：

```text
main.cpp
 ├── GLFW
 ├── DeltaTime
 ├── Log
 └── Game Logic
```

当前结构：

```text
Application
 ├── Window
 ├── Time
 └── Log
```

开始形成基础引擎架构。

---

## 遇到的问题

### localtime 编译警告

Visual Studio 对：

```cpp
localtime()
```

产生 C4996 警告。

解决方案：

```cpp
localtime_s(...)
```

替代传统实现。

提高线程安全性与平台兼容性。

---

## 下一阶段计划

Core 模块：

* [ ] Input
* [ ] Event System
* [ ] Layer System

Renderer 模块：

* [ ] Shader
* [ ] Texture
* [ ] Mesh
* [ ] Renderer

目标：

将现有 OpenGL 游戏项目逐步迁移至 Engine 架构下运行。

---

## 开发感悟

今天首次完成了从“课程项目代码”向“引擎代码”的结构拆分。

虽然功能没有增加，但项目的可维护性显著提升。

后续开发重点将从“继续堆功能”转向“建立稳定架构”，为未来加入 PBR、ECS、动画系统和编辑器功能做准备。
