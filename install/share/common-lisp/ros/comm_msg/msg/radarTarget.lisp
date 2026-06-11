; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude radarTarget.msg.html

(cl:defclass <radarTarget> (roslisp-msg-protocol:ros-message)
  ((targetDistx
    :reader targetDistx
    :initarg :targetDistx
    :type cl:float
    :initform 0.0)
   (targetDisty
    :reader targetDisty
    :initarg :targetDisty
    :type cl:float
    :initform 0.0)
   (targetVelx
    :reader targetVelx
    :initarg :targetVelx
    :type cl:float
    :initform 0.0)
   (targetVely
    :reader targetVely
    :initarg :targetVely
    :type cl:float
    :initform 0.0))
)

(cl:defclass radarTarget (<radarTarget>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <radarTarget>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'radarTarget)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<radarTarget> is deprecated: use comm_msg-msg:radarTarget instead.")))

(cl:ensure-generic-function 'targetDistx-val :lambda-list '(m))
(cl:defmethod targetDistx-val ((m <radarTarget>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:targetDistx-val is deprecated.  Use comm_msg-msg:targetDistx instead.")
  (targetDistx m))

(cl:ensure-generic-function 'targetDisty-val :lambda-list '(m))
(cl:defmethod targetDisty-val ((m <radarTarget>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:targetDisty-val is deprecated.  Use comm_msg-msg:targetDisty instead.")
  (targetDisty m))

(cl:ensure-generic-function 'targetVelx-val :lambda-list '(m))
(cl:defmethod targetVelx-val ((m <radarTarget>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:targetVelx-val is deprecated.  Use comm_msg-msg:targetVelx instead.")
  (targetVelx m))

(cl:ensure-generic-function 'targetVely-val :lambda-list '(m))
(cl:defmethod targetVely-val ((m <radarTarget>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:targetVely-val is deprecated.  Use comm_msg-msg:targetVely instead.")
  (targetVely m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <radarTarget>) ostream)
  "Serializes a message object of type '<radarTarget>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'targetDistx))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'targetDisty))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'targetVelx))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'targetVely))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <radarTarget>) istream)
  "Deserializes a message object of type '<radarTarget>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'targetDistx) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'targetDisty) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'targetVelx) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'targetVely) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<radarTarget>)))
  "Returns string type for a message object of type '<radarTarget>"
  "comm_msg/radarTarget")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'radarTarget)))
  "Returns string type for a message object of type 'radarTarget"
  "comm_msg/radarTarget")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<radarTarget>)))
  "Returns md5sum for a message object of type '<radarTarget>"
  "2f0ce27dc024af9302b335afb3187795")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'radarTarget)))
  "Returns md5sum for a message object of type 'radarTarget"
  "2f0ce27dc024af9302b335afb3187795")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<radarTarget>)))
  "Returns full string definition for message of type '<radarTarget>"
  (cl:format cl:nil "float32   targetDistx         # 目标相对于清扫车参考坐标系X轴坐标~%float32   targetDisty         # 目标相对于清扫车参考坐标系Y轴坐标~%float32   targetVelx          # 目标相对于清扫车参考坐标系X轴速度~%float32   targetVely          # 目标相对于清扫车参考坐标系Y轴速度~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'radarTarget)))
  "Returns full string definition for message of type 'radarTarget"
  (cl:format cl:nil "float32   targetDistx         # 目标相对于清扫车参考坐标系X轴坐标~%float32   targetDisty         # 目标相对于清扫车参考坐标系Y轴坐标~%float32   targetVelx          # 目标相对于清扫车参考坐标系X轴速度~%float32   targetVely          # 目标相对于清扫车参考坐标系Y轴速度~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <radarTarget>))
  (cl:+ 0
     4
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <radarTarget>))
  "Converts a ROS message object to a list"
  (cl:list 'radarTarget
    (cl:cons ':targetDistx (targetDistx msg))
    (cl:cons ':targetDisty (targetDisty msg))
    (cl:cons ':targetVelx (targetVelx msg))
    (cl:cons ':targetVely (targetVely msg))
))
