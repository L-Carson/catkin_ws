; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude locationInfo.msg.html

(cl:defclass <locationInfo> (roslisp-msg-protocol:ros-message)
  ((status
    :reader status
    :initarg :status
    :type cl:integer
    :initform 0)
   (subMapNo
    :reader subMapNo
    :initarg :subMapNo
    :type cl:integer
    :initform 0)
   (posX
    :reader posX
    :initarg :posX
    :type cl:float
    :initform 0.0)
   (posY
    :reader posY
    :initarg :posY
    :type cl:float
    :initform 0.0)
   (dirAngle
    :reader dirAngle
    :initarg :dirAngle
    :type cl:float
    :initform 0.0))
)

(cl:defclass locationInfo (<locationInfo>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <locationInfo>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'locationInfo)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<locationInfo> is deprecated: use comm_msg-msg:locationInfo instead.")))

(cl:ensure-generic-function 'status-val :lambda-list '(m))
(cl:defmethod status-val ((m <locationInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:status-val is deprecated.  Use comm_msg-msg:status instead.")
  (status m))

(cl:ensure-generic-function 'subMapNo-val :lambda-list '(m))
(cl:defmethod subMapNo-val ((m <locationInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:subMapNo-val is deprecated.  Use comm_msg-msg:subMapNo instead.")
  (subMapNo m))

(cl:ensure-generic-function 'posX-val :lambda-list '(m))
(cl:defmethod posX-val ((m <locationInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:posX-val is deprecated.  Use comm_msg-msg:posX instead.")
  (posX m))

(cl:ensure-generic-function 'posY-val :lambda-list '(m))
(cl:defmethod posY-val ((m <locationInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:posY-val is deprecated.  Use comm_msg-msg:posY instead.")
  (posY m))

(cl:ensure-generic-function 'dirAngle-val :lambda-list '(m))
(cl:defmethod dirAngle-val ((m <locationInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:dirAngle-val is deprecated.  Use comm_msg-msg:dirAngle instead.")
  (dirAngle m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <locationInfo>) ostream)
  "Serializes a message object of type '<locationInfo>"
  (cl:let* ((signed (cl:slot-value msg 'status)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'subMapNo)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
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
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'dirAngle))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <locationInfo>) istream)
  "Deserializes a message object of type '<locationInfo>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'status) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'subMapNo) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
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
    (cl:setf (cl:slot-value msg 'dirAngle) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<locationInfo>)))
  "Returns string type for a message object of type '<locationInfo>"
  "comm_msg/locationInfo")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'locationInfo)))
  "Returns string type for a message object of type 'locationInfo"
  "comm_msg/locationInfo")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<locationInfo>)))
  "Returns md5sum for a message object of type '<locationInfo>"
  "b3a48261c7b0aa3907b3479915747d21")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'locationInfo)))
  "Returns md5sum for a message object of type 'locationInfo"
  "b3a48261c7b0aa3907b3479915747d21")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<locationInfo>)))
  "Returns full string definition for message of type '<locationInfo>"
  (cl:format cl:nil "# location status, refer to MAP_NODE_STATE_ENUM~%int32 status~%~%# sub map no, this is valid when >= 0, if -ing status, this is the new sub map no~%int32 subMapNo~%~%# Comb Map pose, which is valid When status is > 0.~%float32 posX~%float32 posY~%float32 dirAngle~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'locationInfo)))
  "Returns full string definition for message of type 'locationInfo"
  (cl:format cl:nil "# location status, refer to MAP_NODE_STATE_ENUM~%int32 status~%~%# sub map no, this is valid when >= 0, if -ing status, this is the new sub map no~%int32 subMapNo~%~%# Comb Map pose, which is valid When status is > 0.~%float32 posX~%float32 posY~%float32 dirAngle~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <locationInfo>))
  (cl:+ 0
     4
     4
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <locationInfo>))
  "Converts a ROS message object to a list"
  (cl:list 'locationInfo
    (cl:cons ':status (status msg))
    (cl:cons ':subMapNo (subMapNo msg))
    (cl:cons ':posX (posX msg))
    (cl:cons ':posY (posY msg))
    (cl:cons ':dirAngle (dirAngle msg))
))
