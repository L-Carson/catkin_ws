; Auto-generated. Do not edit!


(cl:in-package comm_srvs-srv)


;//! \htmlinclude visionFunc-request.msg.html

(cl:defclass <visionFunc-request> (roslisp-msg-protocol:ros-message)
  ()
)

(cl:defclass visionFunc-request (<visionFunc-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <visionFunc-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'visionFunc-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_srvs-srv:<visionFunc-request> is deprecated: use comm_srvs-srv:visionFunc-request instead.")))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <visionFunc-request>) ostream)
  "Serializes a message object of type '<visionFunc-request>"
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <visionFunc-request>) istream)
  "Deserializes a message object of type '<visionFunc-request>"
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<visionFunc-request>)))
  "Returns string type for a service object of type '<visionFunc-request>"
  "comm_srvs/visionFuncRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'visionFunc-request)))
  "Returns string type for a service object of type 'visionFunc-request"
  "comm_srvs/visionFuncRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<visionFunc-request>)))
  "Returns md5sum for a message object of type '<visionFunc-request>"
  "adf5e73fc1530eec7e82d92a5cfadc9f")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'visionFunc-request)))
  "Returns md5sum for a message object of type 'visionFunc-request"
  "adf5e73fc1530eec7e82d92a5cfadc9f")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<visionFunc-request>)))
  "Returns full string definition for message of type '<visionFunc-request>"
  (cl:format cl:nil "~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'visionFunc-request)))
  "Returns full string definition for message of type 'visionFunc-request"
  (cl:format cl:nil "~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <visionFunc-request>))
  (cl:+ 0
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <visionFunc-request>))
  "Converts a ROS message object to a list"
  (cl:list 'visionFunc-request
))
;//! \htmlinclude visionFunc-response.msg.html

(cl:defclass <visionFunc-response> (roslisp-msg-protocol:ros-message)
  ((success
    :reader success
    :initarg :success
    :type cl:boolean
    :initform cl:nil)
   (isUseApi
    :reader isUseApi
    :initarg :isUseApi
    :type cl:boolean
    :initform cl:nil)
   (visionFuncCfgArray
    :reader visionFuncCfgArray
    :initarg :visionFuncCfgArray
    :type (cl:vector comm_srvs-msg:visionFuncCfg)
   :initform (cl:make-array 0 :element-type 'comm_srvs-msg:visionFuncCfg :initial-element (cl:make-instance 'comm_srvs-msg:visionFuncCfg)))
   (visionLabelCfg
    :reader visionLabelCfg
    :initarg :visionLabelCfg
    :type comm_srvs-msg:visionLabelCfg
    :initform (cl:make-instance 'comm_srvs-msg:visionLabelCfg)))
)

(cl:defclass visionFunc-response (<visionFunc-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <visionFunc-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'visionFunc-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_srvs-srv:<visionFunc-response> is deprecated: use comm_srvs-srv:visionFunc-response instead.")))

(cl:ensure-generic-function 'success-val :lambda-list '(m))
(cl:defmethod success-val ((m <visionFunc-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:success-val is deprecated.  Use comm_srvs-srv:success instead.")
  (success m))

(cl:ensure-generic-function 'isUseApi-val :lambda-list '(m))
(cl:defmethod isUseApi-val ((m <visionFunc-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:isUseApi-val is deprecated.  Use comm_srvs-srv:isUseApi instead.")
  (isUseApi m))

(cl:ensure-generic-function 'visionFuncCfgArray-val :lambda-list '(m))
(cl:defmethod visionFuncCfgArray-val ((m <visionFunc-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:visionFuncCfgArray-val is deprecated.  Use comm_srvs-srv:visionFuncCfgArray instead.")
  (visionFuncCfgArray m))

(cl:ensure-generic-function 'visionLabelCfg-val :lambda-list '(m))
(cl:defmethod visionLabelCfg-val ((m <visionFunc-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:visionLabelCfg-val is deprecated.  Use comm_srvs-srv:visionLabelCfg instead.")
  (visionLabelCfg m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <visionFunc-response>) ostream)
  "Serializes a message object of type '<visionFunc-response>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'success) 1 0)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'isUseApi) 1 0)) ostream)
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'visionFuncCfgArray))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'visionFuncCfgArray))
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'visionLabelCfg) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <visionFunc-response>) istream)
  "Deserializes a message object of type '<visionFunc-response>"
    (cl:setf (cl:slot-value msg 'success) (cl:not (cl:zerop (cl:read-byte istream))))
    (cl:setf (cl:slot-value msg 'isUseApi) (cl:not (cl:zerop (cl:read-byte istream))))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'visionFuncCfgArray) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'visionFuncCfgArray)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'comm_srvs-msg:visionFuncCfg))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'visionLabelCfg) istream)
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<visionFunc-response>)))
  "Returns string type for a service object of type '<visionFunc-response>"
  "comm_srvs/visionFuncResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'visionFunc-response)))
  "Returns string type for a service object of type 'visionFunc-response"
  "comm_srvs/visionFuncResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<visionFunc-response>)))
  "Returns md5sum for a message object of type '<visionFunc-response>"
  "adf5e73fc1530eec7e82d92a5cfadc9f")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'visionFunc-response)))
  "Returns md5sum for a message object of type 'visionFunc-response"
  "adf5e73fc1530eec7e82d92a5cfadc9f")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<visionFunc-response>)))
  "Returns full string definition for message of type '<visionFunc-response>"
  (cl:format cl:nil "bool                success~%bool                isUseApi~%visionFuncCfg[]     visionFuncCfgArray~%visionLabelCfg      visionLabelCfg~%~%================================================================================~%MSG: comm_srvs/visionFuncCfg~%uint8   funcId      # 功能ID~%bool    ipu         # 是否使用图像处理单元~%uint8   hz          # 执行频率~%================================================================================~%MSG: comm_srvs/visionLabelCfg~%uint32[] vectDetLabel~%uint32[] vectSegLabel~%~%bool    isSupportThickPipe #是否支持粗水管类别~%float32 thickPipeWidth     #粗水管宽度阈值~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'visionFunc-response)))
  "Returns full string definition for message of type 'visionFunc-response"
  (cl:format cl:nil "bool                success~%bool                isUseApi~%visionFuncCfg[]     visionFuncCfgArray~%visionLabelCfg      visionLabelCfg~%~%================================================================================~%MSG: comm_srvs/visionFuncCfg~%uint8   funcId      # 功能ID~%bool    ipu         # 是否使用图像处理单元~%uint8   hz          # 执行频率~%================================================================================~%MSG: comm_srvs/visionLabelCfg~%uint32[] vectDetLabel~%uint32[] vectSegLabel~%~%bool    isSupportThickPipe #是否支持粗水管类别~%float32 thickPipeWidth     #粗水管宽度阈值~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <visionFunc-response>))
  (cl:+ 0
     1
     1
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'visionFuncCfgArray) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'visionLabelCfg))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <visionFunc-response>))
  "Converts a ROS message object to a list"
  (cl:list 'visionFunc-response
    (cl:cons ':success (success msg))
    (cl:cons ':isUseApi (isUseApi msg))
    (cl:cons ':visionFuncCfgArray (visionFuncCfgArray msg))
    (cl:cons ':visionLabelCfg (visionLabelCfg msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'visionFunc)))
  'visionFunc-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'visionFunc)))
  'visionFunc-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'visionFunc)))
  "Returns string type for a service object of type '<visionFunc>"
  "comm_srvs/visionFunc")