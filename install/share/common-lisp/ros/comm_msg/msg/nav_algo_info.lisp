; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude nav_algo_info.msg.html

(cl:defclass <nav_algo_info> (roslisp-msg-protocol:ros-message)
  ((algoType
    :reader algoType
    :initarg :algoType
    :type cl:fixnum
    :initform 0))
)

(cl:defclass nav_algo_info (<nav_algo_info>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <nav_algo_info>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'nav_algo_info)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<nav_algo_info> is deprecated: use comm_msg-msg:nav_algo_info instead.")))

(cl:ensure-generic-function 'algoType-val :lambda-list '(m))
(cl:defmethod algoType-val ((m <nav_algo_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:algoType-val is deprecated.  Use comm_msg-msg:algoType instead.")
  (algoType m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <nav_algo_info>) ostream)
  "Serializes a message object of type '<nav_algo_info>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'algoType)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <nav_algo_info>) istream)
  "Deserializes a message object of type '<nav_algo_info>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'algoType)) (cl:read-byte istream))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<nav_algo_info>)))
  "Returns string type for a message object of type '<nav_algo_info>"
  "comm_msg/nav_algo_info")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'nav_algo_info)))
  "Returns string type for a message object of type 'nav_algo_info"
  "comm_msg/nav_algo_info")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<nav_algo_info>)))
  "Returns md5sum for a message object of type '<nav_algo_info>"
  "e5aec186900a8a01b0a68e06489a4999")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'nav_algo_info)))
  "Returns md5sum for a message object of type 'nav_algo_info"
  "e5aec186900a8a01b0a68e06489a4999")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<nav_algo_info>)))
  "Returns full string definition for message of type '<nav_algo_info>"
  (cl:format cl:nil "uint8 algoType~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'nav_algo_info)))
  "Returns full string definition for message of type 'nav_algo_info"
  (cl:format cl:nil "uint8 algoType~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <nav_algo_info>))
  (cl:+ 0
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <nav_algo_info>))
  "Converts a ROS message object to a list"
  (cl:list 'nav_algo_info
    (cl:cons ':algoType (algoType msg))
))
