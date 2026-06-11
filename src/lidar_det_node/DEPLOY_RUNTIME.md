# lidar_det 无源码运行时部署（推荐上车）

目标：目标 Orin **不需要** `catkin_ws/src`、**不需要** `catkin_make`，
解压即用 `roslaunch lidar_det_node`。

## 与「拷贝 catkin_ws」方案的区别


| 方式                     | 目标机需要源码   | 目标机需要编译             |
| ---------------------- | --------- | ------------------- |
| 拷贝 `catkin_ws` + devel | 是（`src/`） | 路径变时需 `catkin_make` |
| **本方案：runtime tar.gz** | **否**     | **否**               |


## 在参考机打包（只需做一次）

```bash
cd ~/ly/3d_detection/catkin_ws
chmod +x scripts/package_lidar_det_runtime.sh
./scripts/package_lidar_det_runtime.sh
```

产物：`dist/lidar_det_runtime_aarch64_YYYYMMDD.tar.gz`（约 55MB）

包内仅含 **install 前缀**：可执行文件、`.so`、模型、`launch/config`、
`comm_msg` 消息定义、catkin `setup.bash`。**不含任何 `.cpp` / `src/`**。

## 在目标 Orin 部署

```bash
# 1) 拷贝并解压到任意路径
scp dist/lidar_det_runtime_*.tar.gz saite@<orin>:/opt/
ssh saite@<orin>
cd /opt && tar -xzf lidar_det_runtime_*.tar.gz
cd lidar_det_runtime

# 2) 配置 ROS Master（按车队环境）
export ROS_MASTER_URI=http://localhost:11311

# 3) 启动（二选一）
./start_lidar_det.sh
# 或
source /opt/ros/noetic/setup.bash && source setup.bash
roslaunch lidar_det_node lidar_det_node.launch
```

**无需**安装 TransFusion 源码树、**无需** `3d_target_detection` 仓库。

## 包内布局

```
lidar_det_runtime/
  setup.bash              # catkin install 环境（路径随解压目录自动适配）
  start_lidar_det.sh      # 一键启动脚本
  VERSION.txt             # JetPack / TRT / 打包时间
  lib/lidar_det_node/
    lidar_det_node
    libtransfusion_backend.so
    libspconv.so
  share/lidar_det_node/
    launch/ config/ model/
  share/comm_msg/msg/     # 供 rostopic / 其它节点识别 boxArray
```

节点通过 `$ORIGIN` RPATH 加载同目录 `.so`；模型从 `share/lidar_det_node/model` 自动探测。

## 前提与限制

- 目标机：**同款 Orin Nano + 同 JetPack（R35.3.1 / TRT 8.5.2 / CUDA 11.4）**。
- 目标机：已装 **ROS Noetic**（`/opt/ros/noetic`）。
- 更新模型：在参考机替换 `vendor/model` 后重新执行打包脚本。

## 接入 navZero 自动启动

开机链路：`reboot.sh` → `exec/lidar_det_node`（与 `pcpt_node` 等节点同方式，
日志在 `~/map_config/log/debug/processprint/lidar_det_node.log`）。

> **注意**：navZero 是每台车一份的本地目录。**其他车上的 `reboot.sh`
> 没有 lidar_det_node 启动行，`exec/` 下也没有包装脚本**——runtime 包内的
> `install_navzero_autostart.sh` 会自动补齐这两样（幂等，可重复执行）。

### 每台新车的完整部署流程

```bash
# 1) 解压 runtime 包到 /opt（或家目录）
sudo tar -xzf lidar_det_runtime_*.tar.gz -C /opt
cd /opt/lidar_det_runtime

# 2) 一键接入 navZero 自启（写 exec/lidar_det_node + 改 reboot.sh，自动备份）
./install_navzero_autostart.sh

# 3) 验证（手动跑一次，确认能起来）
/home/$USER/navZero/exec/lidar_det_node
# 第一行应为: lidar_det_node: using runtime:/opt/lidar_det_runtime
# Ctrl-C 退出

# 4) 之后每次开机随 navZero 自动启动；或立即生效：
reboot
```

安装器做了什么：

1. 写入 `~/navZero/exec/lidar_det_node` 包装脚本，环境解析顺序：
  `/opt/lidar_det_runtime` → `~/lidar_det_runtime` → `catkin_ws/devel`（开发机回退）
2. 在 `reboot.sh` 的 `pcpt_node` 启动行后插入（已存在则跳过，原文件自动备份 `.bak.*`）：

```bash
/home/$cur_user/navZero/exec/lidar_det_node             2>&1 >>$nav_log_dir/lidar_det_node.log &
```

### 启动后查看运行状态

```bash
# 进程在吗
ps aux | grep lidar_det_node | grep -v grep
# 实时日志
tail -f ~/map_config/log/debug/processprint/lidar_det_node.log
# 话题有输出吗（需有点云输入）
rostopic hz /trans_net/box
```

## 自检

```bash
ldd lib/lidar_det_node/lidar_det_node | grep -E 'transfusion|spconv'
rospack find lidar_det_node
# 应打印当前解压目录下的 share/lidar_det_node
```

