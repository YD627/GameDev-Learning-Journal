## Unreal引擎介绍
### 背景介绍
- 1998 UE1.0 发布
- 2003 UE2.0 发布
- 2006 UE3.0 发布
- 2014 UE4.0 发布
- 2021 UE5.0 发布
引擎创建者：Tim Sweeney

UE的学习资料：B站和Youtube都有官方账号，其中就有教程。知乎上也有UE的官方账号

### Launcher（启动器）
Epic中可以下载

### 编辑器介绍
有很多模板可以选择 - 一般选C++模板

典型的功能：
1. 地图关卡的创建和分层
    - File -> New Level

2. 可视化游戏世界

3. 导航

4. 选取

5. 属性设置

6. 安放与对齐辅助工具
    - End：中心贴地
    - Alt + End：轴心贴地
    - shift + End：碰撞盒贴地

7. 快速迭代

8. Volume

9. 光源

### Content Browser（内容浏览器）
Content目录下的结构与windows资源管理器结构相同

- 资产导出时要查看其reference，避免导出后资产缺少纹理贴图等资源

### Project项目
- 项目文件：.uproject
- Content: 资产目录
- Source: 代码目录
- Config: 配置文件

```
--------------- Project -----------------
  ------------- Levels ----------------
    ---------- sublevels ------------
      --------- Actors -----------
        ------ Component -------
```

### Actor（角色）
- 可以放入关卡中的都是Actor
- 在C++中，AActor是所有Actor的基类

### Component（组件）
- 可以挂载在Actor上的组件
- 例如：MeshComponent、StaticMeshComponent、TextComponent等
- 可以在C++中自定义组件

### Settings（设置）
- 项目设置
- 游戏设置
- 其他设置