; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude body_pose.msg.html

(cl:defclass <body_pose> (roslisp-msg-protocol:ros-message)
  ((body_yaw
    :reader body_yaw
    :initarg :body_yaw
    :type cl:float
    :initform 0.0)
   (backWheel_x
    :reader backWheel_x
    :initarg :backWheel_x
    :type cl:float
    :initform 0.0)
   (backWheel_y
    :reader backWheel_y
    :initarg :backWheel_y
    :type cl:float
    :initform 0.0)
   (frontWheel_x
    :reader frontWheel_x
    :initarg :frontWheel_x
    :type cl:float
    :initform 0.0)
   (frontWheel_y
    :reader frontWheel_y
    :initarg :frontWheel_y
    :type cl:float
    :initform 0.0)
   (alongEdgeRefPoint_x
    :reader alongEdgeRefPoint_x
    :initarg :alongEdgeRefPoint_x
    :type cl:float
    :initform 0.0)
   (alongEdgeRefPoint_y
    :reader alongEdgeRefPoint_y
    :initarg :alongEdgeRefPoint_y
    :type cl:float
    :initform 0.0))
)

(cl:defclass body_pose (<body_pose>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <body_pose>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'body_pose)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<body_pose> is deprecated: use comm_msg-msg:body_pose instead.")))

(cl:ensure-generic-function 'body_yaw-val :lambda-list '(m))
(cl:defmethod body_yaw-val ((m <body_pose>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:body_yaw-val is deprecated.  Use comm_msg-msg:body_yaw instead.")
  (body_yaw m))

(cl:ensure-generic-function 'backWheel_x-val :lambda-list '(m))
(cl:defmethod backWheel_x-val ((m <body_pose>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:backWheel_x-val is deprecated.  Use comm_msg-msg:backWheel_x instead.")
  (backWheel_x m))

(cl:ensure-generic-function 'backWheel_y-val :lambda-list '(m))
(cl:defmethod backWheel_y-val ((m <body_pose>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:backWheel_y-val is deprecated.  Use comm_msg-msg:backWheel_y instead.")
  (backWheel_y m))

(cl:ensure-generic-function 'frontWheel_x-val :lambda-list '(m))
(cl:defmethod frontWheel_x-val ((m <body_pose>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:frontWheel_x-val is deprecated.  Use comm_msg-msg:frontWheel_x instead.")
  (frontWheel_x m))

(cl:ensure-generic-function 'frontWheel_y-val :lambda-list '(m))
(cl:defmethod frontWheel_y-val ((m <body_pose>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:frontWheel_y-val is deprecated.  Use comm_msg-msg:frontWheel_y instead.")
  (frontWheel_y m))

(cl:ensure-generic-function 'alongEdgeRefPoint_x-val :lambda-list '(m))
(cl:defmethod alongEdgeRefPoint_x-val ((m <body_pose>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:alongEdgeRefPoint_x-val is deprecated.  Use comm_msg-msg:alongEdgeRefPoint_x instead.")
  (alongEdgeRefPoint_x m))

(cl:ensure-generic-function 'alongEdgeRefPoint_y-val :lambda-list '(m))
(cl:defmethod alongEdgeRefPoint_y-val ((m <body_pose>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:alongEdgeRefPoint_y-val is deprecated.  Use comm_msg-msg:alongEdgeRefPoint_y instead.")
  (alongEdgeRefPoint_y m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <body_pose>) ostream)
  "Serializes a message object of type '<body_pose>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'body_yaw))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'backWheel_x))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'backWheel_y))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'frontWheel_x))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'frontWheel_y))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'alongEdgeRefPoint_x))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'alongEdgeRefPoint_y))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <body_pose>) istream)
  "Deserializes a message object of type '<body_pose>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'body_yaw) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'backWheel_x) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'backWheel_y) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'frontWheel_x) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'frontWheel_y) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'alongEdgeRefPoint_x) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'alongEdgeRefPoint_y) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<body_pose>)))
  "Returns string type for a message object of type '<body_pose>"
  "comm_msg/body_pose")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'body_pose)))
  "Returns string type for a message object of type 'body_pose"
  "comm_msg/body_pose")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<body_pose>)))
  "Returns md5sum for a message object of type '<body_pose>"
  "f09536d45d4aed5f09a8a01687e32257")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'body_pose)))
  "Returns md5sum for a message object of type 'body_pose"
  "f09536d45d4aed5f09a8a01687e32257")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<body_pose>)))
  "Returns full string definition for message of type '<body_pose>"
  (cl:format cl:nil "float32 body_yaw~%float32 backWheel_x~%float32 backWheel_y~%float32 frontWheel_x~%float32 frontWheel_y~%float32 alongEdgeRefPoint_x~%float32 alongEdgeRefPoint_y~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'body_pose)))
  "Returns full string definition for message of type 'body_pose"
  (cl:format cl:nil "float32 body_yaw~%float32 backWheel_x~%float32 backWheel_y~%float32 frontWheel_x~%float32 frontWheel_y~%float32 alongEdgeRefPoint_x~%float32 alongEdgeRefPoint_y~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <body_pose>))
  (cl:+ 0
     4
     4
     4
     4
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <body_pose>))
  "Converts a ROS message object to a list"
  (cl:list 'body_pose
    (cl:cons ':body_yaw (body_yaw msg))
    (cl:cons ':backWheel_x (backWheel_x msg))
    (cl:cons ':backWheel_y (backWheel_y msg))
    (cl:cons ':frontWheel_x (frontWheel_x msg))
    (cl:cons ':frontWheel_y (frontWheel_y msg))
    (cl:cons ':alongEdgeRefPoint_x (alongEdgeRefPoint_x msg))
    (cl:cons ':alongEdgeRefPoint_y (alongEdgeRefPoint_y msg))
))
