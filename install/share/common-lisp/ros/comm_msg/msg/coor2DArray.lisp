; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude coor2DArray.msg.html

(cl:defclass <coor2DArray> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (isContainForwardObst
    :reader isContainForwardObst
    :initarg :isContainForwardObst
    :type cl:fixnum
    :initform 0)
   (isContainBackwardObst
    :reader isContainBackwardObst
    :initarg :isContainBackwardObst
    :type cl:fixnum
    :initform 0)
   (coor2DArray
    :reader coor2DArray
    :initarg :coor2DArray
    :type (cl:vector comm_msg-msg:coor2D)
   :initform (cl:make-array 0 :element-type 'comm_msg-msg:coor2D :initial-element (cl:make-instance 'comm_msg-msg:coor2D)))
   (coor2DArrayTop
    :reader coor2DArrayTop
    :initarg :coor2DArrayTop
    :type (cl:vector comm_msg-msg:coor2D)
   :initform (cl:make-array 0 :element-type 'comm_msg-msg:coor2D :initial-element (cl:make-instance 'comm_msg-msg:coor2D)))
   (coor2DArrayMid
    :reader coor2DArrayMid
    :initarg :coor2DArrayMid
    :type (cl:vector comm_msg-msg:coor2D)
   :initform (cl:make-array 0 :element-type 'comm_msg-msg:coor2D :initial-element (cl:make-instance 'comm_msg-msg:coor2D)))
   (coor2DArrayDown
    :reader coor2DArrayDown
    :initarg :coor2DArrayDown
    :type (cl:vector comm_msg-msg:coor2D)
   :initform (cl:make-array 0 :element-type 'comm_msg-msg:coor2D :initial-element (cl:make-instance 'comm_msg-msg:coor2D))))
)

