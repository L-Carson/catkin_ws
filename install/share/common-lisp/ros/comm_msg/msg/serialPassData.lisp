; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude serialPassData.msg.html

(cl:defclass <serialPassData> (roslisp-msg-protocol:ros-message)
  ((dataStream
    :reader dataStream
    :initarg :dataStream
    :type (cl:vector cl:fixnum)
   :initform (cl:make-array 0 :element-type 'cl:fixnum :initial-element 0)))
)

(cl:defclass serialPassData (<serialPassData>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <serialPassData>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'serialPassData)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<serialPassData> is deprecated: use comm_msg-msg:serialPassData instead.")))

(cl:ensure-generic-function 'dataStream-val :lambda-list '(m))
(cl:defmethod dataStream-val ((m <serialPassData>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:dataStream-val is deprecated.  Use comm_msg-msg:dataStream instead.")
  (dataStream m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <serialPassData>) ostream)
  "Serializes a message object of type '<serialPassData>"
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'dataStream))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:write-byte (cl:ldb (cl:byte 8 0) ele) ostream))
   (cl:slot-value msg 'dataStream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <serialPassData>) istream)
  "Deserializes a message object of type '<serialPassData>"
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'dataStream) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'dataStream)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:aref vals i)) (cl:read-byte istream)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<serialPassData>)))
  "Returns string type for a message object of type '<serialPassData>"
  "comm_msg/serialPassData")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'serialPassData)))
  "Returns string type for a message object of type 'serialPassData"
  "comm_msg/serialPassData")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<serialPassData>)))
  "Returns md5sum for a message object of type '<serialPassData>"
  "356d46a591b661111ed0ede4b4ae78ca")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'serialPassData)))
  "Returns md5sum for a message object of type 'serialPassData"
  "356d46a591b661111ed0ede4b4ae78ca")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<serialPassData>)))
  "Returns full string definition for message of type '<serialPassData>"
  (cl:format cl:nil "# serial byte stream~%uint8[] dataStream~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'serialPassData)))
  "Returns full string definition for message of type 'serialPassData"
  (cl:format cl:nil "# serial byte stream~%uint8[] dataStream~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <serialPassData>))
  (cl:+ 0
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'dataStream) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 1)))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <serialPassData>))
  "Converts a ROS message object to a list"
  (cl:list 'serialPassData
    (cl:cons ':dataStream (dataStream msg))
))
