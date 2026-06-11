;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::visionPixel)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'visionPixel (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::VISIONPIXEL")
  (make-package "COMM_MSG::VISIONPIXEL"))

(in-package "ROS")
;;//! \htmlinclude visionPixel.msg.html


(defclass comm_msg::visionPixel
  :super ros::object
  :slots (_label _score _x _y ))

(defmethod comm_msg::visionPixel
  (:init
   (&key
    ((:label __label) (instance comm_msg::label :init))
    ((:score __score) 0.0)
    ((:x __x) 0)
    ((:y __y) 0)
    )
   (send-super :init)
   (setq _label __label)
   (setq _score (float __score))
   (setq _x (round __x))
   (setq _y (round __y))
   self)
  (:label
   (&rest __label)
   (if (keywordp (car __label))
       (send* _label __label)
     (progn
       (if __label (setq _label (car __label)))
       _label)))
  (:score
   (&optional __score)
   (if __score (setq _score __score)) _score)
  (:x
   (&optional __x)
   (if __x (setq _x __x)) _x)
  (:y
   (&optional __y)
   (if __y (setq _y __y)) _y)
  (:serialization-length
   ()
   (+
    ;; comm_msg/label _label
    (send _label :serialization-length)
    ;; float32 _score
    4
    ;; uint32 _x
    4
    ;; uint32 _y
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; comm_msg/label _label
       (send _label :serialize s)
     ;; float32 _score
       (sys::poke _score (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; uint32 _x
       (write-long _x s)
     ;; uint32 _y
       (write-long _y s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; comm_msg/label _label
     (send _label :deserialize buf ptr-) (incf ptr- (send _label :serialization-length))
   ;; float32 _score
     (setq _score (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; uint32 _x
     (setq _x (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; uint32 _y
     (setq _y (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get comm_msg::visionPixel :md5sum-) "668244fa20647074fdbe858cdb96adc5")
(setf (get comm_msg::visionPixel :datatype-) "comm_msg/visionPixel")
(setf (get comm_msg::visionPixel :definition-)
      "label       label
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



(provide :comm_msg/visionPixel "668244fa20647074fdbe858cdb96adc5")


