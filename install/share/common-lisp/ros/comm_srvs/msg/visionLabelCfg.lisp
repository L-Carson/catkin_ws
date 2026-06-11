; Auto-generated. Do not edit!


(cl:in-package comm_srvs-msg)


;//! \htmlinclude visionLabelCfg.msg.html

(cl:defclass <visionLabelCfg> (roslisp-msg-protocol:ros-message)
  ((vectDetLabel
    :reader vectDetLabel
    :initarg :vectDetLabel
    :type (cl:vector cl:integer)
   :initform (cl:make-array 0 :element-type 'cl:integer :initial-element 0))
   (vectSegLabel
    :reader vectSegLabel
    :initarg :vectSegLabel
    :type (cl:vector cl:integer)
   :initform (cl:make-array 0 :element-type 'cl:integer :initial-element 0))
   (isSupportThickPipe
    :reader isSupportThickPipe
    :initarg :isSupportThickPipe
    :type cl:boolean
    :initform cl:nil)
   (thickPipeWidth
    :reader thickPipeWidth
    :initarg :thickPipeWidth
    :type cl:float
    :initform 0.0))
)

(cl:defclass visionLabelCfg (<visionLabelCfg>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <visionLabelCfg>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'visionLabelCfg)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_srvs-msg:<visionLabelCfg> is deprecated: use comm_srvs-msg:visionLabelCfg instead.")))

(cl:ensure-generic-function 'vectDetLabel-val :lambda-list '(m))
(cl:defmethod vectDetLabel-val ((m <visionLabelCfg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-msg:vectDetLabel-val is deprecated.  Use comm_srvs-msg:vectDetLabel instead.")
  (vectDetLabel m))

(cl:ensure-generic-function 'vectSegLabel-val :lambda-list '(m))
(cl:defmethod vectSegLabel-val ((m <visionLabelCfg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-msg:vectSegLabel-val is deprecated.  Use comm_srvs-msg:vectSegLabel instead.")
  (vectSegLabel m))

(cl:ensure-generic-function 'isSupportThickPipe-val :lambda-list '(m))
(cl:defmethod isSupportThickPipe-val ((m <visionLabelCfg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-msg:isSupportThickPipe-val is deprecated.  Use comm_srvs-msg:isSupportThickPipe instead.")
  (isSupportThickPipe m))

(cl:ensure-generic-function 'thickPipeWidth-val :lambda-list '(m))
(cl:defmethod thickPipeWidth-val ((m <visionLabelCfg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-msg:thickPipeWidth-val is deprecated.  Use comm_srvs-msg:thickPipeWidth instead.")
  (thickPipeWidth m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <visionLabelCfg>) ostream)
  "Serializes a message object of type '<visionLabelCfg>"
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'vectDetLabel))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:write-byte (cl:ldb (cl:byte 8 0) ele) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) ele) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) ele) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) ele) ostream))
   (cl:slot-value msg 'vectDetLabel))
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'vectSegLabel))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:write-byte (cl:ldb (cl:byte 8 0) ele) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) ele) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) ele) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) ele) ostream))
   (cl:slot-value msg 'vectSegLabel))
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'isSupportThickPipe) 1 0)) ostream)
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'thickPipeWidth))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <visionLabelCfg>) istream)
  "Deserializes a message object of type '<visionLabelCfg>"
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'vectDetLabel) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'vectDetLabel)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:aref vals i)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:aref vals i)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:aref vals i)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:aref vals i)) (cl:read-byte istream)))))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'vectSegLabel) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'vectSegLabel)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:aref vals i)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:aref vals i)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:aref vals i)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:aref vals i)) (cl:read-byte istream)))))
    (cl:setf (cl:slot-value msg 'isSupportThickPipe) (cl:not (cl:zerop (cl:read-byte istream))))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'thickPipeWidth) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<visionLabelCfg>)))
  "Returns string type for a message object of type '<visionLabelCfg>"
  "comm_srvs/visionLabelCfg")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'visionLabelCfg)))
  "Returns string type for a message object of type 'visionLabelCfg"
  "comm_srvs/visionLabelCfg")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<visionLabelCfg>)))
  "Returns md5sum for a message object of type '<visionLabelCfg>"
  "c57896cd544874b34ea95938d00716d9")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'visionLabelCfg)))
  "Returns md5sum for a message object of type 'visionLabelCfg"
  "c57896cd544874b34ea95938d00716d9")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<visionLabelCfg>)))
  "Returns full string definition for message of type '<visionLabelCfg>"
  (cl:format cl:nil "uint32[] vectDetLabel~%uint32[] vectSegLabel~%~%bool    isSupportThickPipe #是否支持粗水管类别~%float32 thickPipeWidth     #粗水管宽度阈值~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'visionLabelCfg)))
  "Returns full string definition for message of type 'visionLabelCfg"
  (cl:format cl:nil "uint32[] vectDetLabel~%uint32[] vectSegLabel~%~%bool    isSupportThickPipe #是否支持粗水管类别~%float32 thickPipeWidth     #粗水管宽度阈值~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <visionLabelCfg>))
  (cl:+ 0
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'vectDetLabel) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4)))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'vectSegLabel) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4)))
     1
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <visionLabelCfg>))
  "Converts a ROS message object to a list"
  (cl:list 'visionLabelCfg
    (cl:cons ':vectDetLabel (vectDetLabel msg))
    (cl:cons ':vectSegLabel (vectSegLabel msg))
    (cl:cons ':isSupportThickPipe (isSupportThickPipe msg))
    (cl:cons ':thickPipeWidth (thickPipeWidth msg))
))
