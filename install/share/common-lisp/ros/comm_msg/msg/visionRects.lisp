; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude visionRects.msg.html

(cl:defclass <visionRects> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (cameraId
    :reader cameraId
    :initarg :cameraId
    :type cl:string
    :initform "")
   (pipeline
    :reader pipeline
    :initarg :pipeline
    :type comm_msg-msg:label_state
    :initform (cl:make-instance 'comm_msg-msg:label_state))
   (rects
    :reader rects
    :initarg :rects
    :type (cl:vector comm_msg-msg:visionRect)
   :initform (cl:make-array 0 :element-type 'comm_msg-msg:visionRect :initial-element (cl:make-instance 'comm_msg-msg:visionRect))))
)

(cl:defclass visionRects (<visionRects>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <visionRects>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'visionRects)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<visionRects> is deprecated: use comm_msg-msg:visionRects instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <visionRects>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:header-val is deprecated.  Use comm_msg-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'cameraId-val :lambda-list '(m))
(cl:defmethod cameraId-val ((m <visionRects>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:cameraId-val is deprecated.  Use comm_msg-msg:cameraId instead.")
  (cameraId m))

(cl:ensure-generic-function 'pipeline-val :lambda-list '(m))
(cl:defmethod pipeline-val ((m <visionRects>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:pipeline-val is deprecated.  Use comm_msg-msg:pipeline instead.")
  (pipeline m))

(cl:ensure-generic-function 'rects-val :lambda-list '(m))
(cl:defmethod rects-val ((m <visionRects>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:rects-val is deprecated.  Use comm_msg-msg:rects instead.")
  (rects m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <visionRects>) ostream)
  "Serializes a message object of type '<visionRects>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'cameraId))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'cameraId))
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'pipeline) ostream)
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'rects))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'rects))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <visionRects>) istream)
  "Deserializes a message object of type '<visionRects>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'cameraId) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'cameraId) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'pipeline) istream)
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'rects) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'rects)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'comm_msg-msg:visionRect))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<visionRects>)))
  "Returns string type for a message object of type '<visionRects>"
  "comm_msg/visionRects")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'visionRects)))
  "Returns string type for a message object of type 'visionRects"
  "comm_msg/visionRects")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<visionRects>)))
  "Returns md5sum for a message object of type '<visionRects>"
  "2a1ba9962e7bc31dd8c2ea8621f61f8b")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'visionRects)))
  "Returns md5sum for a message object of type 'visionRects"
  "2a1ba9962e7bc31dd8c2ea8621f61f8b")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<visionRects>)))
  "Returns full string definition for message of type '<visionRects>"
  (cl:format cl:nil "std_msgs/Header     header~%string              cameraId~%label_state         pipeline    # 水管电线存在状态~%visionRect[]        rects~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%================================================================================~%MSG: comm_msg/label_state~%#Note: 标签类别存在状态~%~%uint8 NO  = 0~%uint8 YES = 1~%uint8 MAY = 2~%~%uint8 value~%================================================================================~%MSG: comm_msg/visionRect~%label       label~%float32     score~%uint32      xmin~%uint32      ymin~%uint32      xmax~%uint32      ymax~%float32[]   feature~%================================================================================~%MSG: comm_msg/label~%#Note: 枚举类别、枚举值、枚举顺序均不可随意改动，不然md5不匹配，消息将无法正常解析~%~%uint8 UNKNOWN                   = 0     #未知（统一类别）~%uint8 PEDESTRIAN                = 1     #行人~%uint8 CYCLIST                   = 2     #骑自行车的人~%uint8 BICYCLE                   = 3     #自行车（两轮车）~%uint8 CAR                       = 4     #汽车~%uint8 TRUCK                     = 5     #卡车~%uint8 TRAM                      = 6     #有轨电车~%uint8 TRICYCLE                  = 7     #三轮车~%uint8 BUS                       = 8     #公交车~%~%uint8 LOWOBST                   = 10    #低矮障碍~%uint8 ROADEDGE                  = 11    #道路边缘~%uint8 PIPELINE                  = 12    #水管管线~%uint8 CORDON                    = 13    #警戒线~%uint8 TREELAWN                  = 14    #街道绿化带~%uint8 THICKPIPE                 = 15    #粗水管~%uint8 CORD                      = 16    #电线、缆绳~%uint8 SLOPE_PAD                 = 17    #斜坡垫~%uint8 SPEED_BUMP                = 18    #减速带~%~%uint8 COVER_SOLID               = 20    #实心井盖~%uint8 COVER_HOLLOW              = 21    #栅格井盖~%uint8 TRAFFIC_CONE              = 22    #雪糕筒（交通锥）~%uint8 METAL_BARRIER             = 23    #金属围栏（铁马）~%uint8 MEDIAN_BARRIER            = 24    #栅栏（道路中央隔离带）~%uint8 CEREMONIAL_POLE           = 25    #礼宾杆~%uint8 BOLLARD                   = 26    #路桩~%~%uint8 GARBAGE                   = 40    #垃圾（统称）~%uint8 GARBAGE_BOTTLE            = 41    #瓶子~%uint8 GARBAGE_PAPER             = 42    #纸巾~%uint8 GARBAGE_BRANCH            = 43    #树枝~%uint8 GARBAGE_LEAVES            = 44    #树叶~%uint8 GARBAGE_BOUGH             = 45    #粗树枝、长树枝~%uint8 GARBAGE_CUP               = 46    #奶茶杯~%uint8 GARBAGE_CAN               = 47    #易拉罐~%uint8 GARBAGE_CARTON            = 48    #纸盒~%uint8 GARBAGE_PACKAGE           = 49    #烟盒~%uint8 GARBAGE_BRICK             = 70    #砖块（小砖块）~%uint8 GARBAGE_CIGARETTEEND      = 71    #烟头~%~%~%uint8 TRAFFIC_LIGHT             = 50    #交通灯（统称）~%uint8 TRAFFIC_LIGHT_OFF         = 51    #交通灯-关闭~%uint8 TRAFFIC_LIGHT_RED         = 52    #交通灯-红灯~%uint8 TRAFFIC_LIGHT_GREEN       = 53    #交通灯-绿灯~%uint8 TRAFFIC_LIGHT_YELLOW      = 54    #交通灯-黄灯~%~%uint8 TRAFFIC_LIGHT_GROUP       = 55    #交通灯-灯组~%uint8 TRAFFIC_LIGHT_OFF_LITE    = 56    #交通灯-关闭（小灯）~%uint8 TRAFFIC_LIGHT_RED_LITE    = 57    #交通灯-红灯（小灯）~%uint8 TRAFFIC_LIGHT_GREEN_LITE  = 58    #交通灯-绿灯（小灯）~%uint8 TRAFFIC_LIGHT_YELLOW_LITE = 59    #交通灯-黄灯（小灯）~%~%uint8 CHARGING_GUN              = 60    #充电枪~%~%uint8 PLACEHOLDER               = 101    #占位符(MAX)~%~%uint8 value~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'visionRects)))
  "Returns full string definition for message of type 'visionRects"
  (cl:format cl:nil "std_msgs/Header     header~%string              cameraId~%label_state         pipeline    # 水管电线存在状态~%visionRect[]        rects~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%================================================================================~%MSG: comm_msg/label_state~%#Note: 标签类别存在状态~%~%uint8 NO  = 0~%uint8 YES = 1~%uint8 MAY = 2~%~%uint8 value~%================================================================================~%MSG: comm_msg/visionRect~%label       label~%float32     score~%uint32      xmin~%uint32      ymin~%uint32      xmax~%uint32      ymax~%float32[]   feature~%================================================================================~%MSG: comm_msg/label~%#Note: 枚举类别、枚举值、枚举顺序均不可随意改动，不然md5不匹配，消息将无法正常解析~%~%uint8 UNKNOWN                   = 0     #未知（统一类别）~%uint8 PEDESTRIAN                = 1     #行人~%uint8 CYCLIST                   = 2     #骑自行车的人~%uint8 BICYCLE                   = 3     #自行车（两轮车）~%uint8 CAR                       = 4     #汽车~%uint8 TRUCK                     = 5     #卡车~%uint8 TRAM                      = 6     #有轨电车~%uint8 TRICYCLE                  = 7     #三轮车~%uint8 BUS                       = 8     #公交车~%~%uint8 LOWOBST                   = 10    #低矮障碍~%uint8 ROADEDGE                  = 11    #道路边缘~%uint8 PIPELINE                  = 12    #水管管线~%uint8 CORDON                    = 13    #警戒线~%uint8 TREELAWN                  = 14    #街道绿化带~%uint8 THICKPIPE                 = 15    #粗水管~%uint8 CORD                      = 16    #电线、缆绳~%uint8 SLOPE_PAD                 = 17    #斜坡垫~%uint8 SPEED_BUMP                = 18    #减速带~%~%uint8 COVER_SOLID               = 20    #实心井盖~%uint8 COVER_HOLLOW              = 21    #栅格井盖~%uint8 TRAFFIC_CONE              = 22    #雪糕筒（交通锥）~%uint8 METAL_BARRIER             = 23    #金属围栏（铁马）~%uint8 MEDIAN_BARRIER            = 24    #栅栏（道路中央隔离带）~%uint8 CEREMONIAL_POLE           = 25    #礼宾杆~%uint8 BOLLARD                   = 26    #路桩~%~%uint8 GARBAGE                   = 40    #垃圾（统称）~%uint8 GARBAGE_BOTTLE            = 41    #瓶子~%uint8 GARBAGE_PAPER             = 42    #纸巾~%uint8 GARBAGE_BRANCH            = 43    #树枝~%uint8 GARBAGE_LEAVES            = 44    #树叶~%uint8 GARBAGE_BOUGH             = 45    #粗树枝、长树枝~%uint8 GARBAGE_CUP               = 46    #奶茶杯~%uint8 GARBAGE_CAN               = 47    #易拉罐~%uint8 GARBAGE_CARTON            = 48    #纸盒~%uint8 GARBAGE_PACKAGE           = 49    #烟盒~%uint8 GARBAGE_BRICK             = 70    #砖块（小砖块）~%uint8 GARBAGE_CIGARETTEEND      = 71    #烟头~%~%~%uint8 TRAFFIC_LIGHT             = 50    #交通灯（统称）~%uint8 TRAFFIC_LIGHT_OFF         = 51    #交通灯-关闭~%uint8 TRAFFIC_LIGHT_RED         = 52    #交通灯-红灯~%uint8 TRAFFIC_LIGHT_GREEN       = 53    #交通灯-绿灯~%uint8 TRAFFIC_LIGHT_YELLOW      = 54    #交通灯-黄灯~%~%uint8 TRAFFIC_LIGHT_GROUP       = 55    #交通灯-灯组~%uint8 TRAFFIC_LIGHT_OFF_LITE    = 56    #交通灯-关闭（小灯）~%uint8 TRAFFIC_LIGHT_RED_LITE    = 57    #交通灯-红灯（小灯）~%uint8 TRAFFIC_LIGHT_GREEN_LITE  = 58    #交通灯-绿灯（小灯）~%uint8 TRAFFIC_LIGHT_YELLOW_LITE = 59    #交通灯-黄灯（小灯）~%~%uint8 CHARGING_GUN              = 60    #充电枪~%~%uint8 PLACEHOLDER               = 101    #占位符(MAX)~%~%uint8 value~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <visionRects>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     4 (cl:length (cl:slot-value msg 'cameraId))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'pipeline))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'rects) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <visionRects>))
  "Converts a ROS message object to a list"
  (cl:list 'visionRects
    (cl:cons ':header (header msg))
    (cl:cons ':cameraId (cameraId msg))
    (cl:cons ':pipeline (pipeline msg))
    (cl:cons ':rects (rects msg))
))
