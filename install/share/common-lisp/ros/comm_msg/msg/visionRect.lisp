; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude visionRect.msg.html

(cl:defclass <visionRect> (roslisp-msg-protocol:ros-message)
  ((label
    :reader label
    :initarg :label
    :type comm_msg-msg:label
    :initform (cl:make-instance 'comm_msg-msg:label))
   (score
    :reader score
    :initarg :score
    :type cl:float
    :initform 0.0)
   (xmin
    :reader xmin
    :initarg :xmin
    :type cl:integer
    :initform 0)
   (ymin
    :reader ymin
    :initarg :ymin
    :type cl:integer
    :initform 0)
   (xmax
    :reader xmax
    :initarg :xmax
    :type cl:integer
    :initform 0)
   (ymax
    :reader ymax
    :initarg :ymax
    :type cl:integer
    :initform 0)
   (feature
    :reader feature
    :initarg :feature
    :type (cl:vector cl:float)
   :initform (cl:make-array 0 :element-type 'cl:float :initial-element 0.0)))
)

(cl:defclass visionRect (<visionRect>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <visionRect>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'visionRect)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<visionRect> is deprecated: use comm_msg-msg:visionRect instead.")))

(cl:ensure-generic-function 'label-val :lambda-list '(m))
(cl:defmethod label-val ((m <visionRect>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:label-val is deprecated.  Use comm_msg-msg:label instead.")
  (label m))

(cl:ensure-generic-function 'score-val :lambda-list '(m))
(cl:defmethod score-val ((m <visionRect>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:score-val is deprecated.  Use comm_msg-msg:score instead.")
  (score m))

(cl:ensure-generic-function 'xmin-val :lambda-list '(m))
(cl:defmethod xmin-val ((m <visionRect>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:xmin-val is deprecated.  Use comm_msg-msg:xmin instead.")
  (xmin m))

(cl:ensure-generic-function 'ymin-val :lambda-list '(m))
(cl:defmethod ymin-val ((m <visionRect>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:ymin-val is deprecated.  Use comm_msg-msg:ymin instead.")
  (ymin m))

(cl:ensure-generic-function 'xmax-val :lambda-list '(m))
(cl:defmethod xmax-val ((m <visionRect>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:xmax-val is deprecated.  Use comm_msg-msg:xmax instead.")
  (xmax m))

(cl:ensure-generic-function 'ymax-val :lambda-list '(m))
(cl:defmethod ymax-val ((m <visionRect>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:ymax-val is deprecated.  Use comm_msg-msg:ymax instead.")
  (ymax m))

(cl:ensure-generic-function 'feature-val :lambda-list '(m))
(cl:defmethod feature-val ((m <visionRect>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:feature-val is deprecated.  Use comm_msg-msg:feature instead.")
  (feature m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <visionRect>) ostream)
  "Serializes a message object of type '<visionRect>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'label) ostream)
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'score))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'xmin)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'xmin)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'xmin)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'xmin)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'ymin)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'ymin)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'ymin)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'ymin)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'xmax)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'xmax)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'xmax)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'xmax)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'ymax)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'ymax)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'ymax)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'ymax)) ostream)
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'feature))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let ((bits (roslisp-utils:encode-single-float-bits ele)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)))
   (cl:slot-value msg 'feature))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <visionRect>) istream)
  "Deserializes a message object of type '<visionRect>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'label) istream)
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'score) (roslisp-utils:decode-single-float-bits bits)))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'xmin)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'xmin)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'xmin)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'xmin)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'ymin)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'ymin)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'ymin)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'ymin)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'xmax)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'xmax)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'xmax)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'xmax)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'ymax)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'ymax)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'ymax)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'ymax)) (cl:read-byte istream))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'feature) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'feature)))
    (cl:dotimes (i __ros_arr_len)
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:aref vals i) (roslisp-utils:decode-single-float-bits bits))))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<visionRect>)))
  "Returns string type for a message object of type '<visionRect>"
  "comm_msg/visionRect")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'visionRect)))
  "Returns string type for a message object of type 'visionRect"
  "comm_msg/visionRect")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<visionRect>)))
  "Returns md5sum for a message object of type '<visionRect>"
  "b747118cb5066a7266881512a4bb9821")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'visionRect)))
  "Returns md5sum for a message object of type 'visionRect"
  "b747118cb5066a7266881512a4bb9821")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<visionRect>)))
  "Returns full string definition for message of type '<visionRect>"
  (cl:format cl:nil "label       label~%float32     score~%uint32      xmin~%uint32      ymin~%uint32      xmax~%uint32      ymax~%float32[]   feature~%================================================================================~%MSG: comm_msg/label~%#Note: 枚举类别、枚举值、枚举顺序均不可随意改动，不然md5不匹配，消息将无法正常解析~%~%uint8 UNKNOWN                   = 0     #未知（统一类别）~%uint8 PEDESTRIAN                = 1     #行人~%uint8 CYCLIST                   = 2     #骑自行车的人~%uint8 BICYCLE                   = 3     #自行车（两轮车）~%uint8 CAR                       = 4     #汽车~%uint8 TRUCK                     = 5     #卡车~%uint8 TRAM                      = 6     #有轨电车~%uint8 TRICYCLE                  = 7     #三轮车~%uint8 BUS                       = 8     #公交车~%~%uint8 LOWOBST                   = 10    #低矮障碍~%uint8 ROADEDGE                  = 11    #道路边缘~%uint8 PIPELINE                  = 12    #水管管线~%uint8 CORDON                    = 13    #警戒线~%uint8 TREELAWN                  = 14    #街道绿化带~%uint8 THICKPIPE                 = 15    #粗水管~%uint8 CORD                      = 16    #电线、缆绳~%uint8 SLOPE_PAD                 = 17    #斜坡垫~%uint8 SPEED_BUMP                = 18    #减速带~%~%uint8 COVER_SOLID               = 20    #实心井盖~%uint8 COVER_HOLLOW              = 21    #栅格井盖~%uint8 TRAFFIC_CONE              = 22    #雪糕筒（交通锥）~%uint8 METAL_BARRIER             = 23    #金属围栏（铁马）~%uint8 MEDIAN_BARRIER            = 24    #栅栏（道路中央隔离带）~%uint8 CEREMONIAL_POLE           = 25    #礼宾杆~%uint8 BOLLARD                   = 26    #路桩~%~%uint8 GARBAGE                   = 40    #垃圾（统称）~%uint8 GARBAGE_BOTTLE            = 41    #瓶子~%uint8 GARBAGE_PAPER             = 42    #纸巾~%uint8 GARBAGE_BRANCH            = 43    #树枝~%uint8 GARBAGE_LEAVES            = 44    #树叶~%uint8 GARBAGE_BOUGH             = 45    #粗树枝、长树枝~%uint8 GARBAGE_CUP               = 46    #奶茶杯~%uint8 GARBAGE_CAN               = 47    #易拉罐~%uint8 GARBAGE_CARTON            = 48    #纸盒~%uint8 GARBAGE_PACKAGE           = 49    #烟盒~%uint8 GARBAGE_BRICK             = 70    #砖块（小砖块）~%uint8 GARBAGE_CIGARETTEEND      = 71    #烟头~%~%~%uint8 TRAFFIC_LIGHT             = 50    #交通灯（统称）~%uint8 TRAFFIC_LIGHT_OFF         = 51    #交通灯-关闭~%uint8 TRAFFIC_LIGHT_RED         = 52    #交通灯-红灯~%uint8 TRAFFIC_LIGHT_GREEN       = 53    #交通灯-绿灯~%uint8 TRAFFIC_LIGHT_YELLOW      = 54    #交通灯-黄灯~%~%uint8 TRAFFIC_LIGHT_GROUP       = 55    #交通灯-灯组~%uint8 TRAFFIC_LIGHT_OFF_LITE    = 56    #交通灯-关闭（小灯）~%uint8 TRAFFIC_LIGHT_RED_LITE    = 57    #交通灯-红灯（小灯）~%uint8 TRAFFIC_LIGHT_GREEN_LITE  = 58    #交通灯-绿灯（小灯）~%uint8 TRAFFIC_LIGHT_YELLOW_LITE = 59    #交通灯-黄灯（小灯）~%~%uint8 CHARGING_GUN              = 60    #充电枪~%~%uint8 PLACEHOLDER               = 101    #占位符(MAX)~%~%uint8 value~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'visionRect)))
  "Returns full string definition for message of type 'visionRect"
  (cl:format cl:nil "label       label~%float32     score~%uint32      xmin~%uint32      ymin~%uint32      xmax~%uint32      ymax~%float32[]   feature~%================================================================================~%MSG: comm_msg/label~%#Note: 枚举类别、枚举值、枚举顺序均不可随意改动，不然md5不匹配，消息将无法正常解析~%~%uint8 UNKNOWN                   = 0     #未知（统一类别）~%uint8 PEDESTRIAN                = 1     #行人~%uint8 CYCLIST                   = 2     #骑自行车的人~%uint8 BICYCLE                   = 3     #自行车（两轮车）~%uint8 CAR                       = 4     #汽车~%uint8 TRUCK                     = 5     #卡车~%uint8 TRAM                      = 6     #有轨电车~%uint8 TRICYCLE                  = 7     #三轮车~%uint8 BUS                       = 8     #公交车~%~%uint8 LOWOBST                   = 10    #低矮障碍~%uint8 ROADEDGE                  = 11    #道路边缘~%uint8 PIPELINE                  = 12    #水管管线~%uint8 CORDON                    = 13    #警戒线~%uint8 TREELAWN                  = 14    #街道绿化带~%uint8 THICKPIPE                 = 15    #粗水管~%uint8 CORD                      = 16    #电线、缆绳~%uint8 SLOPE_PAD                 = 17    #斜坡垫~%uint8 SPEED_BUMP                = 18    #减速带~%~%uint8 COVER_SOLID               = 20    #实心井盖~%uint8 COVER_HOLLOW              = 21    #栅格井盖~%uint8 TRAFFIC_CONE              = 22    #雪糕筒（交通锥）~%uint8 METAL_BARRIER             = 23    #金属围栏（铁马）~%uint8 MEDIAN_BARRIER            = 24    #栅栏（道路中央隔离带）~%uint8 CEREMONIAL_POLE           = 25    #礼宾杆~%uint8 BOLLARD                   = 26    #路桩~%~%uint8 GARBAGE                   = 40    #垃圾（统称）~%uint8 GARBAGE_BOTTLE            = 41    #瓶子~%uint8 GARBAGE_PAPER             = 42    #纸巾~%uint8 GARBAGE_BRANCH            = 43    #树枝~%uint8 GARBAGE_LEAVES            = 44    #树叶~%uint8 GARBAGE_BOUGH             = 45    #粗树枝、长树枝~%uint8 GARBAGE_CUP               = 46    #奶茶杯~%uint8 GARBAGE_CAN               = 47    #易拉罐~%uint8 GARBAGE_CARTON            = 48    #纸盒~%uint8 GARBAGE_PACKAGE           = 49    #烟盒~%uint8 GARBAGE_BRICK             = 70    #砖块（小砖块）~%uint8 GARBAGE_CIGARETTEEND      = 71    #烟头~%~%~%uint8 TRAFFIC_LIGHT             = 50    #交通灯（统称）~%uint8 TRAFFIC_LIGHT_OFF         = 51    #交通灯-关闭~%uint8 TRAFFIC_LIGHT_RED         = 52    #交通灯-红灯~%uint8 TRAFFIC_LIGHT_GREEN       = 53    #交通灯-绿灯~%uint8 TRAFFIC_LIGHT_YELLOW      = 54    #交通灯-黄灯~%~%uint8 TRAFFIC_LIGHT_GROUP       = 55    #交通灯-灯组~%uint8 TRAFFIC_LIGHT_OFF_LITE    = 56    #交通灯-关闭（小灯）~%uint8 TRAFFIC_LIGHT_RED_LITE    = 57    #交通灯-红灯（小灯）~%uint8 TRAFFIC_LIGHT_GREEN_LITE  = 58    #交通灯-绿灯（小灯）~%uint8 TRAFFIC_LIGHT_YELLOW_LITE = 59    #交通灯-黄灯（小灯）~%~%uint8 CHARGING_GUN              = 60    #充电枪~%~%uint8 PLACEHOLDER               = 101    #占位符(MAX)~%~%uint8 value~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <visionRect>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'label))
     4
     4
     4
     4
     4
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'feature) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4)))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <visionRect>))
  "Converts a ROS message object to a list"
  (cl:list 'visionRect
    (cl:cons ':label (label msg))
    (cl:cons ':score (score msg))
    (cl:cons ':xmin (xmin msg))
    (cl:cons ':ymin (ymin msg))
    (cl:cons ':xmax (xmax msg))
    (cl:cons ':ymax (ymax msg))
    (cl:cons ':feature (feature msg))
))
