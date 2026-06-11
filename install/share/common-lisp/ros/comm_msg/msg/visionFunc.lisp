; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude visionFunc.msg.html

(cl:defclass <visionFunc> (roslisp-msg-protocol:ros-message)
  ((funcId
    :reader funcId
    :initarg :funcId
    :type cl:fixnum
    :initform 0)
   (ipu
    :reader ipu
    :initarg :ipu
    :type cl:boolean
    :initform cl:nil)
   (hz
    :reader hz
    :initarg :hz
    :type cl:fixnum
    :initform 0)
   (cameras
    :reader cameras
    :initarg :cameras
    :type (cl:vector cl:string)
   :initform (cl:make-array 0 :element-type 'cl:string :initial-element ""))
   (usingLabel
    :reader usingLabel
    :initarg :usingLabel
    :type (cl:vector comm_msg-msg:label)
   :initform (cl:make-array 0 :element-type 'comm_msg-msg:label :initial-element (cl:make-instance 'comm_msg-msg:label))))
)

(cl:defclass visionFunc (<visionFunc>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <visionFunc>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'visionFunc)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<visionFunc> is deprecated: use comm_msg-msg:visionFunc instead.")))

(cl:ensure-generic-function 'funcId-val :lambda-list '(m))
(cl:defmethod funcId-val ((m <visionFunc>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:funcId-val is deprecated.  Use comm_msg-msg:funcId instead.")
  (funcId m))

(cl:ensure-generic-function 'ipu-val :lambda-list '(m))
(cl:defmethod ipu-val ((m <visionFunc>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:ipu-val is deprecated.  Use comm_msg-msg:ipu instead.")
  (ipu m))

(cl:ensure-generic-function 'hz-val :lambda-list '(m))
(cl:defmethod hz-val ((m <visionFunc>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:hz-val is deprecated.  Use comm_msg-msg:hz instead.")
  (hz m))

(cl:ensure-generic-function 'cameras-val :lambda-list '(m))
(cl:defmethod cameras-val ((m <visionFunc>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:cameras-val is deprecated.  Use comm_msg-msg:cameras instead.")
  (cameras m))

(cl:ensure-generic-function 'usingLabel-val :lambda-list '(m))
(cl:defmethod usingLabel-val ((m <visionFunc>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:usingLabel-val is deprecated.  Use comm_msg-msg:usingLabel instead.")
  (usingLabel m))
(cl:defmethod roslisp-msg-protocol:symbol-codes ((msg-type (cl:eql '<visionFunc>)))
    "Constants for message type '<visionFunc>"
  '((:NULL_FUNC . 0)
    (:QR_CODE_DET . 1)
    (:NET_DET . 2)
    (:NET_SEG . 3))
)
(cl:defmethod roslisp-msg-protocol:symbol-codes ((msg-type (cl:eql 'visionFunc)))
    "Constants for message type 'visionFunc"
  '((:NULL_FUNC . 0)
    (:QR_CODE_DET . 1)
    (:NET_DET . 2)
    (:NET_SEG . 3))
)
(cl:defmethod roslisp-msg-protocol:serialize ((msg <visionFunc>) ostream)
  "Serializes a message object of type '<visionFunc>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'funcId)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'ipu) 1 0)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'hz)) ostream)
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'cameras))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let ((__ros_str_len (cl:length ele)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) ele))
   (cl:slot-value msg 'cameras))
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'usingLabel))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'usingLabel))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <visionFunc>) istream)
  "Deserializes a message object of type '<visionFunc>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'funcId)) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'ipu) (cl:not (cl:zerop (cl:read-byte istream))))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'hz)) (cl:read-byte istream))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'cameras) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'cameras)))
    (cl:dotimes (i __ros_arr_len)
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:aref vals i) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:aref vals i) __ros_str_idx) (cl:code-char (cl:read-byte istream))))))))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'usingLabel) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'usingLabel)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'comm_msg-msg:label))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<visionFunc>)))
  "Returns string type for a message object of type '<visionFunc>"
  "comm_msg/visionFunc")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'visionFunc)))
  "Returns string type for a message object of type 'visionFunc"
  "comm_msg/visionFunc")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<visionFunc>)))
  "Returns md5sum for a message object of type '<visionFunc>"
  "79b818443e89e9f0f5bd6b426111152a")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'visionFunc)))
  "Returns md5sum for a message object of type 'visionFunc"
  "79b818443e89e9f0f5bd6b426111152a")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<visionFunc>)))
  "Returns full string definition for message of type '<visionFunc>"
  (cl:format cl:nil "# 功能ID~%uint8 NULL_FUNC   = 0   # NULL~%uint8 QR_CODE_DET = 1   # 二维码检测~%uint8 NET_DET     = 2   # 网络检测~%uint8 NET_SEG     = 3   # 网络分割~%~%# msg~%uint8       funcId      # 功能ID~%bool        ipu         # 是否使用图像处理单元~%uint8       hz          # 执行频率~%string[]    cameras     # 相机ID：front_left(前左), front_mid(前中), front_right(前右), rear_mid(后中) etc.~%label[]     usingLabel  # 当前正在使用的类别标签~%================================================================================~%MSG: comm_msg/label~%#Note: 枚举类别、枚举值、枚举顺序均不可随意改动，不然md5不匹配，消息将无法正常解析~%~%uint8 UNKNOWN                   = 0     #未知（统一类别）~%uint8 PEDESTRIAN                = 1     #行人~%uint8 CYCLIST                   = 2     #骑自行车的人~%uint8 BICYCLE                   = 3     #自行车（两轮车）~%uint8 CAR                       = 4     #汽车~%uint8 TRUCK                     = 5     #卡车~%uint8 TRAM                      = 6     #有轨电车~%uint8 TRICYCLE                  = 7     #三轮车~%uint8 BUS                       = 8     #公交车~%~%uint8 LOWOBST                   = 10    #低矮障碍~%uint8 ROADEDGE                  = 11    #道路边缘~%uint8 PIPELINE                  = 12    #水管管线~%uint8 CORDON                    = 13    #警戒线~%uint8 TREELAWN                  = 14    #街道绿化带~%uint8 THICKPIPE                 = 15    #粗水管~%uint8 CORD                      = 16    #电线、缆绳~%uint8 SLOPE_PAD                 = 17    #斜坡垫~%uint8 SPEED_BUMP                = 18    #减速带~%~%uint8 COVER_SOLID               = 20    #实心井盖~%uint8 COVER_HOLLOW              = 21    #栅格井盖~%uint8 TRAFFIC_CONE              = 22    #雪糕筒（交通锥）~%uint8 METAL_BARRIER             = 23    #金属围栏（铁马）~%uint8 MEDIAN_BARRIER            = 24    #栅栏（道路中央隔离带）~%uint8 CEREMONIAL_POLE           = 25    #礼宾杆~%uint8 BOLLARD                   = 26    #路桩~%~%uint8 GARBAGE                   = 40    #垃圾（统称）~%uint8 GARBAGE_BOTTLE            = 41    #瓶子~%uint8 GARBAGE_PAPER             = 42    #纸巾~%uint8 GARBAGE_BRANCH            = 43    #树枝~%uint8 GARBAGE_LEAVES            = 44    #树叶~%uint8 GARBAGE_BOUGH             = 45    #粗树枝、长树枝~%uint8 GARBAGE_CUP               = 46    #奶茶杯~%uint8 GARBAGE_CAN               = 47    #易拉罐~%uint8 GARBAGE_CARTON            = 48    #纸盒~%uint8 GARBAGE_PACKAGE           = 49    #烟盒~%uint8 GARBAGE_BRICK             = 70    #砖块（小砖块）~%uint8 GARBAGE_CIGARETTEEND      = 71    #烟头~%~%~%uint8 TRAFFIC_LIGHT             = 50    #交通灯（统称）~%uint8 TRAFFIC_LIGHT_OFF         = 51    #交通灯-关闭~%uint8 TRAFFIC_LIGHT_RED         = 52    #交通灯-红灯~%uint8 TRAFFIC_LIGHT_GREEN       = 53    #交通灯-绿灯~%uint8 TRAFFIC_LIGHT_YELLOW      = 54    #交通灯-黄灯~%~%uint8 TRAFFIC_LIGHT_GROUP       = 55    #交通灯-灯组~%uint8 TRAFFIC_LIGHT_OFF_LITE    = 56    #交通灯-关闭（小灯）~%uint8 TRAFFIC_LIGHT_RED_LITE    = 57    #交通灯-红灯（小灯）~%uint8 TRAFFIC_LIGHT_GREEN_LITE  = 58    #交通灯-绿灯（小灯）~%uint8 TRAFFIC_LIGHT_YELLOW_LITE = 59    #交通灯-黄灯（小灯）~%~%uint8 CHARGING_GUN              = 60    #充电枪~%~%uint8 PLACEHOLDER               = 101    #占位符(MAX)~%~%uint8 value~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'visionFunc)))
  "Returns full string definition for message of type 'visionFunc"
  (cl:format cl:nil "# 功能ID~%uint8 NULL_FUNC   = 0   # NULL~%uint8 QR_CODE_DET = 1   # 二维码检测~%uint8 NET_DET     = 2   # 网络检测~%uint8 NET_SEG     = 3   # 网络分割~%~%# msg~%uint8       funcId      # 功能ID~%bool        ipu         # 是否使用图像处理单元~%uint8       hz          # 执行频率~%string[]    cameras     # 相机ID：front_left(前左), front_mid(前中), front_right(前右), rear_mid(后中) etc.~%label[]     usingLabel  # 当前正在使用的类别标签~%================================================================================~%MSG: comm_msg/label~%#Note: 枚举类别、枚举值、枚举顺序均不可随意改动，不然md5不匹配，消息将无法正常解析~%~%uint8 UNKNOWN                   = 0     #未知（统一类别）~%uint8 PEDESTRIAN                = 1     #行人~%uint8 CYCLIST                   = 2     #骑自行车的人~%uint8 BICYCLE                   = 3     #自行车（两轮车）~%uint8 CAR                       = 4     #汽车~%uint8 TRUCK                     = 5     #卡车~%uint8 TRAM                      = 6     #有轨电车~%uint8 TRICYCLE                  = 7     #三轮车~%uint8 BUS                       = 8     #公交车~%~%uint8 LOWOBST                   = 10    #低矮障碍~%uint8 ROADEDGE                  = 11    #道路边缘~%uint8 PIPELINE                  = 12    #水管管线~%uint8 CORDON                    = 13    #警戒线~%uint8 TREELAWN                  = 14    #街道绿化带~%uint8 THICKPIPE                 = 15    #粗水管~%uint8 CORD                      = 16    #电线、缆绳~%uint8 SLOPE_PAD                 = 17    #斜坡垫~%uint8 SPEED_BUMP                = 18    #减速带~%~%uint8 COVER_SOLID               = 20    #实心井盖~%uint8 COVER_HOLLOW              = 21    #栅格井盖~%uint8 TRAFFIC_CONE              = 22    #雪糕筒（交通锥）~%uint8 METAL_BARRIER             = 23    #金属围栏（铁马）~%uint8 MEDIAN_BARRIER            = 24    #栅栏（道路中央隔离带）~%uint8 CEREMONIAL_POLE           = 25    #礼宾杆~%uint8 BOLLARD                   = 26    #路桩~%~%uint8 GARBAGE                   = 40    #垃圾（统称）~%uint8 GARBAGE_BOTTLE            = 41    #瓶子~%uint8 GARBAGE_PAPER             = 42    #纸巾~%uint8 GARBAGE_BRANCH            = 43    #树枝~%uint8 GARBAGE_LEAVES            = 44    #树叶~%uint8 GARBAGE_BOUGH             = 45    #粗树枝、长树枝~%uint8 GARBAGE_CUP               = 46    #奶茶杯~%uint8 GARBAGE_CAN               = 47    #易拉罐~%uint8 GARBAGE_CARTON            = 48    #纸盒~%uint8 GARBAGE_PACKAGE           = 49    #烟盒~%uint8 GARBAGE_BRICK             = 70    #砖块（小砖块）~%uint8 GARBAGE_CIGARETTEEND      = 71    #烟头~%~%~%uint8 TRAFFIC_LIGHT             = 50    #交通灯（统称）~%uint8 TRAFFIC_LIGHT_OFF         = 51    #交通灯-关闭~%uint8 TRAFFIC_LIGHT_RED         = 52    #交通灯-红灯~%uint8 TRAFFIC_LIGHT_GREEN       = 53    #交通灯-绿灯~%uint8 TRAFFIC_LIGHT_YELLOW      = 54    #交通灯-黄灯~%~%uint8 TRAFFIC_LIGHT_GROUP       = 55    #交通灯-灯组~%uint8 TRAFFIC_LIGHT_OFF_LITE    = 56    #交通灯-关闭（小灯）~%uint8 TRAFFIC_LIGHT_RED_LITE    = 57    #交通灯-红灯（小灯）~%uint8 TRAFFIC_LIGHT_GREEN_LITE  = 58    #交通灯-绿灯（小灯）~%uint8 TRAFFIC_LIGHT_YELLOW_LITE = 59    #交通灯-黄灯（小灯）~%~%uint8 CHARGING_GUN              = 60    #充电枪~%~%uint8 PLACEHOLDER               = 101    #占位符(MAX)~%~%uint8 value~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <visionFunc>))
  (cl:+ 0
     1
     1
     1
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'cameras) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4 (cl:length ele))))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'usingLabel) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <visionFunc>))
  "Converts a ROS message object to a list"
  (cl:list 'visionFunc
    (cl:cons ':funcId (funcId msg))
    (cl:cons ':ipu (ipu msg))
    (cl:cons ':hz (hz msg))
    (cl:cons ':cameras (cameras msg))
    (cl:cons ':usingLabel (usingLabel msg))
))
