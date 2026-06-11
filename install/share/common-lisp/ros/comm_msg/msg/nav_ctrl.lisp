; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude nav_ctrl.msg.html

(cl:defclass <nav_ctrl> (roslisp-msg-protocol:ros-message)
  ((value
    :reader value
    :initarg :value
    :type cl:fixnum
    :initform 0))
)

(cl:defclass nav_ctrl (<nav_ctrl>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <nav_ctrl>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'nav_ctrl)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<nav_ctrl> is deprecated: use comm_msg-msg:nav_ctrl instead.")))

(cl:ensure-generic-function 'value-val :lambda-list '(m))
(cl:defmethod value-val ((m <nav_ctrl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:value-val is deprecated.  Use comm_msg-msg:value instead.")
  (value m))
(cl:defmethod roslisp-msg-protocol:symbol-codes ((msg-type (cl:eql '<nav_ctrl>)))
    "Constants for message type '<nav_ctrl>"
  '((:GARBAGE_DETECT . 1)
    (:CONTACT_EDGE . 2))
)
(cl:defmethod roslisp-msg-protocol:symbol-codes ((msg-type (cl:eql 'nav_ctrl)))
    "Constants for message type 'nav_ctrl"
  '((:GARBAGE_DETECT . 1)
    (:CONTACT_EDGE . 2))
)
(cl:defmethod roslisp-msg-protocol:serialize ((msg <nav_ctrl>) ostream)
  "Serializes a message object of type '<nav_ctrl>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'value)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <nav_ctrl>) istream)
  "Deserializes a message object of type '<nav_ctrl>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'value)) (cl:read-byte istream))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<nav_ctrl>)))
  "Returns string type for a message object of type '<nav_ctrl>"
  "comm_msg/nav_ctrl")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'nav_ctrl)))
  "Returns string type for a message object of type 'nav_ctrl"
  "comm_msg/nav_ctrl")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<nav_ctrl>)))
  "Returns md5sum for a message object of type '<nav_ctrl>"
  "c6fe31622265b6df86e8a79eeb6d20fe")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'nav_ctrl)))
  "Returns md5sum for a message object of type 'nav_ctrl"
  "c6fe31622265b6df86e8a79eeb6d20fe")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<nav_ctrl>)))
  "Returns full string definition for message of type '<nav_ctrl>"
  (cl:format cl:nil "uint8 GARBAGE_DETECT = 1    # 垃圾寻扫~%uint8 CONTACT_EDGE   = 2    # 触边生成障碍物~%~%uint8 value~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'nav_ctrl)))
  "Returns full string definition for message of type 'nav_ctrl"
  (cl:format cl:nil "uint8 GARBAGE_DETECT = 1    # 垃圾寻扫~%uint8 CONTACT_EDGE   = 2    # 触边生成障碍物~%~%uint8 value~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <nav_ctrl>))
  (cl:+ 0
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <nav_ctrl>))
  "Converts a ROS message object to a list"
  (cl:list 'nav_ctrl
    (cl:cons ':value (value msg))
))
