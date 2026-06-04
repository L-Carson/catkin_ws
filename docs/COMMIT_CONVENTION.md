# Git 提交规范

本规范适用于本仓库及关联的 ROS `catkin_ws` 工作空间。提交信息风格参考 [Conventional Commits](https://www.conventionalcommits.org/)。

---

## 1. 提交信息格式

```
<类型>(可选范围): 简短描述

[可选正文：说明修改原因、影响范围、验证方式]

[可选页脚：Closes #123 / BREAKING CHANGE: ...]
```

**示例：**

```
<fix>(lidar_det_node): 修复 CUDA 流创建失败时节点空转

prepare() 失败时打印明确错误并停止推理，避免无检测输出仍占用 GPU。
```

---

## 2. 类型（Type）

| 类型 | 含义 | 使用场景 |
|------|------|----------|
| `<feat>` | 新功能 | 新节点、新话题、新算法、新 launch |
| `<fix>` | Bug 修复 | 崩溃、错误结果、逻辑错误、兼容性问题 |
| `<docs>` | 文档 | README、注释、安装与运行说明 |
| `<style>` | 格式 | 缩进、命名、空行；**不改变逻辑** |
| `<refactor>` | 重构 | 结构调整、提取模块；**不改变对外行为** |
| `<perf>` | 性能 | 加速推理、降低延迟、减少内存占用 |
| `<test>` | 测试 | 单元测试、集成测试、bag 回放脚本 |
| `<build>` | 构建 | CMakeLists、package.xml、编译选项 |
| `<ci>` | 持续集成 | GitHub Actions、自动化流水线 |
| `<chore>` | 杂项 | `.gitignore`、工具配置、无关业务的小改动 |
| `<config>` | 配置 | 参数 yaml、launch 默认值 |
| `<deps>` | 依赖 | CUDA、TensorRT、ROS 包版本变更 |
| `<data>` | 数据 | 标定、配置数据（大文件请用 Git LFS） |
| `<revert>` | 回滚 | 撤销某次提交 |

---

## 3. 范围（Scope，可选）

括号内写**包名或模块名**，便于检索与生成变更日志：

```
<fix>(lidar_det_node): ...
<feat>(perception): ...
<docs>(catkin_ws): 补充编译说明
```

多包联动改动可省略 scope，在正文中列出涉及包名。

---

## 4. 描述规则

1. 使用**祈使句、现在时**：写「添加」「修复」，不写「添加了」「修复了」。
2. 标题**一行说清做了什么**，避免 `update`、`fix bug`、`修改代码`。
3. 正文补充**为什么改**、**如何验证**（尤其对 `<fix>` / `<refactor>`）。
4. **一次提交只做一件事**；大功能拆成多个提交。
5. 不兼容变更在正文或页脚标明：

   ```
   BREAKING CHANGE: output_topic 由 /box 改为 /trans_net/box
   ```

---

## 5. 示例

### 新功能

```
<feat>(lidar_det_node): 支持 transfusion 后端 score 阈值参数化
```

### Bug 修复

```
<fix>(lidar_det_node): 修复 Detector prepare 失败后仍订阅点云的问题
```

### 文档

```
<docs>: 添加 roslaunch 启动步骤与 CUDA 环境检查说明
```

### 重构

```
<refactor>(detector): 将 TensorRT 加载逻辑抽到独立 Backend 类
```

### 配置

```
<config>(lidar_det_node): 默认 input_topic 设为 /MainLidar/trans_points
```

### 构建

```
<build>(lidar_det_node): 链接 CUDA 11.8 与 TensorRT 8.6
```

### 回滚

```
<revert>: 回滚 transfusion 默认引擎路径

Reverts: a1b2c3d4e5f6789012345678abcdef012345678
```

---

## 6. 不推荐写法

| 不推荐 | 推荐 |
|--------|------|
| `fix bug` | `<fix>(xxx): 修复 yyy 在 zzz 条件下崩溃` |
| `update` | `<feat>(xxx): 添加 zzz 功能` |
| `WIP` | 完成后再提交 |
| `修改代码` | 具体说明改动内容 |
| 一次提交包含多个无关改动 | 拆分为多次提交 |

---

## 7. 分支命名（可选）

与提交类型保持一致：

```
feat/lidar-det-transfusion
fix/cuda-stream-init
docs/setup-guide
refactor/detector-backend
```

---

## 8. 类型速查

```
<feat>      新功能
<fix>       Bug 修复
<docs>      文档
<style>     代码格式（无逻辑变更）
<refactor>  重构
<perf>      性能优化
<test>      测试
<build>     构建/编译
<ci>        CI/CD
<chore>     杂项维护
<config>    配置/launch/参数
<deps>      依赖变更
<data>      数据/标定
<revert>    回滚
```

---

## 9. 启用提交模板

仓库根目录已提供 `.gitmessage` 模板，在本仓库内执行：

```bash
git config commit.template .gitmessage
```

之后执行 `git commit` 时会自动带出模板提示。
