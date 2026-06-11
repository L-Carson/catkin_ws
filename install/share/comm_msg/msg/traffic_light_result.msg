Header      header
uint8       type        # 交通灯显示类型：1(读秒类型) 2(频闪类型) 3(高频亮灭型)
label       label       # 交通灯状态（红灯、绿灯等）
uint8       seconds     # 交通灯读秒
uint8       status      # 频闪类型交通灯的状态：0(常亮) 1(频闪)