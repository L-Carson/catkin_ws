;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::visionFunc)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'visionFunc (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::VISIONFUNC")
  (make-package "COMM_MSG::VISIONFUNC"))

(in-package "ROS")
;;//! \htmlinclude visionFunc.msg.html


(intern "*NULL_FUNC*" (find-package "COMM_MSG::VISIONFUNC"))
(shadow '*NULL_FUNC* (find-package "COMM_MSG::VISIONFUNC"))
(defconstant comm_msg::visionFunc::*NULL_FUNC* 0)
(intern "*QR_CODE_DET*" (find-package "COMM_MSG::VISIONFUNC"))
(shadow '*QR_CODE_DET* (find-package "COMM_MSG::VISIONFUNC"))
(defconstant comm_msg::visionFunc::*QR_CODE_DET* 1)
(intern "*NET_DET*" (find-package "COMM_MSG::VISIONFUNC"))
(shadow '*NET_DET* (find-package "COMM_MSG::VISIONFUNC"))
(defconstant comm_msg::visionFunc::*NET_DET* 2)
(intern "*NET_SEG*" (find-package "COMM_MSG::VISIONFUNC"))
(shadow '*NET_SEG* (find-package "COMM_MSG::VISIONFUNC"))
(defconstant comm_msg::visionFunc::*NET_SEG* 3)

(defun comm_msg::visionFunc-to-symbol (const)
  (cond
        ((= const 0) 'comm_msg::visionFunc::*NULL_FUNC*)
        ((= const 1) 'comm_msg::visionFunc::*QR_CODE_DET*)
        ((= const 2) 'comm_msg::visionFunc::*NET_DET*)
        ((= const 3) 'comm_msg::visionFunc::*NET_SEG*)
        (t nil)))

(defclass comm_msg::visionFunc
  :super ros::object
  :slots (_funcId _ipu _hz _cameras _usingLabel ))

(defmethod comm_msg::visionFunc
  (:init
   (&key
    ((:funcId __funcId) 0)
    ((:ipu __ipu) nil)
    ((:hz __hz) 0)
    ((:cameras __cameras) (let (r) (dotimes (i 0) (push "" r)) r))
    ((:usingLabel __usingLabel) ())
    )
   (send-super :init)
   (setq _funcId (round __funcId))
   (setq _ipu __ipu)
   (setq _hz (round __hz))
   (setq _cameras __cameras)
   (setq _usingLabel __usingLabel)
   self)
  (:funcId
   (&optional __funcId)
   (if __funcId (setq _funcId __funcId)) _funcId)
  (:ipu
   (&optional (__ipu :null))
   (if (not (eq __ipu :null)) (setq _ipu __ipu)) _ipu)
  (:hz
   (&optional __hz)
   (if __hz (setq _hz __hz)) _hz)
  (:cameras
   (&optional __cameras)
   (if __cameras (setq _cameras __cameras)) _cameras)
  (:usingLabel
   (&rest __usingLabel)
   (if (keywordp (car __usingLabel))
       (send* _usingLabel __usingLabel)
     (progn
       (if __usingLabel (setq _usingLabel (car __usingLabel)))
       _usingLabel)))
  (:serialization-length
   ()
   (+
    ;; uint8 _funcId
    1
    ;; bool _ipu
    1
    ;; uint8 _hz
    1
    ;; string[] _cameras
    (apply #'+ (mapcar #'(lambda (x) (+ 4 (length x))) _cameras)) 4
    ;; comm_msg/label[] _usingLabel
    (apply #'+ (send-all _usingLabel :serialization-length)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint8 _funcId
       (write-byte _funcId s)
     ;; bool _ipu
       (if _ipu (write-byte -1 s) (write-byte 0 s))
     ;; uint8 _hz
       (write-byte _hz s)
     ;; string[] _cameras
     (write-long (length _cameras) s)
     (dolist (elem _cameras)
       (write-long (length elem) s) (princ elem s)
       )
     ;; comm_msg/label[] _usingLabel
     (write-long (length _usingLabel) s)
     (dolist (elem _usingLabel)
       (send elem :serialize s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint8 _funcId
     (setq _funcId (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; bool _ipu
     (setq _ipu (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;; uint8 _hz
     (setq _hz (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; string[] _cameras
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _cameras (make-list n))
     (dotimes (i n)
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setf (elt _cameras i) (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
     ))
   ;; comm_msg/label[] _usingLabel
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _usingLabel (let (r) (dotimes (i n) (push (instance comm_msg::label :init) r)) r))
     (dolist (elem- _usingLabel)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;;
   self)
  )

(setf (get comm_msg::visionFunc :md5sum-) "79b818443e89e9f0f5bd6b426111152a")
(setf (get comm_msg::visionFunc :datatype-) "comm_msg/visionFunc")
(setf (get comm_msg::visionFunc :definition-)
      "# 功能ID
uint8 NULL_FUNC   = 0   # NULL
uint8 QR_CODE_DET = 1   # 二维码检测
uint8 NET_DET     = 2   # 网络检测
uint8 NET_SEG     = 3   # 网络分割

# msg
uint8       funcId      # 功能ID
bool        ipu         # 是否使用图像处理单元
uint8       hz          # 执行频率
string[]    cameras     # 相机ID：front_left(前左), front_mid(前中), front_right(前右), rear_mid(后中) etc.
label[]     usingLabel  # 当前正在使用的类别标签
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



(provide :comm_msg/visionFunc "79b818443e89e9f0f5bd6b426111152a")


