; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude coor2D.msg.html

(cl:defclass <coor2D> (roslisp-msg-protocol:ros-message)
  ((label
    :reader label
    :initarg :label
    :type comm_msg-msg:coorLabel
    :initform (cl:make-instance 'comm_msg-msg:coorLabel))
   (pos_x
    :reader pos_x
    :initarg :pos_x
    :type cl:float
    :initform 0.0)
   (pos_y
    :reader pos_y
    :initarg :pos_y
    :type cl:float
    :initform 0.0))
)

(cl:defclass coor2D (<coor2D>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <coor2D>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'coor2D)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<coor2D> is deprecated: use comm_msg-msg:coor2D instead.")))

(cl:ensure-generic-function 'label-val :lambda-list '(m))
(cl:defmethod label-val ((m <coor2D>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:label-val is deprecated.  Use comm_msg-msg:label instead.")
  (label m))

(cl:ensure-generic-function 'pos_x-val :lambda-list '(m))
(cl:defmethod pos_x-val ((m <coor2D>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:pos_x-val is deprecated.  Use comm_msg-msg:pos_x instead.")
  (pos_x m))

(cl:ensure-generic-function 'pos_y-val :lambda-list '(m))
(cl:defmethod pos_y-val ((m <coor2D>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:pos_y-val is deprecated.  Use comm_msg-msg:pos_y instead.")
  (pos_y m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <coor2D>) ostream)
  "Serializes a message object of type '<coor2D>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'label) ostream)
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
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <coor2D>) istream)
  "Deserializes a message object of type '<coor2D>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'label) istream)
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
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<coor2D>)))
  "Returns string type for a message object of type '<coor2D>"
  "comm_msg/coor2D")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'coor2D)))
  "Returns string type for a message object of type 'coor2D"
  "comm_msg/coor2D")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<coor2D>)))
  "Returns md5sum for a message object of type '<coor2D>"
  "a605ddea652ab63420ad444791949b49")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'coor2D)))
  "Returns md5sum for a message object of type 'coor2D"
  "a605ddea652ab63420ad444791949b49")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<coor2D>)))
  "Returns full string definition for message of type '<coor2D>"
  (cl:format cl:nil "coorLabel   label       #坐标类别~%float32     pos_x~%float32     pos_y~%================================================================================~%MSG: comm_msg/coorLabel~%uint8   UNKNOWN      = 0  #未知~%uint8   STATIC       = 1  #静态~%uint8   DYNAMIC      = 2  #动态~%uint8   HOLLOW_OBST  = 3  #空洞障碍~%uint8   SHORT_HOLD   = 4  #低矮维持~%uint8   COMMON_HOLD  = 5  #常规维持~%uint8   BLIND_SPOT   = 6  #盲区障碍~%uint8   SLOPE_PAD    = 7  #斜坡垫~%uint8   CONTACT_EDGE = 8  #触边障碍~%uint8   TREELAWN     = 9  #绿植障碍~%uint8   ULTRASONIC   = 10 #超声~%~%uint8 value~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'coor2D)))
  "Returns full string definition for message of type 'coor2D"
  (cl:format cl:nil "coorLabel   label       #坐标类别~%float32     pos_x~%float32     pos_y~%================================================================================~%MSG: comm_msg/coorLabel~%uint8   UNKNOWN      = 0  #未知~%uint8   STATIC       = 1  #静态~%uint8   DYNAMIC      = 2  #动态~%uint8   HOLLOW_OBST  = 3  #空洞障碍~%uint8   SHORT_HOLD   = 4  #低矮维持~%uint8   COMMON_HOLD  = 5  #常规维持~%uint8   BLIND_SPOT   = 6  #盲区障碍~%uint8   SLOPE_PAD    = 7  #斜坡垫~%uint8   CONTACT_EDGE = 8  #触边障碍~%uint8   TREELAWN     = 9  #绿植障碍~%uint8   ULTRASONIC   = 10 #超声~%~%uint8 value~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <coor2D>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'label))
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <coor2D>))
  "Converts a ROS message object to a list"
  (cl:list 'coor2D
    (cl:cons ':label (label msg))
    (cl:cons ':pos_x (pos_x msg))
    (cl:cons ':pos_y (pos_y msg))
))
