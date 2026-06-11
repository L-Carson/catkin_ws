; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude carBodyFilter.msg.html

(cl:defclass <carBodyFilter> (roslisp-msg-protocol:ros-message)
  ((filter
    :reader filter
    :initarg :filter
    :type cl:fixnum
    :initform 0))
)

(cl:defclass carBodyFilter (<carBodyFilter>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <carBodyFilter>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'carBodyFilter)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<carBodyFilter> is deprecated: use comm_msg-msg:carBodyFilter instead.")))

(cl:ensure-generic-function 'filter-val :lambda-list '(m))
(cl:defmethod filter-val ((m <carBodyFilter>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:filter-val is deprecated.  Use comm_msg-msg:filter instead.")
  (filter m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <carBodyFilter>) ostream)
  "Serializes a message object of type '<carBodyFilter>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'filter)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <carBodyFilter>) istream)
  "Deserializes a message object of type '<carBodyFilter>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'filter)) (cl:read-byte istream))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<carBodyFilter>)))
  "Returns string type for a message object of type '<carBodyFilter>"
  "comm_msg/carBodyFilter")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'carBodyFilter)))
  "Returns string type for a message object of type 'carBodyFilter"
  "comm_msg/carBodyFilter")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<carBodyFilter>)))
  "Returns md5sum for a message object of type '<carBodyFilter>"
  "ca8270e9d6440d7014080687771919c9")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'carBodyFilter)))
  "Returns md5sum for a message object of type 'carBodyFilter"
  "ca8270e9d6440d7014080687771919c9")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<carBodyFilter>)))
  "Returns full string definition for message of type '<carBodyFilter>"
  (cl:format cl:nil "# 0 全量构建~%# 1 增量构建~%# 2 保存并启用~%# 3 取消构建~%~%uint8 filter~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'carBodyFilter)))
  "Returns full string definition for message of type 'carBodyFilter"
  (cl:format cl:nil "# 0 全量构建~%# 1 增量构建~%# 2 保存并启用~%# 3 取消构建~%~%uint8 filter~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <carBodyFilter>))
  (cl:+ 0
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <carBodyFilter>))
  "Converts a ROS message object to a list"
  (cl:list 'carBodyFilter
    (cl:cons ':filter (filter msg))
))
