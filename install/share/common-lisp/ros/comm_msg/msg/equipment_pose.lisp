; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude equipment_pose.msg.html

(cl:defclass <equipment_pose> (roslisp-msg-protocol:ros-message)
  ((status
    :reader status
    :initarg :status
    :type cl:fixnum
    :initform 0)
   (posX
    :reader posX
    :initarg :posX
    :type cl:float
    :initform 0.0)
   (posY
    :reader posY
    :initarg :posY
    :type cl:float
    :initform 0.0)
   (dirAngle
    :reader dirAngle
    :initarg :dirAngle
    :type cl:float
    :initform 0.0))
)

(cl:defclass equipment_pose (<equipment_pose>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <equipment_pose>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'equipment_pose)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<equipment_pose> is deprecated: use comm_msg-msg:equipment_pose instead.")))

(cl:ensure-generic-function 'status-val :lambda-list '(m))
(cl:defmethod status-val ((m <equipment_pose>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:status-val is deprecated.  Use comm_msg-msg:status instead.")
  (status m))

(cl:ensure-generic-function 'posX-val :lambda-list '(m))
(cl:defmethod posX-val ((m <equipment_pose>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:posX-val is deprecated.  Use comm_msg-msg:posX instead.")
  (posX m))

(cl:ensure-generic-function 'posY-val :lambda-list '(m))
(cl:defmethod posY-val ((m <equipment_pose>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:posY-val is deprecated.  Use comm_msg-msg:posY instead.")
  (posY m))

(cl:ensure-generic-function 'dirAngle-val :lambda-list '(m))
(cl:defmethod dirAngle-val ((m <equipment_pose>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:dirAngle-val is deprecated.  Use comm_msg-msg:dirAngle instead.")
  (dirAngle m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <equipment_pose>) ostream)
  "Serializes a message object of type '<equipment_pose>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'status)) ostream)
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'posX))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'posY))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'dirAngle))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <equipment_pose>) istream)
  "Deserializes a message object of type '<equipment_pose>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'status)) (cl:read-byte istream))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'posX) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'posY) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'dirAngle) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<equipment_pose>)))
  "Returns string type for a message object of type '<equipment_pose>"
  "comm_msg/equipment_pose")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'equipment_pose)))
  "Returns string type for a message object of type 'equipment_pose"
  "comm_msg/equipment_pose")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<equipment_pose>)))
  "Returns md5sum for a message object of type '<equipment_pose>"
  "6f56144f592d31a738a420cd57e47f50")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'equipment_pose)))
  "Returns md5sum for a message object of type 'equipment_pose"
  "6f56144f592d31a738a420cd57e47f50")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<equipment_pose>)))
  "Returns full string definition for message of type '<equipment_pose>"
  (cl:format cl:nil "# location status, 1 normal, 0 abnormal~%uint8   status~%float32 posX~%float32 posY~%float32 dirAngle~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'equipment_pose)))
  "Returns full string definition for message of type 'equipment_pose"
  (cl:format cl:nil "# location status, 1 normal, 0 abnormal~%uint8   status~%float32 posX~%float32 posY~%float32 dirAngle~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <equipment_pose>))
  (cl:+ 0
     1
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <equipment_pose>))
  "Converts a ROS message object to a list"
  (cl:list 'equipment_pose
    (cl:cons ':status (status msg))
    (cl:cons ':posX (posX msg))
    (cl:cons ':posY (posY msg))
    (cl:cons ':dirAngle (dirAngle msg))
))
