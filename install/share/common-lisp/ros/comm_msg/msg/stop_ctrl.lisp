; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude stop_ctrl.msg.html

(cl:defclass <stop_ctrl> (roslisp-msg-protocol:ros-message)
  ((isAct
    :reader isAct
    :initarg :isAct
    :type cl:fixnum
    :initform 0))
)

(cl:defclass stop_ctrl (<stop_ctrl>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <stop_ctrl>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'stop_ctrl)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<stop_ctrl> is deprecated: use comm_msg-msg:stop_ctrl instead.")))

(cl:ensure-generic-function 'isAct-val :lambda-list '(m))
(cl:defmethod isAct-val ((m <stop_ctrl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isAct-val is deprecated.  Use comm_msg-msg:isAct instead.")
  (isAct m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <stop_ctrl>) ostream)
  "Serializes a message object of type '<stop_ctrl>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isAct)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <stop_ctrl>) istream)
  "Deserializes a message object of type '<stop_ctrl>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isAct)) (cl:read-byte istream))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<stop_ctrl>)))
  "Returns string type for a message object of type '<stop_ctrl>"
  "comm_msg/stop_ctrl")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'stop_ctrl)))
  "Returns string type for a message object of type 'stop_ctrl"
  "comm_msg/stop_ctrl")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<stop_ctrl>)))
  "Returns md5sum for a message object of type '<stop_ctrl>"
  "6214bd4b533a199bfb2bc3ad84feb300")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'stop_ctrl)))
  "Returns md5sum for a message object of type 'stop_ctrl"
  "6214bd4b533a199bfb2bc3ad84feb300")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<stop_ctrl>)))
  "Returns full string definition for message of type '<stop_ctrl>"
  (cl:format cl:nil "uint8  isAct~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'stop_ctrl)))
  "Returns full string definition for message of type 'stop_ctrl"
  (cl:format cl:nil "uint8  isAct~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <stop_ctrl>))
  (cl:+ 0
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <stop_ctrl>))
  "Converts a ROS message object to a list"
  (cl:list 'stop_ctrl
    (cl:cons ':isAct (isAct msg))
))
