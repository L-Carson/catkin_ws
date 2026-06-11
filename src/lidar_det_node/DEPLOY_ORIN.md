# lidar_det_node 跨 Orin Nano 部署说明

> **推荐（无源码上车）**：见 [DEPLOY_RUNTIME.md](DEPLOY_RUNTIME.md) — 打包
> `dist/lidar_det_runtime_*.tar.gz`，目标机解压即用，**不需要 `src/` 与 `catkin_make`**。

目标：在**其他 Orin Nano**上直接启动 `lidar_det_node`，无需重编译重型后端
（`libtransfusion_backend.so`），也无需 TransFusion 源码树。

## 适用前提

- 目标机与参考机为**同款 Orin Nano、同 JetPack（R35.3.1 / TensorRT 8.5.2 / CUDA 11.4）**。
  `.so` 与 `.engine` 是按平台 + TRT 版本绑定的；JetPack 不一致需重新导出 engine。
- 目标机已安装 ROS Noetic。

## 已做的可移植化改造

1. **预编译产物随包携带**（`src/lidar_det_node/vendor/`）：
   - `vendor/lib/libtransfusion_backend.so`、`vendor/lib/libspconv.so`
   - `vendor/model/`：`*.scn.onnx`、`*weights*.bin`、`*bev*.engine`、`*head*.engine`
2. **构建时自动复用 vendored `.so`**：`catkin_make` 不再编译 TransFusion，
   也不需要 `3d_target_detection` 源码树。
3. **`$ORIGIN` 相对 RPATH**：节点把两个 `.so` 复制到可执行文件同目录，
   通过 `$ORIGIN` 定位。整个工作区拷到任意路径都能跑，
   **无需 `patchelf`，也无需设置 `LD_LIBRARY_PATH`**。
4. **模型路径自动探测**（无需写绝对路径），顺序：
   `vendor/model` → `share/lidar_det_node/model` → 旧 TransFusion 源码树。
   也可用 `model_dir` / `scn_onnx` 等参数覆盖。

## 部署步骤（拷贝 catkin_ws，走 catkin devel）

在参考机上确认 `vendor/lib`、`vendor/model` 已就绪（本仓库已包含）。

```bash
# 1) 拷贝整个 catkin_ws 到目标 Orin（路径/用户名可不同）
rsync -a catkin_ws/ saite@<orin-ip>:/home/saite/ly/3d_detection/catkin_ws/
#   或 scp -r / U 盘拷贝均可

# 2) 在目标机编译（只编译这 3 个轻量包，秒级；不会重编 .so）
cd ~/ly/3d_detection/catkin_ws
catkin_make -DCATKIN_WHITELIST_PACKAGES="comm_msg;comm_srvs;core_lib;comm;lidar_det_interface;lidar_det_node"

# 3) 运行
source devel/setup.bash
export ROS_MASTER_URI=http://localhost:11311   # 按实际 Master 设置
roscore &                                       # 若本机当 Master
roslaunch lidar_det_node lidar_det_node.launch
```

> 为什么仍要 `catkin_make`：ROS1 的 `devel/setup.bash` 烤死了绝对路径，
> 换路径/用户后必须重新生成一次。但它**只重编几十 KB 的节点**，
> `libtransfusion_backend.so` 始终复用 vendored 版本。

如果目标机路径/用户与参考机**完全一致**（同镜像克隆），可直接连 `devel/` 一起拷贝，
跳过第 2 步，`source devel/setup.bash` 后直接 `roslaunch`。

## 启动成功标志

```
model_dir=.../lidar_det_node/vendor/model
scn_onnx=.../transfusion_fast30.scn.onnx
...
Detector backend 'transfusion' ready.
lidar_det_node: sub '/MainLidar/trans_points' -> pub '/trans_net/box'
```

之后节点阻塞在 `ros::spin()` 等待点云输入，无输入时不再打印属正常。

## 自检命令

```bash
# 两个 .so 都应解析到可执行文件同目录（而非 3d_target_detection 树）
ldd devel/lib/lidar_det_node/lidar_det_node | grep -E "transfusion|spconv"
# RPATH 应含 $ORIGIN
readelf -d devel/lib/lidar_det_node/lidar_det_node | grep -iE "rpath|runpath"
```

## 更新模型 / 后端

只需替换 `src/lidar_det_node/vendor/` 下对应文件，重新执行部署步骤即可。
若换了用到的 engine 文件名，在 `config/lidar_det.yaml` 改对应项（写文件名即可，
会自动拼到 `model_dir`）。

## 接入 navZero 自动启动

已在 `navZero/reboot/reboot.sh` 中随感知节点一并启动（`pcpt_node` 之后）：

- 启动脚本：`~/navZero/exec/lidar_det_node`
- 日志：`~/map_config/log/debug/processprint/lidar_det_node.log`

脚本会依次 `source` navZero 与 `~/ly/3d_detection/catkin_ws/devel/setup.bash`，
再执行 `roslaunch lidar_det_node lidar_det_node.launch`。

节点自身日志走 comm 库的 `ST_LOG_*`（不受 navZero `rosconsole.config`
屏蔽 `ROS_INFO` 的影响），由 `config/lidar_det.yaml` 的 `log_file` 指定输出：

- 默认：`~/map_config/log/debug/processprint/lidar_det_node_st.log`
- 设为 `""` 则输出到 stdout（被 roslaunch / wrapper 重定向捕获）

`comm_msg` 仅来自 `src/comm/comm_msg/`（勿在 `src/` 再放副本）。
目标机编译白名单需包含 `comm_msg;comm_srvs;core_lib;comm`（详见 `docs/BUILD.md`）
（见上文部署步骤）。

手动验证：

```bash
/home/$USER/navZero/exec/lidar_det_node
# 或
tail -f ~/map_config/log/debug/processprint/lidar_det_node.log
```

## 备注：版本管理

`vendor/` 内为大体积二进制（约 50MB）。若 `catkin_ws` 纳入 git 且不希望提交，
可改用部署时单独同步 `vendor/`，或用 git-lfs。
