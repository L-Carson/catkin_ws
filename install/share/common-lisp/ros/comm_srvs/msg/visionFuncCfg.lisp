; Auto-generated. Do not edit!


(cl:in-package comm_srvs-msg)


;//! \htmlinclude visionFuncCfg.msg.html

(cl:defclass <visionFuncCfg> (roslisp-msg-protocol:ros-message)
  ((funcId
    :reader funcId
    :initarg :funcId
    :type cl:fixnum
    :initform 0)
   (ipu
    :reader ipu
    :initarg :ipu
    :type cl:boolean
    :initform cl:nil)
   (hz
    :reader hz
    :initarg :hz
    :type cl:fixnum
    :initform 0))
)

(cl:defclass visionFuncCfg (<visionFuncCfg>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <visionFuncCfg>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'visionFuncCfg)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_srvs-msg:<visionFuncCfg> is deprecated: use comm_srvs-msg:visionFuncCfg instead.")))

(cl:ensure-generic-function 'funcId-val :lambda-list '(m))
(cl:defmethod funcId-val ((m <visionFuncCfg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-msg:funcId-val is deprecated.  Use comm_srvs-msg:funcId instead.")
  (funcId m))

(cl:ensure-generic-function 'ipu-val :lambda-list '(m))
(cl:defmethod ipu-val ((m <visionFuncCfg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-msg:ipu-val is deprecated.  Use comm_srvs-msg:ipu instead.")
  (ipu m))

(cl:ensure-generic-function 'hz-val :lambda-list '(m))
(cl:defmethod hz-val ((m <visionFuncCfg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-msg:hz-val is deprecated.  Use comm_srvs-msg:hz instead.")
  (hz m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <visionFuncCfg>) ostream)
  "Serializes a message object of type '<visionFuncCfg>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'funcId)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'ipu) 1 0)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'hz)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <visionFuncCfg>) istream)
  "Deserializes a message object of type '<visionFuncCfg>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'funcId)) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'ipu) (cl:not (cl:zerop (cl:read-byte istream))))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'hz)) (cl:read-byte istream))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<visionFuncCfg>)))
  "Returns string type for a message object of type '<visionFuncCfg>"
  "comm_srvs/visionFuncCfg")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'visionFuncCfg)))
  "Returns string type for a message object of type 'visionFuncCfg"
  "comm_srvs/visionFuncCfg")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<visionFuncCfg>)))
  "Returns md5sum for a message object of type '<visionFuncCfg>"
  "788b771f7f7a06c374a8246119139c2d")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'visionFuncCfg)))
  "Returns md5sum for a message object of type 'visionFuncCfg"
  "788b771f7f7a06c374a8246119139c2d")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<visionFuncCfg>)))
  "Returns full string definition for message of type '<visionFuncCfg>"
  (cl:format cl:nil "uint8   funcId      # 功能ID~%bool    ipu         # 是否使用图像处理单元~%uint8   hz          # 执行频率~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'visionFuncCfg)))
  "Returns full string definition for message of type 'visionFuncCfg"
  (cl:format cl:nil "uint8   funcId      # 功能ID~%bool    ipu         # 是否使用图像处理单元~%uint8   hz          # 执行频率~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <visionFuncCfg>))
  (cl:+ 0
     1
     1
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <visionFuncCfg>))
  "Converts a ROS message object to a list"
  (cl:list 'visionFuncCfg
    (cl:cons ':funcId (funcId msg))
    (cl:cons ':ipu (ipu msg))
    (cl:cons ':hz (hz msg))
))
