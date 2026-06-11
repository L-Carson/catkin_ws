;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::boxArray)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'boxArray (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::BOXARRAY")
  (make-package "COMM_MSG::BOXARRAY"))

(in-package "ROS")
;;//! \htmlinclude boxArray.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass comm_msg::boxArray
  :super ros::object
  :slots (_header _boxes ))

(defmethod comm_msg::boxArray
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:boxes __boxes) ())
    )
   (send-super :init)
   (setq _header __header)
   (setq _boxes __boxes)
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:boxes
   (&rest __boxes)
   (if (keywordp (car __boxes))
       (send* _boxes __boxes)
     (progn
       (if __boxes (setq _boxes (car __boxes)))
       _boxes)))
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; comm_msg/box[] _boxes
    (apply #'+ (send-all _boxes :serialization-length)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; comm_msg/box[] _boxes
     (write-long (length _boxes) s)
     (dolist (elem _boxes)
       (send elem :serialize s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; comm_msg/box[] _boxes
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _boxes (let (r) (dotimes (i n) (push (instance comm_msg::box :init) r)) r))
     (dolist (elem- _boxes)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;;
   self)
  )

(setf (get comm_msg::boxArray :md5sum-) "c75b4d4a4c4616720356b181655ca74f")
(setf (get comm_msg::boxArray :datatype-) "comm_msg/boxArray")
(setf (get comm_msg::boxArray :definition-)
      "Header  header
box[]   boxes

================================================================================
MSG: std_msgs/Header
# Standard metadata for higher-level stamped data types.
# This is generally used to communicate timestamped data 
# in a particular coordinate frame.
# 
# sequence ID: consecutively increasing ID 
uint32 seq
#Two-integer timestamp that is expressed as:
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
# time-handling sugar is provided by the client library
time stamp
#Frame this data is associated with
string frame_id

================================================================================
MSG: comm_msg/box
label   label   #unknown car motor pede
float32 x       #center x
float32 y       #center y
float32 z       #center z
float32 width   #scale x
float32 length  #scale y
float32 height  #scale z
float32 yaw     #rotated by z, radian(not a angle)
float32 vel_x   #m/s
float32 vel_y   #m/s
float32 vel_z   #m/s
================================================================================
MSG: comm_msg/label
#Note: 枚举类别、枚举值、枚举顺序均不可随意改动，不然md5不匹配，消息将无法正常解析

uint8 UNKNOWN                   = 0     #未知（统一类别）
uint8 PEDESTRIAN                = 1     #行人
uint8 CYCLIST                   = 2     #骑自行车的人
uint8 BICYCLE                   = 3     #自行车（两轮车）
uint8 CAR                       = 4     #汽车
uint8 TRUCK                     = 5     #卡车
uint8 TRAM                      = 6     #有轨电车
uint8 TRICYCLE                  = 7     #三轮车
uint8 BUS                       = 8     #公交车

uint8 LOWOBST                   = 10    #低矮障碍
uint8 ROADEDGE                  = 11    #道路边缘
uint8 PIPELINE                  = 12    #水管管线
uint8 CORDON                    = 13    #警戒线
uint8 TREELAWN                  = 14    #街道绿化带
uint8 THICKPIPE                 = 15    #粗水管
uint8 CORD                      = 16    #电线、缆绳
uint8 SLOPE_PAD                 = 17    #斜坡垫
uint8 SPEED_BUMP                = 18    #减速带

uint8 COVER_SOLID               = 20    #实心井盖
uint8 COVER_HOLLOW              = 21    #栅格井盖
uint8 TRAFFIC_CONE              = 22    #雪糕筒（交通锥）
uint8 METAL_BARRIER             = 23    #金属围栏（铁马）
uint8 MEDIAN_BARRIER            = 24    #栅栏（道路中央隔离带）
uint8 CEREMONIAL_POLE           = 25    #礼宾杆
uint8 BOLLARD                   = 26    #路桩

uint8 GARBAGE                   = 40    #垃圾（统称）
uint8 GARBAGE_BOTTLE            = 41    #瓶子
uint8 GARBAGE_PAPER             = 42    #纸巾
uint8 GARBAGE_BRANCH            = 43    #树枝
uint8 GARBAGE_LEAVES            = 44    #树叶
uint8 GARBAGE_BOUGH             = 45    #粗树枝、长树枝
uint8 GARBAGE_CUP               = 46    #奶茶杯
uint8 GARBAGE_CAN               = 47    #易拉罐
uint8 GARBAGE_CARTON            = 48    #纸盒
uint8 GARBAGE_PACKAGE           = 49    #烟盒
uint8 GARBAGE_BRICK             = 70    #砖块（小砖块）
uint8 GARBAGE_CIGARETTEEND      = 71    #烟头


uint8 TRAFFIC_LIGHT             = 50    #交通灯（统称）
uint8 TRAFFIC_LIGHT_OFF         = 51    #交通灯-关闭
uint8 TRAFFIC_LIGHT_RED         = 52    #交通灯-红灯
uint8 TRAFFIC_LIGHT_GREEN       = 53    #交通灯-绿灯
uint8 TRAFFIC_LIGHT_YELLOW      = 54    #交通灯-黄灯

uint8 TRAFFIC_LIGHT_GROUP       = 55    #交通灯-灯组
uint8 TRAFFIC_LIGHT_OFF_LITE    = 56    #交通灯-关闭（小灯）
uint8 TRAFFIC_LIGHT_RED_LITE    = 57    #交通灯-红灯（小灯）
uint8 TRAFFIC_LIGHT_GREEN_LITE  = 58    #交通灯-绿灯（小灯）
uint8 TRAFFIC_LIGHT_YELLOW_LITE = 59    #交通灯-黄灯（小灯）

uint8 CHARGING_GUN              = 60    #充电枪

uint8 PLACEHOLDER               = 101    #占位符(MAX)

uint8 value
")



(provide :comm_msg/boxArray "c75b4d4a4c4616720356b181655ca74f")


