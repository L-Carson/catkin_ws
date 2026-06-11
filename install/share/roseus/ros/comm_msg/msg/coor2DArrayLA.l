;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::coor2DArrayLA)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'coor2DArrayLA (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::COOR2DARRAYLA")
  (make-package "COMM_MSG::COOR2DARRAYLA"))

(in-package "ROS")
;;//! \htmlinclude coor2DArrayLA.msg.html


(defclass comm_msg::coor2DArrayLA
  :super ros::object
  :slots (_hasRisk _coor2DArrayLA ))

(defmethod comm_msg::coor2DArrayLA
  (:init
   (&key
    ((:hasRisk __hasRisk) nil)
    ((:coor2DArrayLA __coor2DArrayLA) ())
    )
   (send-super :init)
   (setq _hasRisk __hasRisk)
   (setq _coor2DArrayLA __coor2DArrayLA)
   self)
  (:hasRisk
   (&optional (__hasRisk :null))
   (if (not (eq __hasRisk :null)) (setq _hasRisk __hasRisk)) _hasRisk)
  (:coor2DArrayLA
   (&rest __coor2DArrayLA)
   (if (keywordp (car __coor2DArrayLA))
       (send* _coor2DArrayLA __coor2DArrayLA)
     (progn
       (if __coor2DArrayLA (setq _coor2DArrayLA (car __coor2DArrayLA)))
       _coor2DArrayLA)))
  (:serialization-length
   ()
   (+
    ;; bool _hasRisk
    1
    ;; comm_msg/coor2DArrayL[] _coor2DArrayLA
    (apply #'+ (send-all _coor2DArrayLA :serialization-length)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; bool _hasRisk
       (if _hasRisk (write-byte -1 s) (write-byte 0 s))
     ;; comm_msg/coor2DArrayL[] _coor2DArrayLA
     (write-long (length _coor2DArrayLA) s)
     (dolist (elem _coor2DArrayLA)
       (send elem :serialize s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; bool _hasRisk
     (setq _hasRisk (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;; comm_msg/coor2DArrayL[] _coor2DArrayLA
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _coor2DArrayLA (let (r) (dotimes (i n) (push (instance comm_msg::coor2DArrayL :init) r)) r))
     (dolist (elem- _coor2DArrayLA)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;;
   self)
  )

(setf (get comm_msg::coor2DArrayLA :md5sum-) "e90f0a07eab5e228d174cb357ff43632")
(setf (get comm_msg::coor2DArrayLA :datatype-) "comm_msg/coor2DArrayLA")
(setf (get comm_msg::coor2DArrayLA :definition-)
      "bool              hasRisk       #图像视野有无风险，如有风险，需要规控减速
coor2DArrayL[]    coor2DArrayLA
================================================================================
MSG: comm_msg/coor2DArrayL
Header      header
label       label
uint8       id
coor2D[]    coor2DArray
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
================================================================================
MSG: comm_msg/coor2D
coorLabel   label       #坐标类别
float32     pos_x
float32     pos_y
================================================================================
MSG: comm_msg/coorLabel
uint8   UNKNOWN      = 0  #未知
uint8   STATIC       = 1  #静态
uint8   DYNAMIC      = 2  #动态
uint8   HOLLOW_OBST  = 3  #空洞障碍
uint8   SHORT_HOLD   = 4  #低矮维持
uint8   COMMON_HOLD  = 5  #常规维持
uint8   BLIND_SPOT   = 6  #盲区障碍
uint8   SLOPE_PAD    = 7  #斜坡垫
uint8   CONTACT_EDGE = 8  #触边障碍
uint8   TREELAWN     = 9  #绿植障碍
uint8   ULTRASONIC   = 10 #超声

uint8 value
")



(provide :comm_msg/coor2DArrayLA "e90f0a07eab5e228d174cb357ff43632")


