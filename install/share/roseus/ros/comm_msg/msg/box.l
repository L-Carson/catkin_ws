;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::box)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'box (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::BOX")
  (make-package "COMM_MSG::BOX"))

(in-package "ROS")
;;//! \htmlinclude box.msg.html


(defclass comm_msg::box
  :super ros::object
  :slots (_label _x _y _z _width _length _height _yaw _vel_x _vel_y _vel_z ))

(defmethod comm_msg::box
  (:init
   (&key
    ((:label __label) (instance comm_msg::label :init))
    ((:x __x) 0.0)
    ((:y __y) 0.0)
    ((:z __z) 0.0)
    ((:width __width) 0.0)
    ((:length __length) 0.0)
    ((:height __height) 0.0)
    ((:yaw __yaw) 0.0)
    ((:vel_x __vel_x) 0.0)
    ((:vel_y __vel_y) 0.0)
    ((:vel_z __vel_z) 0.0)
    )
   (send-super :init)
   (setq _label __label)
   (setq _x (float __x))
   (setq _y (float __y))
   (setq _z (float __z))
   (setq _width (float __width))
   (setq _length (float __length))
   (setq _height (float __height))
   (setq _yaw (float __yaw))
   (setq _vel_x (float __vel_x))
   (setq _vel_y (float __vel_y))
   (setq _vel_z (float __vel_z))
   self)
  (:label
   (&rest __label)
   (if (keywordp (car __label))
       (send* _label __label)
     (progn
       (if __label (setq _label (car __label)))
       _label)))
  (:x
   (&optional __x)
   (if __x (setq _x __x)) _x)
  (:y
   (&optional __y)
   (if __y (setq _y __y)) _y)
  (:z
   (&optional __z)
   (if __z (setq _z __z)) _z)
  (:width
   (&optional __width)
   (if __width (setq _width __width)) _width)
  (:length
   (&optional __length)
   (if __length (setq _length __length)) _length)
  (:height
   (&optional __height)
   (if __height (setq _height __height)) _height)
  (:yaw
   (&optional __yaw)
   (if __yaw (setq _yaw __yaw)) _yaw)
  (:vel_x
   (&optional __vel_x)
   (if __vel_x (setq _vel_x __vel_x)) _vel_x)
  (:vel_y
   (&optional __vel_y)
   (if __vel_y (setq _vel_y __vel_y)) _vel_y)
  (:vel_z
   (&optional __vel_z)
   (if __vel_z (setq _vel_z __vel_z)) _vel_z)
  (:serialization-length
   ()
   (+
    ;; comm_msg/label _label
    (send _label :serialization-length)
    ;; float32 _x
    4
    ;; float32 _y
    4
    ;; float32 _z
    4
    ;; float32 _width
    4
    ;; float32 _length
    4
    ;; float32 _height
    4
    ;; float32 _yaw
    4
    ;; float32 _vel_x
    4
    ;; float32 _vel_y
    4
    ;; float32 _vel_z
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; comm_msg/label _label
       (send _label :serialize s)
     ;; float32 _x
       (sys::poke _x (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _y
       (sys::poke _y (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _z
       (sys::poke _z (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _width
       (sys::poke _width (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _length
       (sys::poke _length (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _height
       (sys::poke _height (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _yaw
       (sys::poke _yaw (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _vel_x
       (sys::poke _vel_x (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _vel_y
       (sys::poke _vel_y (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _vel_z
       (sys::poke _vel_z (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; comm_msg/label _label
     (send _label :deserialize buf ptr-) (incf ptr- (send _label :serialization-length))
   ;; float32 _x
     (setq _x (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _y
     (setq _y (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _z
     (setq _z (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _width
     (setq _width (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _length
     (setq _length (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _height
     (setq _height (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _yaw
     (setq _yaw (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _vel_x
     (setq _vel_x (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _vel_y
     (setq _vel_y (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _vel_z
     (setq _vel_z (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get comm_msg::box :md5sum-) "010e5d8fbb7da500e9e709e85fe17d1f")
(setf (get comm_msg::box :datatype-) "comm_msg/box")
(setf (get comm_msg::box :definition-)
      "label   label   #unknown car motor pede
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



(provide :comm_msg/box "010e5d8fbb7da500e9e709e85fe17d1f")


