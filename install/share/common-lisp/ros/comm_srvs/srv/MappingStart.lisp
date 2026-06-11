; Auto-generated. Do not edit!


(cl:in-package comm_srvs-srv)


;//! \htmlinclude MappingStart-request.msg.html

(cl:defclass <MappingStart-request> (roslisp-msg-protocol:ros-message)
  ((mode
    :reader mode
    :initarg :mode
    :type cl:integer
    :initform 0)
   (state
    :reader state
    :initarg :state
    :type cl:integer
    :initform 0)
   (frontEndMethod
    :reader frontEndMethod
    :initarg :frontEndMethod
    :type cl:integer
    :initform 0)
   (currFrameLeafSize
    :reader currFrameLeafSize
    :initarg :currFrameLeafSize
    :type cl:float
    :initform 0.0)
   (localMapLeafSize
    :reader localMapLeafSize
    :initarg :localMapLeafSize
    :type cl:float
    :initform 0.0)
   (localMapWindowSize
    :reader localMapWindowSize
    :initarg :localMapWindowSize
    :type cl:integer
    :initform 0)
   (loopDetectDist
    :reader loopDetectDist
    :initarg :loopDetectDist
    :type cl:float
    :initform 0.0)
   (cfgDir
    :reader cfgDir
    :initarg :cfgDir
    :type cl:string
    :initform "")
   (dstDir
    :reader dstDir
    :initarg :dstDir
    :type cl:string
    :initform "")
   (bagPath
    :reader bagPath
    :initarg :bagPath
    :type cl:string
    :initform "")
   (srcDir
    :reader srcDir
    :initarg :srcDir
    :type cl:string
    :initform ""))
)

