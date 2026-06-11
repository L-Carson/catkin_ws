; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude dustbinState.msg.html

(cl:defclass <dustbinState> (roslisp-msg-protocol:ros-message)
  ((isFull
    :reader isFull
    :initarg :isFull
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass dustbinState (<dustbinState>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <dustbinState>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'dustbinState)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<dustbinState> is deprecated: use comm_msg-msg:dustbinState instead.")))

(cl:ensure-generic-function 'isFull-val :lambda-list '(m))
(cl:defmethod isFull-val ((m <dustbinState>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isFull-val is deprecated.  Use comm_msg-msg:isFull instead.")
  (isFull m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <dustbinState>) ostream)
  "Serializes a message object of type '<dustbinState>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'isFull) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <dustbinState>) istream)
  "Deserializes a message object of type '<dustbinState>"
    (cl:setf (cl:slot-value msg 'isFull) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<dustbinState>)))
  "Returns string type for a message object of type '<dustbinState>"
  "comm_msg/dustbinState")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'dustbinState)))
  "Returns string type for a message object of type 'dustbinState"
  "comm_msg/dustbinState")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<dustbinState>)))
  "Returns md5sum for a message object of type '<dustbinState>"
  "3815cb2adedcd1d91ff6caca01a84001")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'dustbinState)))
  "Returns md5sum for a message object of type 'dustbinState"
  "3815cb2adedcd1d91ff6caca01a84001")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<dustbinState>)))
  "Returns full string definition for message of type '<dustbinState>"
  (cl:format cl:nil "bool  isFull    #站点垃圾箱是否已满~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'dustbinState)))
  "Returns full string definition for message of type 'dustbinState"
  (cl:format cl:nil "bool  isFull    #站点垃圾箱是否已满~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <dustbinState>))
  (cl:+ 0
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <dustbinState>))
  "Converts a ROS message object to a list"
  (cl:list 'dustbinState
    (cl:cons ':isFull (isFull msg))
))
