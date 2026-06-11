; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude simulation_circular_obst.msg.html

(cl:defclass <simulation_circular_obst> (roslisp-msg-protocol:ros-message)
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
   (radius
    :reader radius
    :initarg :radius
    :type cl:float
    :initform 0.0))
)

(cl:defclass simulation_circular_obst (<simulation_circular_obst>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <simulation_circular_obst>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'simulation_circular_obst)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<simulation_circular_obst> is deprecated: use comm_msg-msg:simulation_circular_obst instead.")))

(cl:ensure-generic-function 'x-val :lambda-list '(m))
(cl:defmethod x-val ((m <simulation_circular_obst>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:x-val is deprecated.  Use comm_msg-msg:x instead.")
  (x m))

(cl:ensure-generic-function 'y-val :lambda-list '(m))
(cl:defmethod y-val ((m <simulation_circular_obst>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:y-val is deprecated.  Use comm_msg-msg:y instead.")
  (y m))

(cl:ensure-generic-function 'radius-val :lambda-list '(m))
(cl:defmethod radius-val ((m <simulation_circular_obst>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:radius-val is deprecated.  Use comm_msg-msg:radius instead.")
  (radius m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <simulation_circular_obst>) ostream)
  "Serializes a message object of type '<simulation_circular_obst>"
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
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'radius))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <simulation_circular_obst>) istream)
  "Deserializes a message object of type '<simulation_circular_obst>"
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
    (cl:setf (cl:slot-value msg 'radius) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<simulation_circular_obst>)))
  "Returns string type for a message object of type '<simulation_circular_obst>"
  "comm_msg/simulation_circular_obst")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'simulation_circular_obst)))
  "Returns string type for a message object of type 'simulation_circular_obst"
  "comm_msg/simulation_circular_obst")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<simulation_circular_obst>)))
  "Returns md5sum for a message object of type '<simulation_circular_obst>"
  "816ab4505c541cc7c042e75523ad2290")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'simulation_circular_obst)))
  "Returns md5sum for a message object of type 'simulation_circular_obst"
  "816ab4505c541cc7c042e75523ad2290")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<simulation_circular_obst>)))
  "Returns full string definition for message of type '<simulation_circular_obst>"
  (cl:format cl:nil "float32 x~%float32 y~%float32 radius~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'simulation_circular_obst)))
  "Returns full string definition for message of type 'simulation_circular_obst"
  (cl:format cl:nil "float32 x~%float32 y~%float32 radius~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <simulation_circular_obst>))
  (cl:+ 0
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <simulation_circular_obst>))
  "Converts a ROS message object to a list"
  (cl:list 'simulation_circular_obst
    (cl:cons ':x (x msg))
    (cl:cons ':y (y msg))
    (cl:cons ':radius (radius msg))
))
