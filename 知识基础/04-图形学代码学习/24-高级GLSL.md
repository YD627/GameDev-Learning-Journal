# 高级GLSL（Advanced GLSL）

---

高级GLSL章节主要介绍了OpenGL中一些高级着色器技巧，包括内建变量、接口块（Interface Block）、Uniform缓冲对象（UBO）等内容。这些技术能够提高着色器之间的数据组织能力，并减少CPU向GPU传递重复数据的开销。

## GLSL中的内建变量

---

OpenGL提供了一些特殊的内建变量（Built-in Variables），可以直接在着色器中使用。

在顶点着色器中最常见的是：

```glsl
gl_Position
```

它表示顶点最终的裁剪空间坐标，必须在顶点着色器中赋值。

除此之外，还有：

```glsl
gl_VertexID
```

用于获取当前顶点的索引编号。

```glsl
gl_InstanceID
```

用于实例化渲染时获取当前实例编号。

示例：

```glsl
#version 330 core

void main()
{
    if(gl_VertexID == 0)
    {
        // 对第一个顶点进行特殊处理
    }
}
```

### gl_FragCoord

在片段着色器中，可以使用：

```glsl
gl_FragCoord
```

获取当前片段在窗口空间中的坐标。

示例：

```glsl
#version 330 core

out vec4 FragColor;

void main()
{
    FragColor = vec4(gl_FragCoord.x / 800.0,
                     gl_FragCoord.y / 600.0,
                     0.0,
                     1.0);
}
```

运行效果：

* 屏幕左下角颜色较暗
* 右上角颜色逐渐变亮
* 根据屏幕坐标生成渐变色

---

## 接口块（Interface Block）

---

当多个变量需要在着色器阶段之间传递时，可以使用接口块统一管理。

普通写法：

```glsl
out VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;
```

片段着色器接收：

```glsl
in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;
```

这样做的好处：

* 提高代码可读性
* 方便管理大量输入输出变量
* 减少变量命名冲突

### 使用示例

顶点着色器：

```glsl
out VS_OUT
{
    vec3 Normal;
} vs_out;

void main()
{
    vs_out.Normal = aNormal;
}
```

片段着色器：

```glsl
in VS_OUT
{
    vec3 Normal;
} fs_in;

void main()
{
    vec3 normal = normalize(fs_in.Normal);
}
```

---

## Uniform缓冲对象（UBO）

---

在多个着色器程序中，经常会共享同一组Uniform数据，例如：

* 投影矩阵（Projection）
* 观察矩阵（View）
* 光源参数

如果每个Shader都单独传递一次：

```cpp
shader1.setMat4("projection", projection);
shader2.setMat4("projection", projection);
shader3.setMat4("projection", projection);
```

会产生大量重复操作。

因此OpenGL提供了Uniform Buffer Object（UBO）。

### 创建Uniform块

GLSL：

```glsl
layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};
```

多个Shader中都可以定义同样的Uniform块：

```glsl
layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};
```

这样所有Shader都可以共享同一块数据。

---

## std140布局规则

---

Uniform块中的数据需要遵守std140内存对齐规则。

例如：

```glsl
layout(std140) uniform ExampleBlock
{
    float value;
    vec3 offset;
};
```

实际布局：

| 变量          | 占用字节 |
| ----------- | ---- |
| float value | 4    |
| padding     | 12   |
| vec3 offset | 16   |

因为：

```text
vec3 按16字节对齐
```

所以会自动补齐（Padding）。

常见规则：

| 类型    | 对齐大小 |
| ----- | ---- |
| float | 4    |
| vec2  | 8    |
| vec3  | 16   |
| vec4  | 16   |
| mat4  | 64   |

因此设计UBO时应尽量使用：

```glsl
vec4
mat4
```

避免产生额外填充空间。

---

## 创建UBO

---

生成缓冲对象：

```cpp
unsigned int uboMatrices;
glGenBuffers(1, &uboMatrices);

glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);

glBufferData(
    GL_UNIFORM_BUFFER,
    2 * sizeof(glm::mat4),
    NULL,
    GL_STATIC_DRAW
);

glBindBuffer(GL_UNIFORM_BUFFER, 0);
```

绑定到绑定点：

```cpp
glBindBufferRange(
    GL_UNIFORM_BUFFER,
    0,
    uboMatrices,
    0,
    2 * sizeof(glm::mat4)
);
```

更新数据：

```cpp
glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);

glBufferSubData(
    GL_UNIFORM_BUFFER,
    0,
    sizeof(glm::mat4),
    glm::value_ptr(projection)
);

glBufferSubData(
    GL_UNIFORM_BUFFER,
    sizeof(glm::mat4),
    sizeof(glm::mat4),
    glm::value_ptr(view)
);
```

---

## 将Uniform块绑定到UBO

---

获取Uniform块索引：

```cpp
unsigned int uniformBlockIndex =
    glGetUniformBlockIndex(
        shader.ID,
        "Matrices"
    );
```

绑定到绑定点：

```cpp
glUniformBlockBinding(
    shader.ID,
    uniformBlockIndex,
    0
);
```

这样：

```text
Shader中的Matrices
        ↓
Binding Point 0
        ↓
UBO Buffer
```

所有使用该绑定点的Shader都会共享数据。

---

## 本章总结

---

高级GLSL主要学习了以下内容：

### 内建变量

```text
gl_Position
gl_VertexID
gl_InstanceID
gl_FragCoord
```

用于获取OpenGL运行时信息。

### Interface Block

```text
统一管理Shader输入输出变量
```

提高代码可维护性。

### Uniform Block

```text
统一管理Uniform数据
```

减少CPU向多个Shader重复传输数据。

### Uniform Buffer Object

```text
实现多个Shader共享同一份Uniform数据
```

提升渲染效率。

高级GLSL的核心思想是：

```text
组织数据
↓
共享数据
↓
减少状态切换
↓
提升渲染效率
```

这也是后续学习延迟渲染（Deferred Shading）、SSBO、实例化渲染等高级技术的重要基础。
