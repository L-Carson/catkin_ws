# e.g. begin: rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl "{daq_fusion: 1, daq_hz: 1, daq_camera:[1,2,3,4,5,6,7,8,9,10], daq_lidar:[1,2,3,4,5]}"     # 相机+雷达：时间戳需对齐
# e.g. begin: rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl "{daq_fusion: 0, daq_hz: 1, daq_camera:[1,2,3,4,5,6,7,8,9,10], daq_lidar:[1,2,3,4,5]}"     # 相机+雷达：时间戳无需对齐

# e.g. begin: rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl "{daq_fusion: 1, daq_hz: 1, daq_camera:[1,2,3,4,5,6,7,8,9,10], daq_lidar:[]}"              # 相机：时间戳需对齐
# e.g. begin: rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl "{daq_fusion: 0, daq_hz: 1, daq_camera:[1,2,3,4,5,6,7,8,9,10], daq_lidar:[]}"              # 相机：时间戳无需对齐

# e.g. begin: rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl "{daq_fusion: 1, daq_hz: 1, daq_camera:[], daq_lidar:[1,2,3,4,5]}"                         # 雷达：时间戳需对齐
# e.g. begin: rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl "{daq_fusion: 0, daq_hz: 1, daq_camera:[], daq_lidar:[1,2,3,4,5]}"                         # 雷达：时间戳无需对齐

# e.g. end:   rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl "{daq_fusion: 1, daq_hz: 1, daq_camera:[], daq_lidar:[]}"                                  # 结束采集
# e.g. end:   rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl "{daq_fusion: 0, daq_hz: 1, daq_camera:[], daq_lidar:[]}"                                  # 结束采集（同上）

bool    daq_fusion      # 采集融合数据（相机+雷达：时间戳对齐）
uint8   daq_hz          # 采集频率
uint8[] daq_camera      # 采集相机数据
uint8[] daq_lidar       # 采集雷达数据
