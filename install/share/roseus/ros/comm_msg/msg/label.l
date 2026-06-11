;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::label)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'label (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::LABEL")
  (make-package "COMM_MSG::LABEL"))

(in-package "ROS")
;;//! \htmlinclude label.msg.html


(intern "*UNKNOWN*" (find-package "COMM_MSG::LABEL"))
(shadow '*UNKNOWN* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*UNKNOWN* 0)
(intern "*PEDESTRIAN*" (find-package "COMM_MSG::LABEL"))
(shadow '*PEDESTRIAN* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*PEDESTRIAN* 1)
(intern "*CYCLIST*" (find-package "COMM_MSG::LABEL"))
(shadow '*CYCLIST* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*CYCLIST* 2)
(intern "*BICYCLE*" (find-package "COMM_MSG::LABEL"))
(shadow '*BICYCLE* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*BICYCLE* 3)
(intern "*CAR*" (find-package "COMM_MSG::LABEL"))
(shadow '*CAR* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*CAR* 4)
(intern "*TRUCK*" (find-package "COMM_MSG::LABEL"))
(shadow '*TRUCK* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*TRUCK* 5)
(intern "*TRAM*" (find-package "COMM_MSG::LABEL"))
(shadow '*TRAM* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*TRAM* 6)
(intern "*TRICYCLE*" (find-package "COMM_MSG::LABEL"))
(shadow '*TRICYCLE* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*TRICYCLE* 7)
(intern "*BUS*" (find-package "COMM_MSG::LABEL"))
(shadow '*BUS* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*BUS* 8)
(intern "*LOWOBST*" (find-package "COMM_MSG::LABEL"))
(shadow '*LOWOBST* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*LOWOBST* 10)
(intern "*ROADEDGE*" (find-package "COMM_MSG::LABEL"))
(shadow '*ROADEDGE* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*ROADEDGE* 11)
(intern "*PIPELINE*" (find-package "COMM_MSG::LABEL"))
(shadow '*PIPELINE* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*PIPELINE* 12)
(intern "*CORDON*" (find-package "COMM_MSG::LABEL"))
(shadow '*CORDON* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*CORDON* 13)
(intern "*TREELAWN*" (find-package "COMM_MSG::LABEL"))
(shadow '*TREELAWN* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*TREELAWN* 14)
(intern "*THICKPIPE*" (find-package "COMM_MSG::LABEL"))
(shadow '*THICKPIPE* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*THICKPIPE* 15)
(intern "*CORD*" (find-package "COMM_MSG::LABEL"))
(shadow '*CORD* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*CORD* 16)
(intern "*SLOPE_PAD*" (find-package "COMM_MSG::LABEL"))
(shadow '*SLOPE_PAD* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*SLOPE_PAD* 17)
(intern "*SPEED_BUMP*" (find-package "COMM_MSG::LABEL"))
(shadow '*SPEED_BUMP* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*SPEED_BUMP* 18)
(intern "*COVER_SOLID*" (find-package "COMM_MSG::LABEL"))
(shadow '*COVER_SOLID* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*COVER_SOLID* 20)
(intern "*COVER_HOLLOW*" (find-package "COMM_MSG::LABEL"))
(shadow '*COVER_HOLLOW* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*COVER_HOLLOW* 21)
(intern "*TRAFFIC_CONE*" (find-package "COMM_MSG::LABEL"))
(shadow '*TRAFFIC_CONE* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*TRAFFIC_CONE* 22)
(intern "*METAL_BARRIER*" (find-package "COMM_MSG::LABEL"))
(shadow '*METAL_BARRIER* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*METAL_BARRIER* 23)
(intern "*MEDIAN_BARRIER*" (find-package "COMM_MSG::LABEL"))
(shadow '*MEDIAN_BARRIER* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*MEDIAN_BARRIER* 24)
(intern "*CEREMONIAL_POLE*" (find-package "COMM_MSG::LABEL"))
(shadow '*CEREMONIAL_POLE* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*CEREMONIAL_POLE* 25)
(intern "*BOLLARD*" (find-package "COMM_MSG::LABEL"))
(shadow '*BOLLARD* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*BOLLARD* 26)
(intern "*GARBAGE*" (find-package "COMM_MSG::LABEL"))
(shadow '*GARBAGE* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*GARBAGE* 40)
(intern "*GARBAGE_BOTTLE*" (find-package "COMM_MSG::LABEL"))
(shadow '*GARBAGE_BOTTLE* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*GARBAGE_BOTTLE* 41)
(intern "*GARBAGE_PAPER*" (find-package "COMM_MSG::LABEL"))
(shadow '*GARBAGE_PAPER* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*GARBAGE_PAPER* 42)
(intern "*GARBAGE_BRANCH*" (find-package "COMM_MSG::LABEL"))
(shadow '*GARBAGE_BRANCH* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*GARBAGE_BRANCH* 43)
(intern "*GARBAGE_LEAVES*" (find-package "COMM_MSG::LABEL"))
(shadow '*GARBAGE_LEAVES* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*GARBAGE_LEAVES* 44)
(intern "*GARBAGE_BOUGH*" (find-package "COMM_MSG::LABEL"))
(shadow '*GARBAGE_BOUGH* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*GARBAGE_BOUGH* 45)
(intern "*GARBAGE_CUP*" (find-package "COMM_MSG::LABEL"))
(shadow '*GARBAGE_CUP* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*GARBAGE_CUP* 46)
(intern "*GARBAGE_CAN*" (find-package "COMM_MSG::LABEL"))
(shadow '*GARBAGE_CAN* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*GARBAGE_CAN* 47)
(intern "*GARBAGE_CARTON*" (find-package "COMM_MSG::LABEL"))
(shadow '*GARBAGE_CARTON* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*GARBAGE_CARTON* 48)
(intern "*GARBAGE_PACKAGE*" (find-package "COMM_MSG::LABEL"))
(shadow '*GARBAGE_PACKAGE* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*GARBAGE_PACKAGE* 49)
(intern "*GARBAGE_BRICK*" (find-package "COMM_MSG::LABEL"))
(shadow '*GARBAGE_BRICK* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*GARBAGE_BRICK* 70)
(intern "*GARBAGE_CIGARETTEEND*" (find-package "COMM_MSG::LABEL"))
(shadow '*GARBAGE_CIGARETTEEND* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*GARBAGE_CIGARETTEEND* 71)
(intern "*TRAFFIC_LIGHT*" (find-package "COMM_MSG::LABEL"))
(shadow '*TRAFFIC_LIGHT* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*TRAFFIC_LIGHT* 50)
(intern "*TRAFFIC_LIGHT_OFF*" (find-package "COMM_MSG::LABEL"))
(shadow '*TRAFFIC_LIGHT_OFF* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*TRAFFIC_LIGHT_OFF* 51)
(intern "*TRAFFIC_LIGHT_RED*" (find-package "COMM_MSG::LABEL"))
(shadow '*TRAFFIC_LIGHT_RED* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*TRAFFIC_LIGHT_RED* 52)
(intern "*TRAFFIC_LIGHT_GREEN*" (find-package "COMM_MSG::LABEL"))
(shadow '*TRAFFIC_LIGHT_GREEN* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*TRAFFIC_LIGHT_GREEN* 53)
(intern "*TRAFFIC_LIGHT_YELLOW*" (find-package "COMM_MSG::LABEL"))
(shadow '*TRAFFIC_LIGHT_YELLOW* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*TRAFFIC_LIGHT_YELLOW* 54)
(intern "*TRAFFIC_LIGHT_GROUP*" (find-package "COMM_MSG::LABEL"))
(shadow '*TRAFFIC_LIGHT_GROUP* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*TRAFFIC_LIGHT_GROUP* 55)
(intern "*TRAFFIC_LIGHT_OFF_LITE*" (find-package "COMM_MSG::LABEL"))
(shadow '*TRAFFIC_LIGHT_OFF_LITE* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*TRAFFIC_LIGHT_OFF_LITE* 56)
(intern "*TRAFFIC_LIGHT_RED_LITE*" (find-package "COMM_MSG::LABEL"))
(shadow '*TRAFFIC_LIGHT_RED_LITE* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*TRAFFIC_LIGHT_RED_LITE* 57)
(intern "*TRAFFIC_LIGHT_GREEN_LITE*" (find-package "COMM_MSG::LABEL"))
(shadow '*TRAFFIC_LIGHT_GREEN_LITE* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*TRAFFIC_LIGHT_GREEN_LITE* 58)
(intern "*TRAFFIC_LIGHT_YELLOW_LITE*" (find-package "COMM_MSG::LABEL"))
(shadow '*TRAFFIC_LIGHT_YELLOW_LITE* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*TRAFFIC_LIGHT_YELLOW_LITE* 59)
(intern "*CHARGING_GUN*" (find-package "COMM_MSG::LABEL"))
(shadow '*CHARGING_GUN* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*CHARGING_GUN* 60)
(intern "*PLACEHOLDER*" (find-package "COMM_MSG::LABEL"))
(shadow '*PLACEHOLDER* (find-package "COMM_MSG::LABEL"))
(defconstant comm_msg::label::*PLACEHOLDER* 101)

(defun comm_msg::label-to-symbol (const)
  (cond
        ((= const 0) 'comm_msg::label::*UNKNOWN*)
        ((= const 1) 'comm_msg::label::*PEDESTRIAN*)
        ((= const 2) 'comm_msg::label::*CYCLIST*)
        ((= const 3) 'comm_msg::label::*BICYCLE*)
        ((= const 4) 'comm_msg::label::*CAR*)
        ((= const 5) 'comm_msg::label::*TRUCK*)
        ((= const 6) 'comm_msg::label::*TRAM*)
        ((= const 7) 'comm_msg::label::*TRICYCLE*)
        ((= const 8) 'comm_msg::label::*BUS*)
        ((= const 10) 'comm_msg::label::*LOWOBST*)
        ((= const 11) 'comm_msg::label::*ROADEDGE*)
        ((= const 12) 'comm_msg::label::*PIPELINE*)
        ((= const 13) 'comm_msg::label::*CORDON*)
        ((= const 14) 'comm_msg::label::*TREELAWN*)
        ((= const 15) 'comm_msg::label::*THICKPIPE*)
        ((= const 16) 'comm_msg::label::*CORD*)
        ((= const 17) 'comm_msg::label::*SLOPE_PAD*)
        ((= const 18) 'comm_msg::label::*SPEED_BUMP*)
        ((= const 20) 'comm_msg::label::*COVER_SOLID*)
        ((= const 21) 'comm_msg::label::*COVER_HOLLOW*)
        ((= const 22) 'comm_msg::label::*TRAFFIC_CONE*)
        ((= const 23) 'comm_msg::label::*METAL_BARRIER*)
        ((= const 24) 'comm_msg::label::*MEDIAN_BARRIER*)
        ((= const 25) 'comm_msg::label::*CEREMONIAL_POLE*)
        ((= const 26) 'comm_msg::label::*BOLLARD*)
        ((= const 40) 'comm_msg::label::*GARBAGE*)
        ((= const 41) 'comm_msg::label::*GARBAGE_BOTTLE*)
        ((= const 42) 'comm_msg::label::*GARBAGE_PAPER*)
        ((= const 43) 'comm_msg::label::*GARBAGE_BRANCH*)
        ((= const 44) 'comm_msg::label::*GARBAGE_LEAVES*)
        ((= const 45) 'comm_msg::label::*GARBAGE_BOUGH*)
        ((= const 46) 'comm_msg::label::*GARBAGE_CUP*)
        ((= const 47) 'comm_msg::label::*GARBAGE_CAN*)
        ((= const 48) 'comm_msg::label::*GARBAGE_CARTON*)
        ((= const 49) 'comm_msg::label::*GARBAGE_PACKAGE*)
        ((= const 70) 'comm_msg::label::*GARBAGE_BRICK*)
        ((= const 71) 'comm_msg::label::*GARBAGE_CIGARETTEEND*)
        ((= const 50) 'comm_msg::label::*TRAFFIC_LIGHT*)
        ((= const 51) 'comm_msg::label::*TRAFFIC_LIGHT_OFF*)
        ((= const 52) 'comm_msg::label::*TRAFFIC_LIGHT_RED*)
        ((= const 53) 'comm_msg::label::*TRAFFIC_LIGHT_GREEN*)
        ((= const 54) 'comm_msg::label::*TRAFFIC_LIGHT_YELLOW*)
        ((= const 55) 'comm_msg::label::*TRAFFIC_LIGHT_GROUP*)
        ((= const 56) 'comm_msg::label::*TRAFFIC_LIGHT_OFF_LITE*)
        ((= const 57) 'comm_msg::label::*TRAFFIC_LIGHT_RED_LITE*)
        ((= const 58) 'comm_msg::label::*TRAFFIC_LIGHT_GREEN_LITE*)
        ((= const 59) 'comm_msg::label::*TRAFFIC_LIGHT_YELLOW_LITE*)
        ((= const 60) 'comm_msg::label::*CHARGING_GUN*)
        ((= const 101) 'comm_msg::label::*PLACEHOLDER*)
        (t nil)))

(defclass comm_msg::label
  :super ros::object
  :slots (_value ))

(defmethod comm_msg::label
  (:init
   (&key
    ((:value __value) 0)
    )
   (send-super :init)
   (setq _value (round __value))
   self)
  (:value
   (&optional __value)
   (if __value (setq _value __value)) _value)
  (:serialization-length
   ()
   (+
    ;; uint8 _value
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint8 _value
       (write-byte _value s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint8 _value
     (setq _value (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;;
   self)
  )

(setf (get comm_msg::label :md5sum-) "c96310482f892c4fff72ca1455e528c2")
(setf (get comm_msg::label :datatype-) "comm_msg/label")
(setf (get comm_msg::label :definition-)
      "#Note: 枚举类别、枚举值、枚举顺序均不可随意改动，不然md5不匹配，消息将无法正常解析

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



(provide :comm_msg/label "c96310482f892c4fff72ca1455e528c2")


