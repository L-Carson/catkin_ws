#!/bin/bash
# One-shot installer: hook lidar_det_node into navZero auto-start on THIS vehicle.
# Run on the target Orin after extracting the runtime bundle. Idempotent.
#
#   1) writes  ~/navZero/exec/lidar_det_node   (wrapper, runtime-first)
#   2) inserts the start line into ~/navZero/reboot/reboot.sh (if missing)
#
# Usage:  ./install_navzero_autostart.sh
set -euo pipefail

CUR_USER="${SUDO_USER:-$(id -un)}"
NAVZERO="/home/${CUR_USER}/navZero"
EXEC_WRAPPER="${NAVZERO}/exec/lidar_det_node"
REBOOT_SH="${NAVZERO}/reboot/reboot.sh"

if [ ! -d "${NAVZERO}" ]; then
    echo "ERROR: ${NAVZERO} not found -- is navZero installed on this vehicle?" >&2
    exit 1
fi

echo "==> [1/2] install wrapper: ${EXEC_WRAPPER}"
mkdir -p "${NAVZERO}/exec"
cat > "${EXEC_WRAPPER}" <<'WRAPPER_EOF'
#!/bin/bash
# Wrapper for lidar_det_node (TransFusion 3D detection).
# Started by navZero/reboot/reboot.sh like the other nodes (non-terminal env).
#
# Env resolution order:
#   1) source-free runtime bundle (deployment vehicles, no catkin_ws/src needed)
#   2) catkin_ws devel space (development machines)

cur_user=$(echo "$(dirname "$(readlink -f "$0")")" | awk -F "/" '{print $3}')

# navZero ROS env (ROS_MASTER_URI, ROS_LOG_DIR, CUDA paths, ...)
source "/home/$cur_user/navZero/reboot/setup.sh"

RUNTIME_CANDIDATES=(
    "/opt/lidar_det_runtime"
    "/home/$cur_user/lidar_det_runtime"
)
DEVEL_SETUP="/home/$cur_user/ly/3d_detection/catkin_ws/devel/setup.bash"

env_found=""
for rt in "${RUNTIME_CANDIDATES[@]}"; do
    if [ -f "$rt/setup.bash" ]; then
        source "$rt/setup.bash"
        env_found="runtime:$rt"
        break
    fi
done

if [ -z "$env_found" ] && [ -f "$DEVEL_SETUP" ]; then
    source "$DEVEL_SETUP"
    env_found="devel:$DEVEL_SETUP"
fi

if [ -z "$env_found" ]; then
    echo "lidar_det_node: no env found (tried ${RUNTIME_CANDIDATES[*]} and $DEVEL_SETUP)" >&2
    exit 1
fi
echo "lidar_det_node: using $env_found"

exec roslaunch lidar_det_node lidar_det_node.launch
WRAPPER_EOF
chmod +x "${EXEC_WRAPPER}"
echo "    done"

echo "==> [2/2] hook into ${REBOOT_SH}"
if [ ! -f "${REBOOT_SH}" ]; then
    echo "ERROR: ${REBOOT_SH} not found" >&2
    exit 1
fi

START_LINE='/home/$cur_user/navZero/exec/lidar_det_node             2>&1 >>$nav_log_dir/lidar_det_node.log &'

if grep -q 'exec/lidar_det_node' "${REBOOT_SH}"; then
    echo "    already present, skip"
else
    cp "${REBOOT_SH}" "${REBOOT_SH}.bak.$(date +%Y%m%d%H%M%S)"
    if grep -q 'exec/pcpt_node' "${REBOOT_SH}"; then
        # insert right after the pcpt_node start line (same style as other nodes)
        sed -i "\#exec/pcpt_node#a ${START_LINE}" "${REBOOT_SH}"
    else
        # fallback: insert before the final exit log
        sed -i "\#log_info \"reboot.sh exit\"#i ${START_LINE}" "${REBOOT_SH}"
    fi
    echo "    inserted (backup saved as ${REBOOT_SH}.bak.*)"
fi

echo ""
echo "Install OK. Verify:"
echo "  grep lidar_det_node ${REBOOT_SH}"
echo "  ${EXEC_WRAPPER}        # manual run; expect 'using runtime:/opt/lidar_det_runtime'"
echo "Auto-start takes effect on next reboot (or rerun navZero reboot.sh)."
