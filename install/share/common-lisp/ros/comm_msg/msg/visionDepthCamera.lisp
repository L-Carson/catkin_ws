; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude visionDepthCamera.msg.html

(cl:defclass <visionDepthCamera> (roslisp-msg-protocol:ros-message)
  ((selectDepthCamera
    :reader selectDepthCamera
    :initarg :selectDepthCamera
    :type cl:fixnum
    :initform 0))
)

(cl:defclass visionDepthCamera (<visionDepthCamera>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <visionDepthCamera>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'visionDepthCamera)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<visionDepthCamera> is deprecated: use comm_msg-msg:visionDepthCamera instead.")))

(cl:ensure-generic-function 'selectDepthCamera-val :lambda-list '(m))
(cl:defmethod selectDepthCamera-val ((m <visionDepthCamera>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:selectDepthCamera-val is deprecated.  Use comm_msg-msg:selectDepthCamera instead.")
  (selectDepthCamera m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <visionDepthCamera>) ostream)
  "Serializes a message object of type '<visionDepthCamera>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'selectDepthCamera)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <visionDepthCamera>) istream)
  "Deserializes a message object of type '<visionDepthCamera>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'selectDepthCamera)) (cl:read-byte istream))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<visionDepthCamera>)))
  "Returns string type for a message object of type '<visionDepthCamera>"
  "comm_msg/visionDepthCamera")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'visionDepthCamera)))
  "Returns string type for a message object of type 'visionDepthCamera"
  "comm_msg/visionDepthCamera")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<visionDepthCamera>)))
  "Returns md5sum for a message object of type '<visionDepthCamera>"
  "81b78f084b6d77e189e72dcfcb2c3fa3")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'visionDepthCamera)))
  "Returns md5sum for a message object of type 'visionDepthCamera"
  "81b78f084b6d77e189e72dcfcb2c3fa3")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<visionDepthCamera>)))
  "Returns full string definition for message of type '<visionDepthCamera>"
  (cl:format cl:nil "# Select Depth Camera~%# 0: not select depth camera~%# 1: the head depth camera~%# 2: the back depth camera~%# 3: the left depth camera~%# 4: the right depth camera~%~%uint8 selectDepthCamera~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'visionDepthCamera)))
  "Returns full string definition for message of type 'visionDepthCamera"
  (cl:format cl:nil "# Select Depth Camera~%# 0: not select depth camera~%# 1: the head depth camera~%# 2: the back depth camera~%# 3: the left depth camera~%# 4: the right depth camera~%~%uint8 selectDepthCamera~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <visionDepthCamera>))
  (cl:+ 0
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <visionDepthCamera>))
  "Converts a ROS message object to a list"
  (cl:list 'visionDepthCamera
    (cl:cons ':selectDepthCamera (selectDepthCamera msg))
))
