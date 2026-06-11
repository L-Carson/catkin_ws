lidar_det 无源码运行时包
========================

前提：目标 Orin 已装 ROS Noetic，且 JetPack/TensorRT 与打包机一致（见 VERSION.txt）。

部署（目标机无需 catkin_ws/src、无需 catkin_make）：

  tar -xzf lidar_det_runtime_*.tar.gz -C /opt   # 或任意目录
  cd /opt/lidar_det_runtime
  export ROS_MASTER_URI=http://localhost:11311    # 按车队配置
  ./start_lidar_det.sh

或：

  source /opt/ros/noetic/setup.bash
  source setup.bash
  roslaunch lidar_det_node lidar_det_node.launch

目录要点：
  lib/lidar_det_node/lidar_det_node          节点可执行文件
  lib/lidar_det_node/libtransfusion_backend.so  预编译后端（勿重编）
  lib/lidar_det_node/libspconv.so
  share/lidar_det_node/model/                模型与 engine
  share/lidar_det_node/launch|config/        启动与参数

自检：
  ldd lib/lidar_det_node/lidar_det_node | grep -E 'transfusion|spconv'
  rospack find lidar_det_node
  python3 -c "from comm_msg.msg import boxArray; print('comm_msg OK')"
  rostopic echo /trans_net/box   # 需先 source setup.bash 且节点在发布

接入 navZero 开机自启（每台车执行一次）：
  ./install_navzero_autostart.sh
