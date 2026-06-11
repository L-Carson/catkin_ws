;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::visionRect)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'visionRect (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::VISIONRECT")
  (make-package "COMM_MSG::VISIONRECT"))

(in-package "ROS")
;;//! \htmlinclude visionRect.msg.html


(defclass comm_msg::visionRect
  :super ros::object
  :slots (_label _score _xmin _ymin _xmax _ymax _feature ))

(defmethod comm_msg::visionRect
  (:init
   (&key
    ((:label __label) (instance comm_msg::label :init))
    ((:score __score) 0.0)
    ((:xmin __xmin) 0)
    ((:ymin __ymin) 0)
    ((:xmax __xmax) 0)
    ((:ymax __ymax) 0)
    ((:feature __feature) (make-array 0 :initial-element 0.0 :element-type :float))
    )
   (send-super :init)
   (setq _label __label)
   (setq _score (float __score))
   (setq _xmin (round __xmin))
   (setq _ymin (round __ymin))
   (setq _xmax (round __xmax))
   (setq _ymax (round __ymax))
   (setq _feature __feature)
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
  (:xmin
   (&optional __xmin)
   (if __xmin (setq _xmin __xmin)) _xmin)
  (:ymin
   (&optional __ymin)
   (if __ymin (setq _ymin __ymin)) _ymin)
  (:xmax
   (&optional __xmax)
   (if __xmax (setq _xmax __xmax)) _xmax)
  (:ymax
   (&optional __ymax)
   (if __ymax (setq _ymax __ymax)) _ymax)
  (:feature
   (&optional __feature)
   (if __feature (setq _feature __feature)) _feature)
  (:serialization-length
   ()
   (+
    ;; comm_msg/label _label
    (send _label :serialization-length)
    ;; float32 _score
    4
    ;; uint32 _xmin
    4
    ;; uint32 _ymin
    4
    ;; uint32 _xmax
    4
    ;; uint32 _ymax
    4
    ;; float32[] _feature
    (* 4    (length _feature)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; comm_msg/label _label
       (send _label :serialize s)
     ;; float32 _score
       (sys::poke _score (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; uint32 _xmin
       (write-long _xmin s)
     ;; uint32 _ymin
       (write-long _ymin s)
     ;; uint32 _xmax
       (write-long _xmax s)
     ;; uint32 _ymax
       (write-long _ymax s)
     ;; float32[] _feature
     (write-long (length _feature) s)
     (dotimes (i (length _feature))
       (sys::poke (elt _feature i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; comm_msg/label _label
     (send _label :deserialize buf ptr-) (incf ptr- (send _label :serialization-length))
   ;; float32 _score
     (setq _score (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; uint32 _xmin
     (setq _xmin (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; uint32 _ymin
     (setq _ymin (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; uint32 _xmax
     (setq _xmax (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; uint32 _ymax
     (setq _ymax (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; float32[] _feature
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _feature (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _feature i) (sys::peek buf ptr- :float)) (incf ptr- 4)
     ))
   ;;
   self)
  )

(setf (get comm_msg::visionRect :md5sum-) "b747118cb5066a7266881512a4bb9821")
(setf (get comm_msg::visionRect :datatype-) "comm_msg/visionRect")
(setf (get comm_msg::visionRect :definition-)
      "label       label
float32     score
uint32      xmin
uint32      ymin
uint32      xmax
uint32      ymax
float32[]   feature
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



(provide :comm_msg/visionRect "b747118cb5066a7266881512a4bb9821")


