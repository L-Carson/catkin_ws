; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude move_info.msg.html

(cl:defclass <move_info> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (steeringAngle
    :reader steeringAngle
    :initarg :steeringAngle
    :type cl:float
    :initform 0.0)
   (moveLinearVel
    :reader moveLinearVel
    :initarg :moveLinearVel
    :type cl:float
    :initform 0.0)
   (yawAngle
    :reader yawAngle
    :initarg :yawAngle
    :type cl:float
    :initform 0.0)
   (yawAngleVel
    :reader yawAngleVel
    :initarg :yawAngleVel
    :type cl:float
    :initform 0.0)
   (accX
    :reader accX
    :initarg :accX
    :type cl:float
    :initform 0.0)
   (accY
    :reader accY
    :initarg :accY
    :type cl:float
    :initform 0.0)
   (accZ
    :reader accZ
    :initarg :accZ
    :type cl:float
    :initform 0.0))
)

(cl:defclass move_info (<move_info>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <move_info>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'move_info)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<move_info> is deprecated: use comm_msg-msg:move_info instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <move_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:header-val is deprecated.  Use comm_msg-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'steeringAngle-val :lambda-list '(m))
(cl:defmethod steeringAngle-val ((m <move_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:steeringAngle-val is deprecated.  Use comm_msg-msg:steeringAngle instead.")
  (steeringAngle m))

(cl:ensure-generic-function 'moveLinearVel-val :lambda-list '(m))
(cl:defmethod moveLinearVel-val ((m <move_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:moveLinearVel-val is deprecated.  Use comm_msg-msg:moveLinearVel instead.")
  (moveLinearVel m))

(cl:ensure-generic-function 'yawAngle-val :lambda-list '(m))
(cl:defmethod yawAngle-val ((m <move_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:yawAngle-val is deprecated.  Use comm_msg-msg:yawAngle instead.")
  (yawAngle m))

(cl:ensure-generic-function 'yawAngleVel-val :lambda-list '(m))
(cl:defmethod yawAngleVel-val ((m <move_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:yawAngleVel-val is deprecated.  Use comm_msg-msg:yawAngleVel instead.")
  (yawAngleVel m))

(cl:ensure-generic-function 'accX-val :lambda-list '(m))
(cl:defmethod accX-val ((m <move_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:accX-val is deprecated.  Use comm_msg-msg:accX instead.")
  (accX m))

(cl:ensure-generic-function 'accY-val :lambda-list '(m))
(cl:defmethod accY-val ((m <move_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:accY-val is deprecated.  Use comm_msg-msg:accY instead.")
  (accY m))

(cl:ensure-generic-function 'accZ-val :lambda-list '(m))
(cl:defmethod accZ-val ((m <move_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:accZ-val is deprecated.  Use comm_msg-msg:accZ instead.")
  (accZ m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <move_info>) ostream)
  "Serializes a message object of type '<move_info>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'steeringAngle))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'moveLinearVel))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'yawAngle))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'yawAngleVel))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'accX))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'accY))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'accZ))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <move_info>) istream)
  "Deserializes a message object of type '<move_info>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'steeringAngle) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'moveLinearVel) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'yawAngle) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'yawAngleVel) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'accX) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'accY) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'accZ) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<move_info>)))
  "Returns string type for a message object of type '<move_info>"
  "comm_msg/move_info")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'move_info)))
  "Returns string type for a message object of type 'move_info"
  "comm_msg/move_info")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<move_info>)))
  "Returns md5sum for a message object of type '<move_info>"
  "5651dd8f0d48de47c0d8061b96a98beb")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'move_info)))
  "Returns md5sum for a message object of type 'move_info"
  "5651dd8f0d48de47c0d8061b96a98beb")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<move_info>)))
  "Returns full string definition for message of type '<move_info>"
  (cl:format cl:nil "Header  header~%~%float32 steeringAngle~%float32 moveLinearVel~%float32 yawAngle~%float32 yawAngleVel~%float32 accX~%float32 accY~%float32 accZ~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'move_info)))
  "Returns full string definition for message of type 'move_info"
  (cl:format cl:nil "Header  header~%~%float32 steeringAngle~%float32 moveLinearVel~%float32 yawAngle~%float32 yawAngleVel~%float32 accX~%float32 accY~%float32 accZ~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <move_info>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     4
     4
     4
     4
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <move_info>))
  "Converts a ROS message object to a list"
  (cl:list 'move_info
    (cl:cons ':header (header msg))
    (cl:cons ':steeringAngle (steeringAngle msg))
    (cl:cons ':moveLinearVel (moveLinearVel msg))
    (cl:cons ':yawAngle (yawAngle msg))
    (cl:cons ':yawAngleVel (yawAngleVel msg))
    (cl:cons ':accX (accX msg))
    (cl:cons ':accY (accY msg))
    (cl:cons ':accZ (accZ msg))
))
