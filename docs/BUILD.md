# catkin_ws 编译说明

## 包布局（comm_msg 唯一来源）

`comm_msg` **只使用** `src/comm/comm_msg/`（整车 comm 仓库内版本）。
**不要**在 `src/` 下再放一份 `comm_msg`，否则会报重名错误。

```
src/
├── comm/
│   ├── comm/          # libcomm.a（ST_LOG 等）
│   ├── comm_msg/      # ← box / boxArray / label 消息定义
│   ├── comm_srvs/     # comm 依赖
│   └── core_lib/      # comm 依赖
├── lidar_det_interface/
└── lidar_det_node/
```

## 编译 lidar_det_node（开发机 / Orin）

```bash
cd ~/ly/3d_detection/catkin_ws
source /opt/ros/noetic/setup.bash

# 首次或删了 build/devel 后：全量编译依赖链
catkin_make -DCATKIN_WHITELIST_PACKAGES="comm_msg;comm_srvs;core_lib;comm;lidar_det_interface;lidar_det_node"

source devel/setup.bash
roslaunch lidar_det_node lidar_det_node.launch
```

> **包名说明**：源码在 `src/comm/comm_msg/`，catkin 白名单里写包名 `comm_msg`
>（不是路径 `comm/comm_msg`）。不要在 `src/` 下再放第二份 `comm_msg`。

仅改节点源码时，同一命令会增量编译，通常几十秒以内。

## 运行

```bash
source devel/setup.bash
export ROS_MASTER_URI=http://localhost:11311   # 按车队实际 Master
roscore &                                        # 本机当 Master 时
roslaunch lidar_det_node lidar_det_node.launch
```

日志（ST_LOG）默认：`~/map_config/log/debug/processprint/lidar_det_node_st.log`

## 打包无源码运行时（可选）

```bash
./scripts/package_lidar_det_runtime.sh
# 产物：dist/lidar_det_runtime_aarch64_YYYYMMDD.tar.gz
```

详见 `src/lidar_det_node/DEPLOY_RUNTIME.md`。

## 常见问题

| 现象 | 处理 |
|------|------|
| `Multiple packages found with the same name "comm_msg"` | 删除 `src/comm_msg`；确认 `src/comm/comm_msg/` 下**没有** `CATKIN_IGNORE` |
| `Could not find comm` | 白名单加上 `comm_srvs;core_lib;comm` |
| `transfusion_backend not found` | 确认 `src/lidar_det_node/vendor/lib/*.so` 存在 |
| 删了 `build/` `devel/` | 重新执行上文 `catkin_make`；或临时用 `source install/setup.bash`（若 install 仍在） |
