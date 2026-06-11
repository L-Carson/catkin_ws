; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude pose3D.msg.html

(cl:defclass <pose3D> (roslisp-msg-protocol:ros-message)
  ((posX
    :reader posX
    :initarg :posX
    :type cl:float
    :initform 0.0)
   (posY
    :reader posY
    :initarg :posY
    :type cl:float
    :initform 0.0)
   (posZ
    :reader posZ
    :initarg :posZ
    :type cl:float
    :initform 0.0)
   (roll
    :reader roll
    :initarg :roll
    :type cl:float
    :initform 0.0)
   (pitch
    :reader pitch
    :initarg :pitch
    :type cl:float
    :initform 0.0)
   (yaw
    :reader yaw
    :initarg :yaw
    :type cl:float
    :initform 0.0))
)

(cl:defclass pose3D (<pose3D>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <pose3D>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'pose3D)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<pose3D> is deprecated: use comm_msg-msg:pose3D instead.")))

(cl:ensure-generic-function 'posX-val :lambda-list '(m))
(cl:defmethod posX-val ((m <pose3D>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:posX-val is deprecated.  Use comm_msg-msg:posX instead.")
  (posX m))

(cl:ensure-generic-function 'posY-val :lambda-list '(m))
(cl:defmethod posY-val ((m <pose3D>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:posY-val is deprecated.  Use comm_msg-msg:posY instead.")
  (posY m))

(cl:ensure-generic-function 'posZ-val :lambda-list '(m))
(cl:defmethod posZ-val ((m <pose3D>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:posZ-val is deprecated.  Use comm_msg-msg:posZ instead.")
  (posZ m))

(cl:ensure-generic-function 'roll-val :lambda-list '(m))
(cl:defmethod roll-val ((m <pose3D>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:roll-val is deprecated.  Use comm_msg-msg:roll instead.")
  (roll m))

(cl:ensure-generic-function 'pitch-val :lambda-list '(m))
(cl:defmethod pitch-val ((m <pose3D>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:pitch-val is deprecated.  Use comm_msg-msg:pitch instead.")
  (pitch m))

(cl:ensure-generic-function 'yaw-val :lambda-list '(m))
(cl:defmethod yaw-val ((m <pose3D>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:yaw-val is deprecated.  Use comm_msg-msg:yaw instead.")
  (yaw m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <pose3D>) ostream)
  "Serializes a message object of type '<pose3D>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'posX))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'posY))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'posZ))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'roll))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'pitch))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'yaw))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <pose3D>) istream)
  "Deserializes a message object of type '<pose3D>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'posX) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'posY) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'posZ) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'roll) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'pitch) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'yaw) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<pose3D>)))
  "Returns string type for a message object of type '<pose3D>"
  "comm_msg/pose3D")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'pose3D)))
  "Returns string type for a message object of type 'pose3D"
  "comm_msg/pose3D")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<pose3D>)))
  "Returns md5sum for a message object of type '<pose3D>"
  "b0f6a63f05aca4ae94f3fa6431f6fc26")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'pose3D)))
  "Returns md5sum for a message object of type 'pose3D"
  "b0f6a63f05aca4ae94f3fa6431f6fc26")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<pose3D>)))
  "Returns full string definition for message of type '<pose3D>"
  (cl:format cl:nil "float32 posX~%float32 posY~%float32 posZ~%float32 roll~%float32 pitch~%float32 yaw~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'pose3D)))
  "Returns full string definition for message of type 'pose3D"
  (cl:format cl:nil "float32 posX~%float32 posY~%float32 posZ~%float32 roll~%float32 pitch~%float32 yaw~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <pose3D>))
  (cl:+ 0
     4
     4
     4
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <pose3D>))
  "Converts a ROS message object to a list"
  (cl:list 'pose3D
    (cl:cons ':posX (posX msg))
    (cl:cons ':posY (posY msg))
    (cl:cons ':posZ (posZ msg))
    (cl:cons ':roll (roll msg))
    (cl:cons ':pitch (pitch msg))
    (cl:cons ':yaw (yaw msg))
))
