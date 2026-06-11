uint8                   is_open     # 是否开启交通灯检测：0(关闭) 1(开启)
float32                 pixel_bias  # 像素偏差
float32                 rect_bias   # 检测框匹配率偏差
traffic_light_info      target      # 目标交通灯信息
traffic_light_info[]    nears       # 周边交通灯信息