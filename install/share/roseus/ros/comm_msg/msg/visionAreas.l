;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::visionAreas)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'visionAreas (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::VISIONAREAS")
  (make-package "COMM_MSG::VISIONAREAS"))

(in-package "ROS")
;;//! \htmlinclude visionAreas.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass comm_msg::visionAreas
  :super ros::object
  :slots (_header _cameraId _pipeline _areas ))

(defmethod comm_msg::visionAreas
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:cameraId __cameraId) "")
    ((:pipeline __pipeline) (instance comm_msg::label_state :init))
    ((:areas __areas) ())
    )
   (send-super :init)
   (setq _header __header)
   (setq _cameraId (string __cameraId))
   (setq _pipeline __pipeline)
   (setq _areas __areas)
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:cameraId
   (&optional __cameraId)
   (if __cameraId (setq _cameraId __cameraId)) _cameraId)
  (:pipeline
   (&rest __pipeline)
   (if (keywordp (car __pipeline))
       (send* _pipeline __pipeline)
     (progn
       (if __pipeline (setq _pipeline (car __pipeline)))
       _pipeline)))
  (:areas
   (&rest __areas)
   (if (keywordp (car __areas))
       (send* _areas __areas)
     (progn
       (if __areas (setq _areas (car __areas)))
       _areas)))
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; string _cameraId
    4 (length _cameraId)
    ;; comm_msg/label_state _pipeline
    (send _pipeline :serialization-length)
    ;; comm_msg/visionArea[] _areas
    (apply #'+ (send-all _areas :serialization-length)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; string _cameraId
       (write-long (length _cameraId) s) (princ _cameraId s)
     ;; comm_msg/label_state _pipeline
       (send _pipeline :serialize s)
     ;; comm_msg/visionArea[] _areas
     (write-long (length _areas) s)
     (dolist (elem _areas)
       (send elem :serialize s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; string _cameraId
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _cameraId (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; comm_msg/label_state _pipeline
     (send _pipeline :deserialize buf ptr-) (incf ptr- (send _pipeline :serialization-length))
   ;; comm_msg/visionArea[] _areas
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _areas (let (r) (dotimes (i n) (push (instance comm_msg::visionArea :init) r)) r))
     (dolist (elem- _areas)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;;
   self)
  )

(setf (get comm_msg::visionAreas :md5sum-) "f694b576a47ff2e3b81db5949a0a3f7f")
(setf (get comm_msg::visionAreas :datatype-) "comm_msg/visionAreas")
(setf (get comm_msg::visionAreas :definition-)
      "std_msgs/Header     header
string              cameraId
label_state         pipeline    # 水管电线存在状态
visionArea[]        areas
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
MSG: comm_msg/label_state
#Note: 标签类别存在状态

uint8 NO  = 0
uint8 YES = 1
uint8 MAY = 2

uint8 value
================================================================================
MSG: comm_msg/visionArea
visionPixel[]   pixels
================================================================================
MSG: comm_msg/visionPixel
label       label
float32     score
uint32      x
uint32      y
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



(provide :comm_msg/visionAreas "f694b576a47ff2e3b81db5949a0a3f7f")


