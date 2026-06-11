; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude coor3D.msg.html

(cl:defclass <coor3D> (roslisp-msg-protocol:ros-message)
  ((pos_x
    :reader pos_x
    :initarg :pos_x
    :type cl:float
    :initform 0.0)
   (pos_y
    :reader pos_y
    :initarg :pos_y
    :type cl:float
    :initform 0.0)
   (pos_z
    :reader pos_z
    :initarg :pos_z
    :type cl:float
    :initform 0.0))
)

(cl:defclass coor3D (<coor3D>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <coor3D>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'coor3D)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<coor3D> is deprecated: use comm_msg-msg:coor3D instead.")))

(cl:ensure-generic-function 'pos_x-val :lambda-list '(m))
(cl:defmethod pos_x-val ((m <coor3D>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:pos_x-val is deprecated.  Use comm_msg-msg:pos_x instead.")
  (pos_x m))

(cl:ensure-generic-function 'pos_y-val :lambda-list '(m))
(cl:defmethod pos_y-val ((m <coor3D>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:pos_y-val is deprecated.  Use comm_msg-msg:pos_y instead.")
  (pos_y m))

(cl:ensure-generic-function 'pos_z-val :lambda-list '(m))
(cl:defmethod pos_z-val ((m <coor3D>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:pos_z-val is deprecated.  Use comm_msg-msg:pos_z instead.")
  (pos_z m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <coor3D>) ostream)
  "Serializes a message object of type '<coor3D>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'pos_x))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'pos_y))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'pos_z))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <coor3D>) istream)
  "Deserializes a message object of type '<coor3D>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'pos_x) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'pos_y) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'pos_z) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<coor3D>)))
  "Returns string type for a message object of type '<coor3D>"
  "comm_msg/coor3D")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'coor3D)))
  "Returns string type for a message object of type 'coor3D"
  "comm_msg/coor3D")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<coor3D>)))
  "Returns md5sum for a message object of type '<coor3D>"
  "03225cff21da80c8dcb47075eac84a6b")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'coor3D)))
  "Returns md5sum for a message object of type 'coor3D"
  "03225cff21da80c8dcb47075eac84a6b")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<coor3D>)))
  "Returns full string definition for message of type '<coor3D>"
  (cl:format cl:nil "float32 pos_x~%float32 pos_y~%float32 pos_z~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'coor3D)))
  "Returns full string definition for message of type 'coor3D"
  (cl:format cl:nil "float32 pos_x~%float32 pos_y~%float32 pos_z~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <coor3D>))
  (cl:+ 0
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <coor3D>))
  "Converts a ROS message object to a list"
  (cl:list 'coor3D
    (cl:cons ':pos_x (pos_x msg))
    (cl:cons ':pos_y (pos_y msg))
    (cl:cons ':pos_z (pos_z msg))
))
