; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude spoiler_detect.msg.html

(cl:defclass <spoiler_detect> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (label
    :reader label
    :initarg :label
    :type comm_msg-msg:label
    :initform (cl:make-instance 'comm_msg-msg:label))
   (detect_value
    :reader detect_value
    :initarg :detect_value
    :type cl:fixnum
    :initform 0))
)

(cl:defclass spoiler_detect (<spoiler_detect>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <spoiler_detect>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'spoiler_detect)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<spoiler_detect> is deprecated: use comm_msg-msg:spoiler_detect instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <spoiler_detect>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:header-val is deprecated.  Use comm_msg-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'label-val :lambda-list '(m))
(cl:defmethod label-val ((m <spoiler_detect>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:label-val is deprecated.  Use comm_msg-msg:label instead.")
  (label m))

(cl:ensure-generic-function 'detect_value-val :lambda-list '(m))
(cl:defmethod detect_value-val ((m <spoiler_detect>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:detect_value-val is deprecated.  Use comm_msg-msg:detect_value instead.")
  (detect_value m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <spoiler_detect>) ostream)
  "Serializes a message object of type '<spoiler_detect>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'label) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'detect_value)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <spoiler_detect>) istream)
  "Deserializes a message object of type '<spoiler_detect>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'label) istream)
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'detect_value)) (cl:read-byte istream))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<spoiler_detect>)))
  "Returns string type for a message object of type '<spoiler_detect>"
  "comm_msg/spoiler_detect")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'spoiler_detect)))
  "Returns string type for a message object of type 'spoiler_detect"
  "comm_msg/spoiler_detect")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<spoiler_detect>)))
  "Returns md5sum for a message object of type '<spoiler_detect>"
  "03ce56ab1e5a7e206499aa32ad624a78")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'spoiler_detect)))
  "Returns md5sum for a message object of type 'spoiler_detect"
  "03ce56ab1e5a7e206499aa32ad624a78")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<spoiler_detect>)))
  "Returns full string definition for message of type '<spoiler_detect>"
  (cl:format cl:nil "Header header~%label label         # 检测到的尾翼范围内障碍物类别~%uint8 detect_value  # 检测到尾翼范围内是障碍物存在  0: no, 1: yes~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%================================================================================~%MSG: comm_msg/label~%#Note: 枚举类别、枚举值、枚举顺序均不可随意改动，不然md5不匹配，消息将无法正常解析~%~%uint8 UNKNOWN                   = 0     #未知（统一类别）~%uint8 PEDESTRIAN                = 1     #行人~%uint8 CYCLIST                   = 2     #骑自行车的人~%uint8 BICYCLE                   = 3     #自行车（两轮车）~%uint8 CAR                       = 4     #汽车~%uint8 TRUCK                     = 5     #卡车~%uint8 TRAM                      = 6     #有轨电车~%uint8 TRICYCLE                  = 7     #三轮车~%uint8 BUS                       = 8     #公交车~%~%uint8 LOWOBST                   = 10    #低矮障碍~%uint8 ROADEDGE                  = 11    #道路边缘~%uint8 PIPELINE                  = 12    #水管管线~%uint8 CORDON                    = 13    #警戒线~%uint8 TREELAWN                  = 14    #街道绿化带~%uint8 THICKPIPE                 = 15    #粗水管~%uint8 CORD                      = 16    #电线、缆绳~%uint8 SLOPE_PAD                 = 17    #斜坡垫~%uint8 SPEED_BUMP                = 18    #减速带~%~%uint8 COVER_SOLID               = 20    #实心井盖~%uint8 COVER_HOLLOW              = 21    #栅格井盖~%uint8 TRAFFIC_CONE              = 22    #雪糕筒（交通锥）~%uint8 METAL_BARRIER             = 23    #金属围栏（铁马）~%uint8 MEDIAN_BARRIER            = 24    #栅栏（道路中央隔离带）~%uint8 CEREMONIAL_POLE           = 25    #礼宾杆~%uint8 BOLLARD                   = 26    #路桩~%~%uint8 GARBAGE                   = 40    #垃圾（统称）~%uint8 GARBAGE_BOTTLE            = 41    #瓶子~%uint8 GARBAGE_PAPER             = 42    #纸巾~%uint8 GARBAGE_BRANCH            = 43    #树枝~%uint8 GARBAGE_LEAVES            = 44    #树叶~%uint8 GARBAGE_BOUGH             = 45    #粗树枝、长树枝~%uint8 GARBAGE_CUP               = 46    #奶茶杯~%uint8 GARBAGE_CAN               = 47    #易拉罐~%uint8 GARBAGE_CARTON            = 48    #纸盒~%uint8 GARBAGE_PACKAGE           = 49    #烟盒~%uint8 GARBAGE_BRICK             = 70    #砖块（小砖块）~%uint8 GARBAGE_CIGARETTEEND      = 71    #烟头~%~%~%uint8 TRAFFIC_LIGHT             = 50    #交通灯（统称）~%uint8 TRAFFIC_LIGHT_OFF         = 51    #交通灯-关闭~%uint8 TRAFFIC_LIGHT_RED         = 52    #交通灯-红灯~%uint8 TRAFFIC_LIGHT_GREEN       = 53    #交通灯-绿灯~%uint8 TRAFFIC_LIGHT_YELLOW      = 54    #交通灯-黄灯~%~%uint8 TRAFFIC_LIGHT_GROUP       = 55    #交通灯-灯组~%uint8 TRAFFIC_LIGHT_OFF_LITE    = 56    #交通灯-关闭（小灯）~%uint8 TRAFFIC_LIGHT_RED_LITE    = 57    #交通灯-红灯（小灯）~%uint8 TRAFFIC_LIGHT_GREEN_LITE  = 58    #交通灯-绿灯（小灯）~%uint8 TRAFFIC_LIGHT_YELLOW_LITE = 59    #交通灯-黄灯（小灯）~%~%uint8 CHARGING_GUN              = 60    #充电枪~%~%uint8 PLACEHOLDER               = 101    #占位符(MAX)~%~%uint8 value~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'spoiler_detect)))
  "Returns full string definition for message of type 'spoiler_detect"
  (cl:format cl:nil "Header header~%label label         # 检测到的尾翼范围内障碍物类别~%uint8 detect_value  # 检测到尾翼范围内是障碍物存在  0: no, 1: yes~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%================================================================================~%MSG: comm_msg/label~%#Note: 枚举类别、枚举值、枚举顺序均不可随意改动，不然md5不匹配，消息将无法正常解析~%~%uint8 UNKNOWN                   = 0     #未知（统一类别）~%uint8 PEDESTRIAN                = 1     #行人~%uint8 CYCLIST                   = 2     #骑自行车的人~%uint8 BICYCLE                   = 3     #自行车（两轮车）~%uint8 CAR                       = 4     #汽车~%uint8 TRUCK                     = 5     #卡车~%uint8 TRAM                      = 6     #有轨电车~%uint8 TRICYCLE                  = 7     #三轮车~%uint8 BUS                       = 8     #公交车~%~%uint8 LOWOBST                   = 10    #低矮障碍~%uint8 ROADEDGE                  = 11    #道路边缘~%uint8 PIPELINE                  = 12    #水管管线~%uint8 CORDON                    = 13    #警戒线~%uint8 TREELAWN                  = 14    #街道绿化带~%uint8 THICKPIPE                 = 15    #粗水管~%uint8 CORD                      = 16    #电线、缆绳~%uint8 SLOPE_PAD                 = 17    #斜坡垫~%uint8 SPEED_BUMP                = 18    #减速带~%~%uint8 COVER_SOLID               = 20    #实心井盖~%uint8 COVER_HOLLOW              = 21    #栅格井盖~%uint8 TRAFFIC_CONE              = 22    #雪糕筒（交通锥）~%uint8 METAL_BARRIER             = 23    #金属围栏（铁马）~%uint8 MEDIAN_BARRIER            = 24    #栅栏（道路中央隔离带）~%uint8 CEREMONIAL_POLE           = 25    #礼宾杆~%uint8 BOLLARD                   = 26    #路桩~%~%uint8 GARBAGE                   = 40    #垃圾（统称）~%uint8 GARBAGE_BOTTLE            = 41    #瓶子~%uint8 GARBAGE_PAPER             = 42    #纸巾~%uint8 GARBAGE_BRANCH            = 43    #树枝~%uint8 GARBAGE_LEAVES            = 44    #树叶~%uint8 GARBAGE_BOUGH             = 45    #粗树枝、长树枝~%uint8 GARBAGE_CUP               = 46    #奶茶杯~%uint8 GARBAGE_CAN               = 47    #易拉罐~%uint8 GARBAGE_CARTON            = 48    #纸盒~%uint8 GARBAGE_PACKAGE           = 49    #烟盒~%uint8 GARBAGE_BRICK             = 70    #砖块（小砖块）~%uint8 GARBAGE_CIGARETTEEND      = 71    #烟头~%~%~%uint8 TRAFFIC_LIGHT             = 50    #交通灯（统称）~%uint8 TRAFFIC_LIGHT_OFF         = 51    #交通灯-关闭~%uint8 TRAFFIC_LIGHT_RED         = 52    #交通灯-红灯~%uint8 TRAFFIC_LIGHT_GREEN       = 53    #交通灯-绿灯~%uint8 TRAFFIC_LIGHT_YELLOW      = 54    #交通灯-黄灯~%~%uint8 TRAFFIC_LIGHT_GROUP       = 55    #交通灯-灯组~%uint8 TRAFFIC_LIGHT_OFF_LITE    = 56    #交通灯-关闭（小灯）~%uint8 TRAFFIC_LIGHT_RED_LITE    = 57    #交通灯-红灯（小灯）~%uint8 TRAFFIC_LIGHT_GREEN_LITE  = 58    #交通灯-绿灯（小灯）~%uint8 TRAFFIC_LIGHT_YELLOW_LITE = 59    #交通灯-黄灯（小灯）~%~%uint8 CHARGING_GUN              = 60    #充电枪~%~%uint8 PLACEHOLDER               = 101    #占位符(MAX)~%~%uint8 value~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <spoiler_detect>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'label))
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <spoiler_detect>))
  "Converts a ROS message object to a list"
  (cl:list 'spoiler_detect
    (cl:cons ':header (header msg))
    (cl:cons ':label (label msg))
    (cl:cons ':detect_value (detect_value msg))
))