(cl:defclass coor2DArray (<coor2DArray>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <coor2DArray>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'coor2DArray)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<coor2DArray> is deprecated: use comm_msg-msg:coor2DArray instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <coor2DArray>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:header-val is deprecated.  Use comm_msg-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'isContainForwardObst-val :lambda-list '(m))
(cl:defmethod isContainForwardObst-val ((m <coor2DArray>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isContainForwardObst-val is deprecated.  Use comm_msg-msg:isContainForwardObst instead.")
  (isContainForwardObst m))

(cl:ensure-generic-function 'isContainBackwardObst-val :lambda-list '(m))
(cl:defmethod isContainBackwardObst-val ((m <coor2DArray>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isContainBackwardObst-val is deprecated.  Use comm_msg-msg:isContainBackwardObst instead.")
  (isContainBackwardObst m))

(cl:ensure-generic-function 'coor2DArray-val :lambda-list '(m))
(cl:defmethod coor2DArray-val ((m <coor2DArray>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:coor2DArray-val is deprecated.  Use comm_msg-msg:coor2DArray instead.")
  (coor2DArray m))

(cl:ensure-generic-function 'coor2DArrayTop-val :lambda-list '(m))
(cl:defmethod coor2DArrayTop-val ((m <coor2DArray>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:coor2DArrayTop-val is deprecated.  Use comm_msg-msg:coor2DArrayTop instead.")
  (coor2DArrayTop m))

(cl:ensure-generic-function 'coor2DArrayMid-val :lambda-list '(m))
(cl:defmethod coor2DArrayMid-val ((m <coor2DArray>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:coor2DArrayMid-val is deprecated.  Use comm_msg-msg:coor2DArrayMid instead.")
  (coor2DArrayMid m))

(cl:ensure-generic-function 'coor2DArrayDown-val :lambda-list '(m))
(cl:defmethod coor2DArrayDown-val ((m <coor2DArray>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:coor2DArrayDown-val is deprecated.  Use comm_msg-msg:coor2DArrayDown instead.")
  (coor2DArrayDown m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <coor2DArray>) ostream)
  "Serializes a message object of type '<coor2DArray>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isContainForwardObst)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isContainBackwardObst)) ostream)
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'coor2DArray))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'coor2DArray))
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'coor2DArrayTop))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'coor2DArrayTop))
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'coor2DArrayMid))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'coor2DArrayMid))
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'coor2DArrayDown))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'coor2DArrayDown))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <coor2DArray>) istream)
  "Deserializes a message object of type '<coor2DArray>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isContainForwardObst)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isContainBackwardObst)) (cl:read-byte istream))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'coor2DArray) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'coor2DArray)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'comm_msg-msg:coor2D))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'coor2DArrayTop) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'coor2DArrayTop)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'comm_msg-msg:coor2D))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'coor2DArrayMid) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'coor2DArrayMid)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'comm_msg-msg:coor2D))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'coor2DArrayDown) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'coor2DArrayDown)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'comm_msg-msg:coor2D))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<coor2DArray>)))
  "Returns string type for a message object of type '<coor2DArray>"
  "comm_msg/coor2DArray")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'coor2DArray)))
  "Returns string type for a message object of type 'coor2DArray"
  "comm_msg/coor2DArray")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<coor2DArray>)))
  "Returns md5sum for a message object of type '<coor2DArray>"
  "c15593ec81512cc55beecc6a469f2978")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'coor2DArray)))
  "Returns md5sum for a message object of type 'coor2DArray"
  "c15593ec81512cc55beecc6a469f2978")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<coor2DArray>)))
  "Returns full string definition for message of type '<coor2DArray>"
  (cl:format cl:nil "Header    header~%uint8     isContainForwardObst~%uint8     isContainBackwardObst~%coor2D[]  coor2DArray       #待删除~%coor2D[]  coor2DArrayTop~%coor2D[]  coor2DArrayMid~%coor2D[]  coor2DArrayDown~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%================================================================================~%MSG: comm_msg/coor2D~%coorLabel   label       #坐标类别~%float32     pos_x~%float32     pos_y~%================================================================================~%MSG: comm_msg/coorLabel~%uint8   UNKNOWN      = 0  #未知~%uint8   STATIC       = 1  #静态~%uint8   DYNAMIC      = 2  #动态~%uint8   HOLLOW_OBST  = 3  #空洞障碍~%uint8   SHORT_HOLD   = 4  #低矮维持~%uint8   COMMON_HOLD  = 5  #常规维持~%uint8   BLIND_SPOT   = 6  #盲区障碍~%uint8   SLOPE_PAD    = 7  #斜坡垫~%uint8   CONTACT_EDGE = 8  #触边障碍~%uint8   TREELAWN     = 9  #绿植障碍~%uint8   ULTRASONIC   = 10 #超声~%~%uint8 value~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'coor2DArray)))
  "Returns full string definition for message of type 'coor2DArray"
  (cl:format cl:nil "Header    header~%uint8     isContainForwardObst~%uint8     isContainBackwardObst~%coor2D[]  coor2DArray       #待删除~%coor2D[]  coor2DArrayTop~%coor2D[]  coor2DArrayMid~%coor2D[]  coor2DArrayDown~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%================================================================================~%MSG: comm_msg/coor2D~%coorLabel   label       #坐标类别~%float32     pos_x~%float32     pos_y~%================================================================================~%MSG: comm_msg/coorLabel~%uint8   UNKNOWN      = 0  #未知~%uint8   STATIC       = 1  #静态~%uint8   DYNAMIC      = 2  #动态~%uint8   HOLLOW_OBST  = 3  #空洞障碍~%uint8   SHORT_HOLD   = 4  #低矮维持~%uint8   COMMON_HOLD  = 5  #常规维持~%uint8   BLIND_SPOT   = 6  #盲区障碍~%uint8   SLOPE_PAD    = 7  #斜坡垫~%uint8   CONTACT_EDGE = 8  #触边障碍~%uint8   TREELAWN     = 9  #绿植障碍~%uint8   ULTRASONIC   = 10 #超声~%~%uint8 value~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <coor2DArray>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     1
     1
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'coor2DArray) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'coor2DArrayTop) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'coor2DArrayMid) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'coor2DArrayDown) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <coor2DArray>))
  "Converts a ROS message object to a list"
  (cl:list 'coor2DArray
    (cl:cons ':header (header msg))
    (cl:cons ':isContainForwardObst (isContainForwardObst msg))
    (cl:cons ':isContainBackwardObst (isContainBackwardObst msg))
    (cl:cons ':coor2DArray (coor2DArray msg))
    (cl:cons ':coor2DArrayTop (coor2DArrayTop msg))
    (cl:cons ':coor2DArrayMid (coor2DArrayMid msg))
    (cl:cons ':coor2DArrayDown (coor2DArrayDown msg))
))
