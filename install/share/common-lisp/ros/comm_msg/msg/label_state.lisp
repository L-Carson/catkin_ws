; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude label_state.msg.html

(cl:defclass <label_state> (roslisp-msg-protocol:ros-message)
  ((value
    :reader value
    :initarg :value
    :type cl:fixnum
    :initform 0))
)

(cl:defclass label_state (<label_state>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <label_state>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'label_state)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<label_state> is deprecated: use comm_msg-msg:label_state instead.")))

(cl:ensure-generic-function 'value-val :lambda-list '(m))
(cl:defmethod value-val ((m <label_state>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:value-val is deprecated.  Use comm_msg-msg:value instead.")
  (value m))
(cl:defmethod roslisp-msg-protocol:symbol-codes ((msg-type (cl:eql '<label_state>)))
    "Constants for message type '<label_state>"
  '((:NO . 0)
    (:YES . 1)
    (:MAY . 2))
)
(cl:defmethod roslisp-msg-protocol:symbol-codes ((msg-type (cl:eql 'label_state)))
    "Constants for message type 'label_state"
  '((:NO . 0)
    (:YES . 1)
    (:MAY . 2))
)
(cl:defmethod roslisp-msg-protocol:serialize ((msg <label_state>) ostream)
  "Serializes a message object of type '<label_state>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'value)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <label_state>) istream)
  "Deserializes a message object of type '<label_state>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'value)) (cl:read-byte istream))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<label_state>)))
  "Returns string type for a message object of type '<label_state>"
  "comm_msg/label_state")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'label_state)))
  "Returns string type for a message object of type 'label_state"
  "comm_msg/label_state")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<label_state>)))
  "Returns md5sum for a message object of type '<label_state>"
  "888716ada627b9e5d6021e226a28f209")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'label_state)))
  "Returns md5sum for a message object of type 'label_state"
  "888716ada627b9e5d6021e226a28f209")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<label_state>)))
  "Returns full string definition for message of type '<label_state>"
  (cl:format cl:nil "#Note: 标签类别存在状态~%~%uint8 NO  = 0~%uint8 YES = 1~%uint8 MAY = 2~%~%uint8 value~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'label_state)))
  "Returns full string definition for message of type 'label_state"
  (cl:format cl:nil "#Note: 标签类别存在状态~%~%uint8 NO  = 0~%uint8 YES = 1~%uint8 MAY = 2~%~%uint8 value~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <label_state>))
  (cl:+ 0
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <label_state>))
  "Converts a ROS message object to a list"
  (cl:list 'label_state
    (cl:cons ':value (value msg))
))
