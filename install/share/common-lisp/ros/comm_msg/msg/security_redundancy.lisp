; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude security_redundancy.msg.html

(cl:defclass <security_redundancy> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (isStopLocationAbnormal
    :reader isStopLocationAbnormal
    :initarg :isStopLocationAbnormal
    :type cl:fixnum
    :initform 0)
   (isStopCollision
    :reader isStopCollision
    :initarg :isStopCollision
    :type cl:fixnum
    :initform 0))
)

(cl:defclass security_redundancy (<security_redundancy>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <security_redundancy>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'security_redundancy)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<security_redundancy> is deprecated: use comm_msg-msg:security_redundancy instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <security_redundancy>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:header-val is deprecated.  Use comm_msg-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'isStopLocationAbnormal-val :lambda-list '(m))
(cl:defmethod isStopLocationAbnormal-val ((m <security_redundancy>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isStopLocationAbnormal-val is deprecated.  Use comm_msg-msg:isStopLocationAbnormal instead.")
  (isStopLocationAbnormal m))

(cl:ensure-generic-function 'isStopCollision-val :lambda-list '(m))
(cl:defmethod isStopCollision-val ((m <security_redundancy>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isStopCollision-val is deprecated.  Use comm_msg-msg:isStopCollision instead.")
  (isStopCollision m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <security_redundancy>) ostream)
  "Serializes a message object of type '<security_redundancy>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isStopLocationAbnormal)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isStopCollision)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <security_redundancy>) istream)
  "Deserializes a message object of type '<security_redundancy>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isStopLocationAbnormal)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isStopCollision)) (cl:read-byte istream))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<security_redundancy>)))
  "Returns string type for a message object of type '<security_redundancy>"
  "comm_msg/security_redundancy")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'security_redundancy)))
  "Returns string type for a message object of type 'security_redundancy"
  "comm_msg/security_redundancy")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<security_redundancy>)))
  "Returns md5sum for a message object of type '<security_redundancy>"
  "fc488962e3f3ac9e71c3b0975a57ab80")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'security_redundancy)))
  "Returns md5sum for a message object of type 'security_redundancy"
  "fc488962e3f3ac9e71c3b0975a57ab80")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<security_redundancy>)))
  "Returns full string definition for message of type '<security_redundancy>"
  (cl:format cl:nil "Header  header~%~%uint8   isStopLocationAbnormal~%uint8   isStopCollision~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'security_redundancy)))
  "Returns full string definition for message of type 'security_redundancy"
  (cl:format cl:nil "Header  header~%~%uint8   isStopLocationAbnormal~%uint8   isStopCollision~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <security_redundancy>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     1
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <security_redundancy>))
  "Converts a ROS message object to a list"
  (cl:list 'security_redundancy
    (cl:cons ':header (header msg))
    (cl:cons ':isStopLocationAbnormal (isStopLocationAbnormal msg))
    (cl:cons ':isStopCollision (isStopCollision msg))
))
