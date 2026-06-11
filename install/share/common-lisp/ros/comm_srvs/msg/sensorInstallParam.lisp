; Auto-generated. Do not edit!


(cl:in-package comm_srvs-msg)


;//! \htmlinclude sensorInstallParam.msg.html

(cl:defclass <sensorInstallParam> (roslisp-msg-protocol:ros-message)
  ((name
    :reader name
    :initarg :name
    :type cl:string
    :initform "")
   (x
    :reader x
    :initarg :x
    :type cl:float
    :initform 0.0)
   (y
    :reader y
    :initarg :y
    :type cl:float
    :initform 0.0)
   (z
    :reader z
    :initarg :z
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

(cl:defclass sensorInstallParam (<sensorInstallParam>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <sensorInstallParam>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'sensorInstallParam)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_srvs-msg:<sensorInstallParam> is deprecated: use comm_srvs-msg:sensorInstallParam instead.")))

(cl:ensure-generic-function 'name-val :lambda-list '(m))
(cl:defmethod name-val ((m <sensorInstallParam>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-msg:name-val is deprecated.  Use comm_srvs-msg:name instead.")
  (name m))

(cl:ensure-generic-function 'x-val :lambda-list '(m))
(cl:defmethod x-val ((m <sensorInstallParam>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-msg:x-val is deprecated.  Use comm_srvs-msg:x instead.")
  (x m))

(cl:ensure-generic-function 'y-val :lambda-list '(m))
(cl:defmethod y-val ((m <sensorInstallParam>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-msg:y-val is deprecated.  Use comm_srvs-msg:y instead.")
  (y m))

(cl:ensure-generic-function 'z-val :lambda-list '(m))
(cl:defmethod z-val ((m <sensorInstallParam>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-msg:z-val is deprecated.  Use comm_srvs-msg:z instead.")
  (z m))

(cl:ensure-generic-function 'roll-val :lambda-list '(m))
(cl:defmethod roll-val ((m <sensorInstallParam>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-msg:roll-val is deprecated.  Use comm_srvs-msg:roll instead.")
  (roll m))

(cl:ensure-generic-function 'pitch-val :lambda-list '(m))
(cl:defmethod pitch-val ((m <sensorInstallParam>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-msg:pitch-val is deprecated.  Use comm_srvs-msg:pitch instead.")
  (pitch m))

(cl:ensure-generic-function 'yaw-val :lambda-list '(m))
(cl:defmethod yaw-val ((m <sensorInstallParam>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-msg:yaw-val is deprecated.  Use comm_srvs-msg:yaw instead.")
  (yaw m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <sensorInstallParam>) ostream)
  "Serializes a message object of type '<sensorInstallParam>"
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'name))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'name))
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
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'z))))
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
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <sensorInstallParam>) istream)
  "Deserializes a message object of type '<sensorInstallParam>"
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'name) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'name) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
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
    (cl:setf (cl:slot-value msg 'z) (roslisp-utils:decode-single-float-bits bits)))
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
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<sensorInstallParam>)))
  "Returns string type for a message object of type '<sensorInstallParam>"
  "comm_srvs/sensorInstallParam")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'sensorInstallParam)))
  "Returns string type for a message object of type 'sensorInstallParam"
  "comm_srvs/sensorInstallParam")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<sensorInstallParam>)))
  "Returns md5sum for a message object of type '<sensorInstallParam>"
  "4694c7f482fc573b14d6260c92b28b4a")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'sensorInstallParam)))
  "Returns md5sum for a message object of type 'sensorInstallParam"
  "4694c7f482fc573b14d6260c92b28b4a")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<sensorInstallParam>)))
  "Returns full string definition for message of type '<sensorInstallParam>"
  (cl:format cl:nil "string  name ~%float32 x~%float32 y~%float32 z~%float32 roll~%float32 pitch~%float32 yaw~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'sensorInstallParam)))
  "Returns full string definition for message of type 'sensorInstallParam"
  (cl:format cl:nil "string  name ~%float32 x~%float32 y~%float32 z~%float32 roll~%float32 pitch~%float32 yaw~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <sensorInstallParam>))
  (cl:+ 0
     4 (cl:length (cl:slot-value msg 'name))
     4
     4
     4
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <sensorInstallParam>))
  "Converts a ROS message object to a list"
  (cl:list 'sensorInstallParam
    (cl:cons ':name (name msg))
    (cl:cons ':x (x msg))
    (cl:cons ':y (y msg))
    (cl:cons ':z (z msg))
    (cl:cons ':roll (roll msg))
    (cl:cons ':pitch (pitch msg))
    (cl:cons ':yaw (yaw msg))
))
