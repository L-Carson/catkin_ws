
1 运行程序需要在当前目录运行
source ../../devel/setup.sh
rosrun pcpt_net_node pcpt_net_node

2 生成engine 需要在配置文件中修改：

#1 低矮障碍物

version: 2 # 2 旧雷达驱动 3 新雷达驱动
modelType:  2  # 2 pointPillars 低矮障碍物        1 pointPillars 人车           0 为centerPoint人车
anchorFile: model/anchor_mul.bin
sensorNameRs16: /MiddleRsBpLidar/car_filtered_points/multiFrame  #人车点云
sensorNameRsBp: /MiddleRsBpLidar/car_filtered_points  #低矮障碍物多帧点云
istest: false #是否测试
testFileName: model/2.bin                   #测试文件
onnxFile: model/lidar_detect_8_3.onnx       #待生成engine的onnx文件
generateEngineFile: model/lidar_detect_8_3.onnx.bin #待生成engine文件
isOnnx2EngineFile: false                      #是否重新生成engine文件
modelFile: model/lidar_detect_8_3-.bin #模型文件 即 生成的engine文件



