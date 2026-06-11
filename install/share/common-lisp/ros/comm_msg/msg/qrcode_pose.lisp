; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude qrcode_pose.msg.html

(cl:defclass <qrcode_pose> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (id
    :reader id
    :initarg :id
    :type cl:integer
    :initform 0)
   (pose
    :reader pose
    :initarg :pose
    :type geometry_msgs-msg:Pose
    :initform (cl:make-instance 'geometry_msgs-msg:Pose)))
)

(cl:defclass qrcode_pose (<qrcode_pose>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <qrcode_pose>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'qrcode_pose)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<qrcode_pose> is deprecated: use comm_msg-msg:qrcode_pose instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <qrcode_pose>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:header-val is deprecated.  Use comm_msg-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'id-val :lambda-list '(m))
(cl:defmethod id-val ((m <qrcode_pose>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:id-val is deprecated.  Use comm_msg-msg:id instead.")
  (id m))

(cl:ensure-generic-function 'pose-val :lambda-list '(m))
(cl:defmethod pose-val ((m <qrcode_pose>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:pose-val is deprecated.  Use comm_msg-msg:pose instead.")
  (pose m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <qrcode_pose>) ostream)
  "Serializes a message object of type '<qrcode_pose>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:let* ((signed (cl:slot-value msg 'id)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'pose) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <qrcode_pose>) istream)
  "Deserializes a message object of type '<qrcode_pose>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'id) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'pose) istream)
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<qrcode_pose>)))
  "Returns string type for a message object of type '<qrcode_pose>"
  "comm_msg/qrcode_pose")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'qrcode_pose)))
  "Returns string type for a message object of type 'qrcode_pose"
  "comm_msg/qrcode_pose")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<qrcode_pose>)))
  "Returns md5sum for a message object of type '<qrcode_pose>"
  "7959a9b3fd2053f3de440accefb2edd4")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'qrcode_pose)))
  "Returns md5sum for a message object of type 'qrcode_pose"
  "7959a9b3fd2053f3de440accefb2edd4")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<qrcode_pose>)))
  "Returns full string definition for message of type '<qrcode_pose>"
  (cl:format cl:nil "Header              header~%int32               id~%geometry_msgs/Pose  pose~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%================================================================================~%MSG: geometry_msgs/Pose~%# A representation of pose in free space, composed of position and orientation. ~%Point position~%Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'qrcode_pose)))
  "Returns full string definition for message of type 'qrcode_pose"
  (cl:format cl:nil "Header              header~%int32               id~%geometry_msgs/Pose  pose~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%================================================================================~%MSG: geometry_msgs/Pose~%# A representation of pose in free space, composed of position and orientation. ~%Point position~%Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <qrcode_pose>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     4
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'pose))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <qrcode_pose>))
  "Converts a ROS message object to a list"
  (cl:list 'qrcode_pose
    (cl:cons ':header (header msg))
    (cl:cons ':id (id msg))
    (cl:cons ':pose (pose msg))
))
