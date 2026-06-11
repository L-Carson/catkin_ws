; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude ipu.msg.html

(cl:defclass <ipu> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header)))
)

(cl:defclass ipu (<ipu>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <ipu>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'ipu)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<ipu> is deprecated: use comm_msg-msg:ipu instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <ipu>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:header-val is deprecated.  Use comm_msg-msg:header instead.")
  (header m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <ipu>) ostream)
  "Serializes a message object of type '<ipu>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <ipu>) istream)
  "Deserializes a message object of type '<ipu>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<ipu>)))
  "Returns string type for a message object of type '<ipu>"
  "comm_msg/ipu")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'ipu)))
  "Returns string type for a message object of type 'ipu"
  "comm_msg/ipu")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<ipu>)))
  "Returns md5sum for a message object of type '<ipu>"
  "d7be0bb39af8fb9129d5a76e6b63a290")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'ipu)))
  "Returns md5sum for a message object of type 'ipu"
  "d7be0bb39af8fb9129d5a76e6b63a290")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<ipu>)))
  "Returns full string definition for message of type '<ipu>"
  (cl:format cl:nil "Header header   #心跳包~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'ipu)))
  "Returns full string definition for message of type 'ipu"
  (cl:format cl:nil "Header header   #心跳包~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <ipu>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <ipu>))
  "Converts a ROS message object to a list"
  (cl:list 'ipu
    (cl:cons ':header (header msg))
))
