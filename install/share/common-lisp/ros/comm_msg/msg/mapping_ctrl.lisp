; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude mapping_ctrl.msg.html

(cl:defclass <mapping_ctrl> (roslisp-msg-protocol:ros-message)
  ((cmd
    :reader cmd
    :initarg :cmd
    :type cl:integer
    :initform 0))
)

(cl:defclass mapping_ctrl (<mapping_ctrl>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <mapping_ctrl>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'mapping_ctrl)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<mapping_ctrl> is deprecated: use comm_msg-msg:mapping_ctrl instead.")))

(cl:ensure-generic-function 'cmd-val :lambda-list '(m))
(cl:defmethod cmd-val ((m <mapping_ctrl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:cmd-val is deprecated.  Use comm_msg-msg:cmd instead.")
  (cmd m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <mapping_ctrl>) ostream)
  "Serializes a message object of type '<mapping_ctrl>"
  (cl:let* ((signed (cl:slot-value msg 'cmd)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <mapping_ctrl>) istream)
  "Deserializes a message object of type '<mapping_ctrl>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'cmd) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<mapping_ctrl>)))
  "Returns string type for a message object of type '<mapping_ctrl>"
  "comm_msg/mapping_ctrl")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'mapping_ctrl)))
  "Returns string type for a message object of type 'mapping_ctrl"
  "comm_msg/mapping_ctrl")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<mapping_ctrl>)))
  "Returns md5sum for a message object of type '<mapping_ctrl>"
  "66990e73c7aab0c47ddcdc70f7fa5bd0")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'mapping_ctrl)))
  "Returns md5sum for a message object of type 'mapping_ctrl"
  "66990e73c7aab0c47ddcdc70f7fa5bd0")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<mapping_ctrl>)))
  "Returns full string definition for message of type '<mapping_ctrl>"
  (cl:format cl:nil "int32 cmd #1:save map command 2:enable gnss 3:disable gnss~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'mapping_ctrl)))
  "Returns full string definition for message of type 'mapping_ctrl"
  (cl:format cl:nil "int32 cmd #1:save map command 2:enable gnss 3:disable gnss~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <mapping_ctrl>))
  (cl:+ 0
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <mapping_ctrl>))
  "Converts a ROS message object to a list"
  (cl:list 'mapping_ctrl
    (cl:cons ':cmd (cmd msg))
))
