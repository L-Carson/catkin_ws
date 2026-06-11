; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude coorLabel.msg.html

(cl:defclass <coorLabel> (roslisp-msg-protocol:ros-message)
  ((value
    :reader value
    :initarg :value
    :type cl:fixnum
    :initform 0))
)

(cl:defclass coorLabel (<coorLabel>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <coorLabel>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'coorLabel)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<coorLabel> is deprecated: use comm_msg-msg:coorLabel instead.")))

(cl:ensure-generic-function 'value-val :lambda-list '(m))
(cl:defmethod value-val ((m <coorLabel>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:value-val is deprecated.  Use comm_msg-msg:value instead.")
  (value m))
(cl:defmethod roslisp-msg-protocol:symbol-codes ((msg-type (cl:eql '<coorLabel>)))
    "Constants for message type '<coorLabel>"
  '((:UNKNOWN . 0)
    (:STATIC . 1)
    (:DYNAMIC . 2)
    (:HOLLOW_OBST . 3)
    (:SHORT_HOLD . 4)
    (:COMMON_HOLD . 5)
    (:BLIND_SPOT . 6)
    (:SLOPE_PAD . 7)
    (:CONTACT_EDGE . 8)
    (:TREELAWN . 9)
    (:ULTRASONIC . 10))
)
(cl:defmethod roslisp-msg-protocol:symbol-codes ((msg-type (cl:eql 'coorLabel)))
    "Constants for message type 'coorLabel"
  '((:UNKNOWN . 0)
    (:STATIC . 1)
    (:DYNAMIC . 2)
    (:HOLLOW_OBST . 3)
    (:SHORT_HOLD . 4)
    (:COMMON_HOLD . 5)
    (:BLIND_SPOT . 6)
    (:SLOPE_PAD . 7)
    (:CONTACT_EDGE . 8)
    (:TREELAWN . 9)
    (:ULTRASONIC . 10))
)
(cl:defmethod roslisp-msg-protocol:serialize ((msg <coorLabel>) ostream)
  "Serializes a message object of type '<coorLabel>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'value)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <coorLabel>) istream)
  "Deserializes a message object of type '<coorLabel>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'value)) (cl:read-byte istream))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<coorLabel>)))
  "Returns string type for a message object of type '<coorLabel>"
  "comm_msg/coorLabel")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'coorLabel)))
  "Returns string type for a message object of type 'coorLabel"
  "comm_msg/coorLabel")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<coorLabel>)))
  "Returns md5sum for a message object of type '<coorLabel>"
  "c0283db75cf753d6e1cb791ae85ce57c")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'coorLabel)))
  "Returns md5sum for a message object of type 'coorLabel"
  "c0283db75cf753d6e1cb791ae85ce57c")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<coorLabel>)))
  "Returns full string definition for message of type '<coorLabel>"
  (cl:format cl:nil "uint8   UNKNOWN      = 0  #未知~%uint8   STATIC       = 1  #静态~%uint8   DYNAMIC      = 2  #动态~%uint8   HOLLOW_OBST  = 3  #空洞障碍~%uint8   SHORT_HOLD   = 4  #低矮维持~%uint8   COMMON_HOLD  = 5  #常规维持~%uint8   BLIND_SPOT   = 6  #盲区障碍~%uint8   SLOPE_PAD    = 7  #斜坡垫~%uint8   CONTACT_EDGE = 8  #触边障碍~%uint8   TREELAWN     = 9  #绿植障碍~%uint8   ULTRASONIC   = 10 #超声~%~%uint8 value~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'coorLabel)))
  "Returns full string definition for message of type 'coorLabel"
  (cl:format cl:nil "uint8   UNKNOWN      = 0  #未知~%uint8   STATIC       = 1  #静态~%uint8   DYNAMIC      = 2  #动态~%uint8   HOLLOW_OBST  = 3  #空洞障碍~%uint8   SHORT_HOLD   = 4  #低矮维持~%uint8   COMMON_HOLD  = 5  #常规维持~%uint8   BLIND_SPOT   = 6  #盲区障碍~%uint8   SLOPE_PAD    = 7  #斜坡垫~%uint8   CONTACT_EDGE = 8  #触边障碍~%uint8   TREELAWN     = 9  #绿植障碍~%uint8   ULTRASONIC   = 10 #超声~%~%uint8 value~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <coorLabel>))
  (cl:+ 0
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <coorLabel>))
  "Converts a ROS message object to a list"
  (cl:list 'coorLabel
    (cl:cons ':value (value msg))
))
