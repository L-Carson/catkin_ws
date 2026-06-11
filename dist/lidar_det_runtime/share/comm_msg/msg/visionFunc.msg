# 功能ID
uint8 NULL_FUNC   = 0   # NULL
uint8 QR_CODE_DET = 1   # 二维码检测
uint8 NET_DET     = 2   # 网络检测
uint8 NET_SEG     = 3   # 网络分割

# msg
uint8       funcId      # 功能ID
bool        ipu         # 是否使用图像处理单元
uint8       hz          # 执行频率
string[]    cameras     # 相机ID：front_left(前左), front_mid(前中), front_right(前右), rear_mid(后中) etc.
label[]     usingLabel  # 当前正在使用的类别标签