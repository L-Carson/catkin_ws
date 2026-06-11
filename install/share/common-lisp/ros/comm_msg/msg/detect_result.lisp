; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude detect_result.msg.html

(cl:defclass <detect_result> (roslisp-msg-protocol:ros-message)
  ((result
    :reader result
    :initarg :result
    :type cl:fixnum
    :initform 0))
)

(cl:defclass detect_result (<detect_result>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <detect_result>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'detect_result)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<detect_result> is deprecated: use comm_msg-msg:detect_result instead.")))

(cl:ensure-generic-function 'result-val :lambda-list '(m))
(cl:defmethod result-val ((m <detect_result>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:result-val is deprecated.  Use comm_msg-msg:result instead.")
  (result m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <detect_result>) ostream)
  "Serializes a message object of type '<detect_result>"
  (cl:let* ((signed (cl:slot-value msg 'result)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 256) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <detect_result>) istream)
  "Deserializes a message object of type '<detect_result>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'result) (cl:if (cl:< unsigned 128) unsigned (cl:- unsigned 256))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<detect_result>)))
  "Returns string type for a message object of type '<detect_result>"
  "comm_msg/detect_result")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'detect_result)))
  "Returns string type for a message object of type 'detect_result"
  "comm_msg/detect_result")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<detect_result>)))
  "Returns md5sum for a message object of type '<detect_result>"
  "4414c67819626a1b8e0f043a9a0d6c9a")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'detect_result)))
  "Returns md5sum for a message object of type 'detect_result"
  "4414c67819626a1b8e0f043a9a0d6c9a")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<detect_result>)))
  "Returns full string definition for message of type '<detect_result>"
  (cl:format cl:nil "# 识别门时 0为开 1为关 -1未知~%int8  result~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'detect_result)))
  "Returns full string definition for message of type 'detect_result"
  (cl:format cl:nil "# 识别门时 0为开 1为关 -1未知~%int8  result~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <detect_result>))
  (cl:+ 0
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <detect_result>))
  "Converts a ROS message object to a list"
  (cl:list 'detect_result
    (cl:cons ':result (result msg))
))
