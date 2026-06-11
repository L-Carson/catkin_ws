; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude module_abnormal_info.msg.html

(cl:defclass <module_abnormal_info> (roslisp-msg-protocol:ros-message)
  ((module
    :reader module
    :initarg :module
    :type cl:integer
    :initform 0)
   (errCode
    :reader errCode
    :initarg :errCode
    :type cl:integer
    :initform 0))
)

(cl:defclass module_abnormal_info (<module_abnormal_info>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <module_abnormal_info>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'module_abnormal_info)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<module_abnormal_info> is deprecated: use comm_msg-msg:module_abnormal_info instead.")))

(cl:ensure-generic-function 'module-val :lambda-list '(m))
(cl:defmethod module-val ((m <module_abnormal_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:module-val is deprecated.  Use comm_msg-msg:module instead.")
  (module m))

(cl:ensure-generic-function 'errCode-val :lambda-list '(m))
(cl:defmethod errCode-val ((m <module_abnormal_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:errCode-val is deprecated.  Use comm_msg-msg:errCode instead.")
  (errCode m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <module_abnormal_info>) ostream)
  "Serializes a message object of type '<module_abnormal_info>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'module)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'module)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'module)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'module)) ostream)
  (cl:let* ((signed (cl:slot-value msg 'errCode)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <module_abnormal_info>) istream)
  "Deserializes a message object of type '<module_abnormal_info>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'module)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'module)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'module)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'module)) (cl:read-byte istream))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'errCode) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<module_abnormal_info>)))
  "Returns string type for a message object of type '<module_abnormal_info>"
  "comm_msg/module_abnormal_info")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'module_abnormal_info)))
  "Returns string type for a message object of type 'module_abnormal_info"
  "comm_msg/module_abnormal_info")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<module_abnormal_info>)))
  "Returns md5sum for a message object of type '<module_abnormal_info>"
  "84ac86691c35a13d550d35161d31a61a")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'module_abnormal_info)))
  "Returns md5sum for a message object of type 'module_abnormal_info"
  "84ac86691c35a13d550d35161d31a61a")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<module_abnormal_info>)))
  "Returns full string definition for message of type '<module_abnormal_info>"
  (cl:format cl:nil "uint32 module~%int32  errCode          # 0 表示正常, 其它都表示异常，后续扩展~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'module_abnormal_info)))
  "Returns full string definition for message of type 'module_abnormal_info"
  (cl:format cl:nil "uint32 module~%int32  errCode          # 0 表示正常, 其它都表示异常，后续扩展~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <module_abnormal_info>))
  (cl:+ 0
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <module_abnormal_info>))
  "Converts a ROS message object to a list"
  (cl:list 'module_abnormal_info
    (cl:cons ':module (module msg))
    (cl:cons ':errCode (errCode msg))
))
