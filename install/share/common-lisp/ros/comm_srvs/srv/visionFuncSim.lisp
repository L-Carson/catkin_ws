; Auto-generated. Do not edit!


(cl:in-package comm_srvs-srv)


;//! \htmlinclude visionFuncSim-request.msg.html

(cl:defclass <visionFuncSim-request> (roslisp-msg-protocol:ros-message)
  ((qrCodeId
    :reader qrCodeId
    :initarg :qrCodeId
    :type cl:fixnum
    :initform 0)
   (funcId
    :reader funcId
    :initarg :funcId
    :type cl:fixnum
    :initform 0)
   (isUseIpu
    :reader isUseIpu
    :initarg :isUseIpu
    :type cl:boolean
    :initform cl:nil)
   (hz
    :reader hz
    :initarg :hz
    :type cl:fixnum
    :initform 0)
   (camera
    :reader camera
    :initarg :camera
    :type cl:string
    :initform ""))
)

(cl:defclass visionFuncSim-request (<visionFuncSim-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <visionFuncSim-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'visionFuncSim-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_srvs-srv:<visionFuncSim-request> is deprecated: use comm_srvs-srv:visionFuncSim-request instead.")))

(cl:ensure-generic-function 'qrCodeId-val :lambda-list '(m))
(cl:defmethod qrCodeId-val ((m <visionFuncSim-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:qrCodeId-val is deprecated.  Use comm_srvs-srv:qrCodeId instead.")
  (qrCodeId m))

(cl:ensure-generic-function 'funcId-val :lambda-list '(m))
(cl:defmethod funcId-val ((m <visionFuncSim-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:funcId-val is deprecated.  Use comm_srvs-srv:funcId instead.")
  (funcId m))

(cl:ensure-generic-function 'isUseIpu-val :lambda-list '(m))
(cl:defmethod isUseIpu-val ((m <visionFuncSim-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:isUseIpu-val is deprecated.  Use comm_srvs-srv:isUseIpu instead.")
  (isUseIpu m))

(cl:ensure-generic-function 'hz-val :lambda-list '(m))
(cl:defmethod hz-val ((m <visionFuncSim-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:hz-val is deprecated.  Use comm_srvs-srv:hz instead.")
  (hz m))

(cl:ensure-generic-function 'camera-val :lambda-list '(m))
(cl:defmethod camera-val ((m <visionFuncSim-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:camera-val is deprecated.  Use comm_srvs-srv:camera instead.")
  (camera m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <visionFuncSim-request>) ostream)
  "Serializes a message object of type '<visionFuncSim-request>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'qrCodeId)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'funcId)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'isUseIpu) 1 0)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'hz)) ostream)
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'camera))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'camera))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <visionFuncSim-request>) istream)
  "Deserializes a message object of type '<visionFuncSim-request>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'qrCodeId)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'funcId)) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'isUseIpu) (cl:not (cl:zerop (cl:read-byte istream))))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'hz)) (cl:read-byte istream))
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'camera) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'camera) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<visionFuncSim-request>)))
  "Returns string type for a service object of type '<visionFuncSim-request>"
  "comm_srvs/visionFuncSimRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'visionFuncSim-request)))
  "Returns string type for a service object of type 'visionFuncSim-request"
  "comm_srvs/visionFuncSimRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<visionFuncSim-request>)))
  "Returns md5sum for a message object of type '<visionFuncSim-request>"
  "0a3c827bca110ade616a89594700a208")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'visionFuncSim-request)))
  "Returns md5sum for a message object of type 'visionFuncSim-request"
  "0a3c827bca110ade616a89594700a208")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<visionFuncSim-request>)))
  "Returns full string definition for message of type '<visionFuncSim-request>"
  (cl:format cl:nil "uint8       qrCodeId~%uint8       funcId~%bool        isUseIpu~%uint8       hz~%string      camera~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'visionFuncSim-request)))
  "Returns full string definition for message of type 'visionFuncSim-request"
  (cl:format cl:nil "uint8       qrCodeId~%uint8       funcId~%bool        isUseIpu~%uint8       hz~%string      camera~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <visionFuncSim-request>))
  (cl:+ 0
     1
     1
     1
     1
     4 (cl:length (cl:slot-value msg 'camera))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <visionFuncSim-request>))
  "Converts a ROS message object to a list"
  (cl:list 'visionFuncSim-request
    (cl:cons ':qrCodeId (qrCodeId msg))
    (cl:cons ':funcId (funcId msg))
    (cl:cons ':isUseIpu (isUseIpu msg))
    (cl:cons ':hz (hz msg))
    (cl:cons ':camera (camera msg))
))
;//! \htmlinclude visionFuncSim-response.msg.html

(cl:defclass <visionFuncSim-response> (roslisp-msg-protocol:ros-message)
  ((success
    :reader success
    :initarg :success
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass visionFuncSim-response (<visionFuncSim-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <visionFuncSim-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'visionFuncSim-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_srvs-srv:<visionFuncSim-response> is deprecated: use comm_srvs-srv:visionFuncSim-response instead.")))

(cl:ensure-generic-function 'success-val :lambda-list '(m))
(cl:defmethod success-val ((m <visionFuncSim-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:success-val is deprecated.  Use comm_srvs-srv:success instead.")
  (success m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <visionFuncSim-response>) ostream)
  "Serializes a message object of type '<visionFuncSim-response>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'success) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <visionFuncSim-response>) istream)
  "Deserializes a message object of type '<visionFuncSim-response>"
    (cl:setf (cl:slot-value msg 'success) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<visionFuncSim-response>)))
  "Returns string type for a service object of type '<visionFuncSim-response>"
  "comm_srvs/visionFuncSimResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'visionFuncSim-response)))
  "Returns string type for a service object of type 'visionFuncSim-response"
  "comm_srvs/visionFuncSimResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<visionFuncSim-response>)))
  "Returns md5sum for a message object of type '<visionFuncSim-response>"
  "0a3c827bca110ade616a89594700a208")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'visionFuncSim-response)))
  "Returns md5sum for a message object of type 'visionFuncSim-response"
  "0a3c827bca110ade616a89594700a208")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<visionFuncSim-response>)))
  "Returns full string definition for message of type '<visionFuncSim-response>"
  (cl:format cl:nil "bool        success~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'visionFuncSim-response)))
  "Returns full string definition for message of type 'visionFuncSim-response"
  (cl:format cl:nil "bool        success~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <visionFuncSim-response>))
  (cl:+ 0
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <visionFuncSim-response>))
  "Converts a ROS message object to a list"
  (cl:list 'visionFuncSim-response
    (cl:cons ':success (success msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'visionFuncSim)))
  'visionFuncSim-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'visionFuncSim)))
  'visionFuncSim-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'visionFuncSim)))
  "Returns string type for a service object of type '<visionFuncSim>"
  "comm_srvs/visionFuncSim")