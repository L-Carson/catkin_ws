; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude ultrasonicsData.msg.html

(cl:defclass <ultrasonicsData> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (usData
    :reader usData
    :initarg :usData
    :type (cl:vector comm_msg-msg:ultrasonicData)
   :initform (cl:make-array 0 :element-type 'comm_msg-msg:ultrasonicData :initial-element (cl:make-instance 'comm_msg-msg:ultrasonicData))))
)

(cl:defclass ultrasonicsData (<ultrasonicsData>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <ultrasonicsData>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'ultrasonicsData)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<ultrasonicsData> is deprecated: use comm_msg-msg:ultrasonicsData instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <ultrasonicsData>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:header-val is deprecated.  Use comm_msg-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'usData-val :lambda-list '(m))
(cl:defmethod usData-val ((m <ultrasonicsData>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:usData-val is deprecated.  Use comm_msg-msg:usData instead.")
  (usData m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <ultrasonicsData>) ostream)
  "Serializes a message object of type '<ultrasonicsData>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'usData))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'usData))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <ultrasonicsData>) istream)
  "Deserializes a message object of type '<ultrasonicsData>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'usData) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'usData)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'comm_msg-msg:ultrasonicData))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<ultrasonicsData>)))
  "Returns string type for a message object of type '<ultrasonicsData>"
  "comm_msg/ultrasonicsData")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'ultrasonicsData)))
  "Returns string type for a message object of type 'ultrasonicsData"
  "comm_msg/ultrasonicsData")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<ultrasonicsData>)))
  "Returns md5sum for a message object of type '<ultrasonicsData>"
  "107cce378ac5b6b859f4402e5340a53f")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'ultrasonicsData)))
  "Returns md5sum for a message object of type 'ultrasonicsData"
  "107cce378ac5b6b859f4402e5340a53f")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<ultrasonicsData>)))
  "Returns full string definition for message of type '<ultrasonicsData>"
  (cl:format cl:nil "Header           header~%ultrasonicData[] usData~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%================================================================================~%MSG: comm_msg/ultrasonicData~%string   usName~%coor2D[] data~%================================================================================~%MSG: comm_msg/coor2D~%coorLabel   label       #坐标类别~%float32     pos_x~%float32     pos_y~%================================================================================~%MSG: comm_msg/coorLabel~%uint8   UNKNOWN      = 0  #未知~%uint8   STATIC       = 1  #静态~%uint8   DYNAMIC      = 2  #动态~%uint8   HOLLOW_OBST  = 3  #空洞障碍~%uint8   SHORT_HOLD   = 4  #低矮维持~%uint8   COMMON_HOLD  = 5  #常规维持~%uint8   BLIND_SPOT   = 6  #盲区障碍~%uint8   SLOPE_PAD    = 7  #斜坡垫~%uint8   CONTACT_EDGE = 8  #触边障碍~%uint8   TREELAWN     = 9  #绿植障碍~%uint8   ULTRASONIC   = 10 #超声~%~%uint8 value~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'ultrasonicsData)))
  "Returns full string definition for message of type 'ultrasonicsData"
  (cl:format cl:nil "Header           header~%ultrasonicData[] usData~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%================================================================================~%MSG: comm_msg/ultrasonicData~%string   usName~%coor2D[] data~%================================================================================~%MSG: comm_msg/coor2D~%coorLabel   label       #坐标类别~%float32     pos_x~%float32     pos_y~%================================================================================~%MSG: comm_msg/coorLabel~%uint8   UNKNOWN      = 0  #未知~%uint8   STATIC       = 1  #静态~%uint8   DYNAMIC      = 2  #动态~%uint8   HOLLOW_OBST  = 3  #空洞障碍~%uint8   SHORT_HOLD   = 4  #低矮维持~%uint8   COMMON_HOLD  = 5  #常规维持~%uint8   BLIND_SPOT   = 6  #盲区障碍~%uint8   SLOPE_PAD    = 7  #斜坡垫~%uint8   CONTACT_EDGE = 8  #触边障碍~%uint8   TREELAWN     = 9  #绿植障碍~%uint8   ULTRASONIC   = 10 #超声~%~%uint8 value~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <ultrasonicsData>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'usData) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <ultrasonicsData>))
  "Converts a ROS message object to a list"
  (cl:list 'ultrasonicsData
    (cl:cons ':header (header msg))
    (cl:cons ':usData (usData msg))
))
