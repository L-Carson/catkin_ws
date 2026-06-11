; Auto-generated. Do not edit!


(cl:in-package comm_srvs-srv)


;//! \htmlinclude InitPose-request.msg.html

(cl:defclass <InitPose-request> (roslisp-msg-protocol:ros-message)
  ((x
    :reader x
    :initarg :x
    :type cl:float
    :initform 0.0)
   (y
    :reader y
    :initarg :y
    :type cl:float
    :initform 0.0)
   (yaw
    :reader yaw
    :initarg :yaw
    :type cl:float
    :initform 0.0))
)

(cl:defclass InitPose-request (<InitPose-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <InitPose-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'InitPose-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_srvs-srv:<InitPose-request> is deprecated: use comm_srvs-srv:InitPose-request instead.")))

(cl:ensure-generic-function 'x-val :lambda-list '(m))
(cl:defmethod x-val ((m <InitPose-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:x-val is deprecated.  Use comm_srvs-srv:x instead.")
  (x m))

(cl:ensure-generic-function 'y-val :lambda-list '(m))
(cl:defmethod y-val ((m <InitPose-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:y-val is deprecated.  Use comm_srvs-srv:y instead.")
  (y m))

(cl:ensure-generic-function 'yaw-val :lambda-list '(m))
(cl:defmethod yaw-val ((m <InitPose-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:yaw-val is deprecated.  Use comm_srvs-srv:yaw instead.")
  (yaw m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <InitPose-request>) ostream)
  "Serializes a message object of type '<InitPose-request>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'x))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'y))))
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
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <InitPose-request>) istream)
  "Deserializes a message object of type '<InitPose-request>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'x) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'y) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'yaw) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<InitPose-request>)))
  "Returns string type for a service object of type '<InitPose-request>"
  "comm_srvs/InitPoseRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'InitPose-request)))
  "Returns string type for a service object of type 'InitPose-request"
  "comm_srvs/InitPoseRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<InitPose-request>)))
  "Returns md5sum for a message object of type '<InitPose-request>"
  "4ea516c50b8e2b735013ac031a10cc9d")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'InitPose-request)))
  "Returns md5sum for a message object of type 'InitPose-request"
  "4ea516c50b8e2b735013ac031a10cc9d")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<InitPose-request>)))
  "Returns full string definition for message of type '<InitPose-request>"
  (cl:format cl:nil "float32 x~%float32 y~%float32 yaw~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'InitPose-request)))
  "Returns full string definition for message of type 'InitPose-request"
  (cl:format cl:nil "float32 x~%float32 y~%float32 yaw~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <InitPose-request>))
  (cl:+ 0
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <InitPose-request>))
  "Converts a ROS message object to a list"
  (cl:list 'InitPose-request
    (cl:cons ':x (x msg))
    (cl:cons ':y (y msg))
    (cl:cons ':yaw (yaw msg))
))
;//! \htmlinclude InitPose-response.msg.html

(cl:defclass <InitPose-response> (roslisp-msg-protocol:ros-message)
  ((success
    :reader success
    :initarg :success
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass InitPose-response (<InitPose-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <InitPose-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'InitPose-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_srvs-srv:<InitPose-response> is deprecated: use comm_srvs-srv:InitPose-response instead.")))

(cl:ensure-generic-function 'success-val :lambda-list '(m))
(cl:defmethod success-val ((m <InitPose-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:success-val is deprecated.  Use comm_srvs-srv:success instead.")
  (success m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <InitPose-response>) ostream)
  "Serializes a message object of type '<InitPose-response>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'success) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <InitPose-response>) istream)
  "Deserializes a message object of type '<InitPose-response>"
    (cl:setf (cl:slot-value msg 'success) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<InitPose-response>)))
  "Returns string type for a service object of type '<InitPose-response>"
  "comm_srvs/InitPoseResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'InitPose-response)))
  "Returns string type for a service object of type 'InitPose-response"
  "comm_srvs/InitPoseResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<InitPose-response>)))
  "Returns md5sum for a message object of type '<InitPose-response>"
  "4ea516c50b8e2b735013ac031a10cc9d")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'InitPose-response)))
  "Returns md5sum for a message object of type 'InitPose-response"
  "4ea516c50b8e2b735013ac031a10cc9d")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<InitPose-response>)))
  "Returns full string definition for message of type '<InitPose-response>"
  (cl:format cl:nil "bool    success~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'InitPose-response)))
  "Returns full string definition for message of type 'InitPose-response"
  (cl:format cl:nil "bool    success~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <InitPose-response>))
  (cl:+ 0
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <InitPose-response>))
  "Converts a ROS message object to a list"
  (cl:list 'InitPose-response
    (cl:cons ':success (success msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'InitPose)))
  'InitPose-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'InitPose)))
  'InitPose-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'InitPose)))
  "Returns string type for a service object of type '<InitPose>"
  "comm_srvs/InitPose")