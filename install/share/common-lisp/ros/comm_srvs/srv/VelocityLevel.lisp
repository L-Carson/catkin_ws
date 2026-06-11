; Auto-generated. Do not edit!


(cl:in-package comm_srvs-srv)


;//! \htmlinclude VelocityLevel-request.msg.html

(cl:defclass <VelocityLevel-request> (roslisp-msg-protocol:ros-message)
  ((level
    :reader level
    :initarg :level
    :type cl:integer
    :initform 0))
)

(cl:defclass VelocityLevel-request (<VelocityLevel-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <VelocityLevel-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'VelocityLevel-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_srvs-srv:<VelocityLevel-request> is deprecated: use comm_srvs-srv:VelocityLevel-request instead.")))

(cl:ensure-generic-function 'level-val :lambda-list '(m))
(cl:defmethod level-val ((m <VelocityLevel-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:level-val is deprecated.  Use comm_srvs-srv:level instead.")
  (level m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <VelocityLevel-request>) ostream)
  "Serializes a message object of type '<VelocityLevel-request>"
  (cl:let* ((signed (cl:slot-value msg 'level)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <VelocityLevel-request>) istream)
  "Deserializes a message object of type '<VelocityLevel-request>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'level) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<VelocityLevel-request>)))
  "Returns string type for a service object of type '<VelocityLevel-request>"
  "comm_srvs/VelocityLevelRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'VelocityLevel-request)))
  "Returns string type for a service object of type 'VelocityLevel-request"
  "comm_srvs/VelocityLevelRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<VelocityLevel-request>)))
  "Returns md5sum for a message object of type '<VelocityLevel-request>"
  "442ff6aec1dc9bb7000abb97372a7b4d")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'VelocityLevel-request)))
  "Returns md5sum for a message object of type 'VelocityLevel-request"
  "442ff6aec1dc9bb7000abb97372a7b4d")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<VelocityLevel-request>)))
  "Returns full string definition for message of type '<VelocityLevel-request>"
  (cl:format cl:nil "int32   level~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'VelocityLevel-request)))
  "Returns full string definition for message of type 'VelocityLevel-request"
  (cl:format cl:nil "int32   level~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <VelocityLevel-request>))
  (cl:+ 0
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <VelocityLevel-request>))
  "Converts a ROS message object to a list"
  (cl:list 'VelocityLevel-request
    (cl:cons ':level (level msg))
))
;//! \htmlinclude VelocityLevel-response.msg.html

(cl:defclass <VelocityLevel-response> (roslisp-msg-protocol:ros-message)
  ((success
    :reader success
    :initarg :success
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass VelocityLevel-response (<VelocityLevel-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <VelocityLevel-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'VelocityLevel-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_srvs-srv:<VelocityLevel-response> is deprecated: use comm_srvs-srv:VelocityLevel-response instead.")))

(cl:ensure-generic-function 'success-val :lambda-list '(m))
(cl:defmethod success-val ((m <VelocityLevel-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:success-val is deprecated.  Use comm_srvs-srv:success instead.")
  (success m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <VelocityLevel-response>) ostream)
  "Serializes a message object of type '<VelocityLevel-response>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'success) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <VelocityLevel-response>) istream)
  "Deserializes a message object of type '<VelocityLevel-response>"
    (cl:setf (cl:slot-value msg 'success) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<VelocityLevel-response>)))
  "Returns string type for a service object of type '<VelocityLevel-response>"
  "comm_srvs/VelocityLevelResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'VelocityLevel-response)))
  "Returns string type for a service object of type 'VelocityLevel-response"
  "comm_srvs/VelocityLevelResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<VelocityLevel-response>)))
  "Returns md5sum for a message object of type '<VelocityLevel-response>"
  "442ff6aec1dc9bb7000abb97372a7b4d")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'VelocityLevel-response)))
  "Returns md5sum for a message object of type 'VelocityLevel-response"
  "442ff6aec1dc9bb7000abb97372a7b4d")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<VelocityLevel-response>)))
  "Returns full string definition for message of type '<VelocityLevel-response>"
  (cl:format cl:nil "bool    success~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'VelocityLevel-response)))
  "Returns full string definition for message of type 'VelocityLevel-response"
  (cl:format cl:nil "bool    success~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <VelocityLevel-response>))
  (cl:+ 0
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <VelocityLevel-response>))
  "Converts a ROS message object to a list"
  (cl:list 'VelocityLevel-response
    (cl:cons ':success (success msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'VelocityLevel)))
  'VelocityLevel-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'VelocityLevel)))
  'VelocityLevel-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'VelocityLevel)))
  "Returns string type for a service object of type '<VelocityLevel>"
  "comm_srvs/VelocityLevel")