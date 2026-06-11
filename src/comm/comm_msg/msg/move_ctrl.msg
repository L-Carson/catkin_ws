Header  header

uint8   isExistTask
uint8   isTeleCtrl
uint8   isCleaning
float32 steeringAngle
float32 moveLinearVel
uint8   isMoveSmooth
uint8   isStopSmooth

# 发给安全冗余模块新增的消息
uint8   isUseSecurityRedundancy
uint8   isIgnoreLaserData
uint8   isIgnoreUltrasonicData
uint8   planningModel          # 0无效模式, 1贴边模式, 2跟线模式, 3发点远程遥控模式, 4方向远程遥控模式, 5调试工具遥控车辆
uint8   brushModel             # 0关刷模式，1开刷模式