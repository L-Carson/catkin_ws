; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude radarTargetArray.msg.html

(cl:defclass <radarTargetArray> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (radarID
    :reader radarID
    :initarg :radarID
    :type cl:fixnum
    :initform 0)
   (allTargets
    :reader allTargets
    :initarg :allTargets
    :type (cl:vector comm_msg-msg:radarTarget)
   :initform (cl:make-array 0 :element-type 'comm_msg-msg:radarTarget :initial-element (cl:make-instance 'comm_msg-msg:radarTarget))))
)

(cl:defclass radarTargetArray (<radarTargetArray>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <radarTargetArray>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'radarTargetArray)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<radarTargetArray> is deprecated: use comm_msg-msg:radarTargetArray instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <radarTargetArray>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:header-val is deprecated.  Use comm_msg-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'radarID-val :lambda-list '(m))
(cl:defmethod radarID-val ((m <radarTargetArray>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:radarID-val is deprecated.  Use comm_msg-msg:radarID instead.")
  (radarID m))

(cl:ensure-generic-function 'allTargets-val :lambda-list '(m))
(cl:defmethod allTargets-val ((m <radarTargetArray>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:allTargets-val is deprecated.  Use comm_msg-msg:allTargets instead.")
  (allTargets m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <radarTargetArray>) ostream)
  "Serializes a message object of type '<radarTargetArray>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:let* ((signed (cl:slot-value msg 'radarID)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'allTargets))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'allTargets))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <radarTargetArray>) istream)
  "Deserializes a message object of type '<radarTargetArray>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'radarID) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'allTargets) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'allTargets)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'comm_msg-msg:radarTarget))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<radarTargetArray>)))
  "Returns string type for a message object of type '<radarTargetArray>"
  "comm_msg/radarTargetArray")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'radarTargetArray)))
  "Returns string type for a message object of type 'radarTargetArray"
  "comm_msg/radarTargetArray")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<radarTargetArray>)))
  "Returns md5sum for a message object of type '<radarTargetArray>"
  "6dbbce6501fbb2b1edc228d20004f209")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'radarTargetArray)))
  "Returns md5sum for a message object of type 'radarTargetArray"
  "6dbbce6501fbb2b1edc228d20004f209")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<radarTargetArray>)))
  "Returns full string definition for message of type '<radarTargetArray>"
  (cl:format cl:nil "Header          header~%int16           radarID~%radarTarget[]   allTargets~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%================================================================================~%MSG: comm_msg/radarTarget~%float32   targetDistx         # 目标相对于清扫车参考坐标系X轴坐标~%float32   targetDisty         # 目标相对于清扫车参考坐标系Y轴坐标~%float32   targetVelx          # 目标相对于清扫车参考坐标系X轴速度~%float32   targetVely          # 目标相对于清扫车参考坐标系Y轴速度~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'radarTargetArray)))
  "Returns full string definition for message of type 'radarTargetArray"
  (cl:format cl:nil "Header          header~%int16           radarID~%radarTarget[]   allTargets~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%================================================================================~%MSG: comm_msg/radarTarget~%float32   targetDistx         # 目标相对于清扫车参考坐标系X轴坐标~%float32   targetDisty         # 目标相对于清扫车参考坐标系Y轴坐标~%float32   targetVelx          # 目标相对于清扫车参考坐标系X轴速度~%float32   targetVely          # 目标相对于清扫车参考坐标系Y轴速度~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <radarTargetArray>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     2
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'allTargets) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <radarTargetArray>))
  "Converts a ROS message object to a list"
  (cl:list 'radarTargetArray
    (cl:cons ':header (header msg))
    (cl:cons ':radarID (radarID msg))
    (cl:cons ':allTargets (allTargets msg))
))
