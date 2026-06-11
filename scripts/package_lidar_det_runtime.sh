#!/usr/bin/env bash
# Build (on reference Orin) and pack a SOURCE-FREE runtime bundle for other Orins.
#
# Output: catkin_ws/dist/lidar_det_runtime_<arch>_<date>.tar.gz
#         (extract anywhere, source setup.bash, roslaunch -- no src/build/devel)
#
# Prerequisite on build machine: ROS Noetic, same JetPack as target fleet.
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
WS_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"
DIST_DIR="${WS_DIR}/dist"
STAGING="${DIST_DIR}/lidar_det_runtime"
ARCH="$(uname -m)"
DATE_TAG="$(date +%Y%m%d)"
OUT_NAME="lidar_det_runtime_${ARCH}_${DATE_TAG}"
TARBALL="${DIST_DIR}/${OUT_NAME}.tar.gz"

PACKAGES="comm_msg;comm_srvs;core_lib;comm;lidar_det_interface;lidar_det_node"

echo "==> [1/5] catkin_make install (whitelist: ${PACKAGES})"
cd "${WS_DIR}"
source /opt/ros/noetic/setup.bash
catkin_make install -DCATKIN_WHITELIST_PACKAGES="${PACKAGES}"

echo "==> [2/5] stage runtime tree (install prefix only, no src)"
rm -rf "${STAGING}"
mkdir -p "${STAGING}"
rsync -a "${WS_DIR}/install/" "${STAGING}/"

echo "==> [3/5] prune dev-only artifacts (headers / interface / optional msg gens)"
rm -rf "${STAGING}/include"
rm -rf "${STAGING}/share/lidar_det_interface"
# Drop optional message bindings (lisp/js/roseus). Keep comm_msg Python so
# rostopic echo /trans_net/box and rospy tools work on deployment vehicles.
rm -rf "${STAGING}/share/common-lisp"
rm -rf "${STAGING}/share/gennodejs"
rm -rf "${STAGING}/share/roseus"
if [ -d "${STAGING}/lib/python3/dist-packages" ]; then
  find "${STAGING}/lib/python3/dist-packages" -mindepth 1 -maxdepth 1 \
    ! -name 'comm_msg' -exec rm -rf {} +
fi

echo "==> [4/5] add launcher + version metadata"
cat > "${STAGING}/VERSION.txt" <<EOF
lidar_det_runtime
arch=${ARCH}
built_at=$(date -Iseconds)
builder=$(hostname)
jetpack=$(cat /etc/nv_tegra_release 2>/dev/null | head -1 || echo unknown)
tensorrt=$(dpkg -l 2>/dev/null | awk '/libnvinfer8/ {print $2,$3; exit}' || echo unknown)
packages=${PACKAGES}
EOF

cat > "${STAGING}/start_lidar_det.sh" <<'EOF'
#!/usr/bin/env bash
# Source-free launcher. Usage (any path):
#   ./start_lidar_det.sh
#   ./start_lidar_det.sh --master http://192.168.1.15:11311
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source /opt/ros/noetic/setup.bash
source "${ROOT}/setup.bash"
: "${ROS_MASTER_URI:=http://localhost:11311}"
if [[ $# -ge 1 && "$1" == --master=* ]]; then
  export ROS_MASTER_URI="${1#--master=}"
fi
exec roslaunch lidar_det_node lidar_det_node.launch
EOF
chmod +x "${STAGING}/start_lidar_det.sh"
chmod +x "${STAGING}/lib/lidar_det_node/"*.so 2>/dev/null || true

# navZero auto-start installer (run once on each target vehicle)
cp "${SCRIPT_DIR}/install_navzero_autostart.sh" "${STAGING}/"
chmod +x "${STAGING}/install_navzero_autostart.sh"

cat > "${STAGING}/README_RUNTIME.txt" <<'EOF'
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
EOF

echo "==> [5/5] tarball"
mkdir -p "${DIST_DIR}"
rm -f "${TARBALL}"
tar -czf "${TARBALL}" -C "${DIST_DIR}" "$(basename "${STAGING}")"
ln -sfn "$(basename "${TARBALL}")" "${DIST_DIR}/lidar_det_runtime_latest.tar.gz"

echo ""
echo "Done: ${TARBALL}"
du -sh "${TARBALL}" "${STAGING}"
echo "Deploy on target: tar -xzf ${OUT_NAME}.tar.gz -C /opt && cd /opt/lidar_det_runtime && ./start_lidar_det.sh"
