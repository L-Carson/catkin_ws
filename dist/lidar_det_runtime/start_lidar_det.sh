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