(cl:defclass MappingStart-request (<MappingStart-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <MappingStart-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'MappingStart-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_srvs-srv:<MappingStart-request> is deprecated: use comm_srvs-srv:MappingStart-request instead.")))

(cl:ensure-generic-function 'mode-val :lambda-list '(m))
(cl:defmethod mode-val ((m <MappingStart-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:mode-val is deprecated.  Use comm_srvs-srv:mode instead.")
  (mode m))

(cl:ensure-generic-function 'state-val :lambda-list '(m))
(cl:defmethod state-val ((m <MappingStart-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:state-val is deprecated.  Use comm_srvs-srv:state instead.")
  (state m))

(cl:ensure-generic-function 'frontEndMethod-val :lambda-list '(m))
(cl:defmethod frontEndMethod-val ((m <MappingStart-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:frontEndMethod-val is deprecated.  Use comm_srvs-srv:frontEndMethod instead.")
  (frontEndMethod m))

(cl:ensure-generic-function 'currFrameLeafSize-val :lambda-list '(m))
(cl:defmethod currFrameLeafSize-val ((m <MappingStart-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:currFrameLeafSize-val is deprecated.  Use comm_srvs-srv:currFrameLeafSize instead.")
  (currFrameLeafSize m))

(cl:ensure-generic-function 'localMapLeafSize-val :lambda-list '(m))
(cl:defmethod localMapLeafSize-val ((m <MappingStart-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:localMapLeafSize-val is deprecated.  Use comm_srvs-srv:localMapLeafSize instead.")
  (localMapLeafSize m))

(cl:ensure-generic-function 'localMapWindowSize-val :lambda-list '(m))
(cl:defmethod localMapWindowSize-val ((m <MappingStart-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:localMapWindowSize-val is deprecated.  Use comm_srvs-srv:localMapWindowSize instead.")
  (localMapWindowSize m))

(cl:ensure-generic-function 'loopDetectDist-val :lambda-list '(m))
(cl:defmethod loopDetectDist-val ((m <MappingStart-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:loopDetectDist-val is deprecated.  Use comm_srvs-srv:loopDetectDist instead.")
  (loopDetectDist m))

(cl:ensure-generic-function 'cfgDir-val :lambda-list '(m))
(cl:defmethod cfgDir-val ((m <MappingStart-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:cfgDir-val is deprecated.  Use comm_srvs-srv:cfgDir instead.")
  (cfgDir m))

(cl:ensure-generic-function 'dstDir-val :lambda-list '(m))
(cl:defmethod dstDir-val ((m <MappingStart-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:dstDir-val is deprecated.  Use comm_srvs-srv:dstDir instead.")
  (dstDir m))

(cl:ensure-generic-function 'bagPath-val :lambda-list '(m))
(cl:defmethod bagPath-val ((m <MappingStart-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:bagPath-val is deprecated.  Use comm_srvs-srv:bagPath instead.")
  (bagPath m))

(cl:ensure-generic-function 'srcDir-val :lambda-list '(m))
(cl:defmethod srcDir-val ((m <MappingStart-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:srcDir-val is deprecated.  Use comm_srvs-srv:srcDir instead.")
  (srcDir m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <MappingStart-request>) ostream)
  "Serializes a message object of type '<MappingStart-request>"
  (cl:let* ((signed (cl:slot-value msg 'mode)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'state)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'frontEndMethod)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'currFrameLeafSize))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'localMapLeafSize))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let* ((signed (cl:slot-value msg 'localMapWindowSize)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'loopDetectDist))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'cfgDir))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'cfgDir))
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'dstDir))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'dstDir))
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'bagPath))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'bagPath))
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'srcDir))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'srcDir))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <MappingStart-request>) istream)
  "Deserializes a message object of type '<MappingStart-request>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'mode) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'state) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'frontEndMethod) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'currFrameLeafSize) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'localMapLeafSize) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'localMapWindowSize) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'loopDetectDist) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'cfgDir) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'cfgDir) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'dstDir) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'dstDir) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'bagPath) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'bagPath) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'srcDir) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'srcDir) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<MappingStart-request>)))
  "Returns string type for a service object of type '<MappingStart-request>"
  "comm_srvs/MappingStartRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'MappingStart-request)))
  "Returns string type for a service object of type 'MappingStart-request"
  "comm_srvs/MappingStartRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<MappingStart-request>)))
  "Returns md5sum for a message object of type '<MappingStart-request>"
  "50d318bcef824d9f76f338ae7c891086")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'MappingStart-request)))
  "Returns md5sum for a message object of type 'MappingStart-request"
  "50d318bcef824d9f76f338ae7c891086")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<MappingStart-request>)))
  "Returns full string definition for message of type '<MappingStart-request>"
  (cl:format cl:nil "int32   mode~%int32   state~%int32   frontEndMethod~%float32 currFrameLeafSize~%float32 localMapLeafSize  ~%int32   localMapWindowSize ~%float32 loopDetectDist ~%string  cfgDir~%string  dstDir~%string  bagPath~%string  srcDir~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'MappingStart-request)))
  "Returns full string definition for message of type 'MappingStart-request"
  (cl:format cl:nil "int32   mode~%int32   state~%int32   frontEndMethod~%float32 currFrameLeafSize~%float32 localMapLeafSize  ~%int32   localMapWindowSize ~%float32 loopDetectDist ~%string  cfgDir~%string  dstDir~%string  bagPath~%string  srcDir~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <MappingStart-request>))
  (cl:+ 0
     4
     4
     4
     4
     4
     4
     4
     4 (cl:length (cl:slot-value msg 'cfgDir))
     4 (cl:length (cl:slot-value msg 'dstDir))
     4 (cl:length (cl:slot-value msg 'bagPath))
     4 (cl:length (cl:slot-value msg 'srcDir))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <MappingStart-request>))
  "Converts a ROS message object to a list"
  (cl:list 'MappingStart-request
    (cl:cons ':mode (mode msg))
    (cl:cons ':state (state msg))
    (cl:cons ':frontEndMethod (frontEndMethod msg))
    (cl:cons ':currFrameLeafSize (currFrameLeafSize msg))
    (cl:cons ':localMapLeafSize (localMapLeafSize msg))
    (cl:cons ':localMapWindowSize (localMapWindowSize msg))
    (cl:cons ':loopDetectDist (loopDetectDist msg))
    (cl:cons ':cfgDir (cfgDir msg))
    (cl:cons ':dstDir (dstDir msg))
    (cl:cons ':bagPath (bagPath msg))
    (cl:cons ':srcDir (srcDir msg))
))
;//! \htmlinclude MappingStart-response.msg.html

(cl:defclass <MappingStart-response> (roslisp-msg-protocol:ros-message)
  ((success
    :reader success
    :initarg :success
    :type cl:boolean
    :initform cl:nil)
   (message
    :reader message
    :initarg :message
    :type cl:string
    :initform ""))
)

(cl:defclass MappingStart-response (<MappingStart-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <MappingStart-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'MappingStart-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_srvs-srv:<MappingStart-response> is deprecated: use comm_srvs-srv:MappingStart-response instead.")))

(cl:ensure-generic-function 'success-val :lambda-list '(m))
(cl:defmethod success-val ((m <MappingStart-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:success-val is deprecated.  Use comm_srvs-srv:success instead.")
  (success m))

(cl:ensure-generic-function 'message-val :lambda-list '(m))
(cl:defmethod message-val ((m <MappingStart-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:message-val is deprecated.  Use comm_srvs-srv:message instead.")
  (message m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <MappingStart-response>) ostream)
  "Serializes a message object of type '<MappingStart-response>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'success) 1 0)) ostream)
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'message))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'message))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <MappingStart-response>) istream)
  "Deserializes a message object of type '<MappingStart-response>"
    (cl:setf (cl:slot-value msg 'success) (cl:not (cl:zerop (cl:read-byte istream))))
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'message) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'message) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<MappingStart-response>)))
  "Returns string type for a service object of type '<MappingStart-response>"
  "comm_srvs/MappingStartResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'MappingStart-response)))
  "Returns string type for a service object of type 'MappingStart-response"
  "comm_srvs/MappingStartResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<MappingStart-response>)))
  "Returns md5sum for a message object of type '<MappingStart-response>"
  "50d318bcef824d9f76f338ae7c891086")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'MappingStart-response)))
  "Returns md5sum for a message object of type 'MappingStart-response"
  "50d318bcef824d9f76f338ae7c891086")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<MappingStart-response>)))
  "Returns full string definition for message of type '<MappingStart-response>"
  (cl:format cl:nil "bool   success~%string message~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'MappingStart-response)))
  "Returns full string definition for message of type 'MappingStart-response"
  (cl:format cl:nil "bool   success~%string message~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <MappingStart-response>))
  (cl:+ 0
     1
     4 (cl:length (cl:slot-value msg 'message))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <MappingStart-response>))
  "Converts a ROS message object to a list"
  (cl:list 'MappingStart-response
    (cl:cons ':success (success msg))
    (cl:cons ':message (message msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'MappingStart)))
  'MappingStart-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'MappingStart)))
  'MappingStart-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'MappingStart)))
  "Returns string type for a service object of type '<MappingStart>"
  "comm_srvs/MappingStart")