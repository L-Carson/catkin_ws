; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude dev_info.msg.html

(cl:defclass <dev_info> (roslisp-msg-protocol:ros-message)
  ((chargerMotorState
    :reader chargerMotorState
    :initarg :chargerMotorState
    :type cl:fixnum
    :initform 0)
   (chargerMotorTravel
    :reader chargerMotorTravel
    :initarg :chargerMotorTravel
    :type cl:integer
    :initform 0)
   (liftMotorState
    :reader liftMotorState
    :initarg :liftMotorState
    :type cl:fixnum
    :initform 0)
   (rollMotorState
    :reader rollMotorState
    :initarg :rollMotorState
    :type cl:fixnum
    :initform 0)
   (leftLiftMotorTravel
    :reader leftLiftMotorTravel
    :initarg :leftLiftMotorTravel
    :type cl:integer
    :initform 0)
   (rightLiftMotorTravel
    :reader rightLiftMotorTravel
    :initarg :rightLiftMotorTravel
    :type cl:integer
    :initform 0)
   (leftRollMotorTravel
    :reader leftRollMotorTravel
    :initarg :leftRollMotorTravel
    :type cl:integer
    :initform 0)
   (rightRollMotorTravel
    :reader rightRollMotorTravel
    :initarg :rightRollMotorTravel
    :type cl:integer
    :initform 0)
   (angleSensorOne
    :reader angleSensorOne
    :initarg :angleSensorOne
    :type cl:float
    :initform 0.0)
   (angleSensorTwo
    :reader angleSensorTwo
    :initarg :angleSensorTwo
    :type cl:float
    :initform 0.0)
   (isStationEmergeStop
    :reader isStationEmergeStop
    :initarg :isStationEmergeStop
    :type cl:fixnum
    :initform 0)
   (isManualChargeMode
    :reader isManualChargeMode
    :initarg :isManualChargeMode
    :type cl:fixnum
    :initform 0)
   (proximitySw
    :reader proximitySw
    :initarg :proximitySw
    :type cl:fixnum
    :initform 0)
   (chargeSw
    :reader chargeSw
    :initarg :chargeSw
    :type cl:fixnum
    :initform 0)
   (waterSw
    :reader waterSw
    :initarg :waterSw
    :type cl:fixnum
    :initform 0)
   (stationLockSw
    :reader stationLockSw
    :initarg :stationLockSw
    :type cl:fixnum
    :initform 0)
   (chargerTravelSw
    :reader chargerTravelSw
    :initarg :chargerTravelSw
    :type cl:fixnum
    :initform 0)
   (leftTravelSw
    :reader leftTravelSw
    :initarg :leftTravelSw
    :type cl:fixnum
    :initform 0)
   (rightTravelSw
    :reader rightTravelSw
    :initarg :rightTravelSw
    :type cl:fixnum
    :initform 0)
   (trashBinCoverOpenAngle
    :reader trashBinCoverOpenAngle
    :initarg :trashBinCoverOpenAngle
    :type cl:float
    :initform 0.0))
)

(cl:defclass dev_info (<dev_info>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <dev_info>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'dev_info)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<dev_info> is deprecated: use comm_msg-msg:dev_info instead.")))

(cl:ensure-generic-function 'chargerMotorState-val :lambda-list '(m))
(cl:defmethod chargerMotorState-val ((m <dev_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:chargerMotorState-val is deprecated.  Use comm_msg-msg:chargerMotorState instead.")
  (chargerMotorState m))

(cl:ensure-generic-function 'chargerMotorTravel-val :lambda-list '(m))
(cl:defmethod chargerMotorTravel-val ((m <dev_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:chargerMotorTravel-val is deprecated.  Use comm_msg-msg:chargerMotorTravel instead.")
  (chargerMotorTravel m))

(cl:ensure-generic-function 'liftMotorState-val :lambda-list '(m))
(cl:defmethod liftMotorState-val ((m <dev_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:liftMotorState-val is deprecated.  Use comm_msg-msg:liftMotorState instead.")
  (liftMotorState m))

(cl:ensure-generic-function 'rollMotorState-val :lambda-list '(m))
(cl:defmethod rollMotorState-val ((m <dev_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:rollMotorState-val is deprecated.  Use comm_msg-msg:rollMotorState instead.")
  (rollMotorState m))

(cl:ensure-generic-function 'leftLiftMotorTravel-val :lambda-list '(m))
(cl:defmethod leftLiftMotorTravel-val ((m <dev_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:leftLiftMotorTravel-val is deprecated.  Use comm_msg-msg:leftLiftMotorTravel instead.")
  (leftLiftMotorTravel m))

(cl:ensure-generic-function 'rightLiftMotorTravel-val :lambda-list '(m))
(cl:defmethod rightLiftMotorTravel-val ((m <dev_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:rightLiftMotorTravel-val is deprecated.  Use comm_msg-msg:rightLiftMotorTravel instead.")
  (rightLiftMotorTravel m))

(cl:ensure-generic-function 'leftRollMotorTravel-val :lambda-list '(m))
(cl:defmethod leftRollMotorTravel-val ((m <dev_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:leftRollMotorTravel-val is deprecated.  Use comm_msg-msg:leftRollMotorTravel instead.")
  (leftRollMotorTravel m))

(cl:ensure-generic-function 'rightRollMotorTravel-val :lambda-list '(m))
(cl:defmethod rightRollMotorTravel-val ((m <dev_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:rightRollMotorTravel-val is deprecated.  Use comm_msg-msg:rightRollMotorTravel instead.")
  (rightRollMotorTravel m))

(cl:ensure-generic-function 'angleSensorOne-val :lambda-list '(m))
(cl:defmethod angleSensorOne-val ((m <dev_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:angleSensorOne-val is deprecated.  Use comm_msg-msg:angleSensorOne instead.")
  (angleSensorOne m))

(cl:ensure-generic-function 'angleSensorTwo-val :lambda-list '(m))
(cl:defmethod angleSensorTwo-val ((m <dev_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:angleSensorTwo-val is deprecated.  Use comm_msg-msg:angleSensorTwo instead.")
  (angleSensorTwo m))

(cl:ensure-generic-function 'isStationEmergeStop-val :lambda-list '(m))
(cl:defmethod isStationEmergeStop-val ((m <dev_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isStationEmergeStop-val is deprecated.  Use comm_msg-msg:isStationEmergeStop instead.")
  (isStationEmergeStop m))

(cl:ensure-generic-function 'isManualChargeMode-val :lambda-list '(m))
(cl:defmethod isManualChargeMode-val ((m <dev_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isManualChargeMode-val is deprecated.  Use comm_msg-msg:isManualChargeMode instead.")
  (isManualChargeMode m))

(cl:ensure-generic-function 'proximitySw-val :lambda-list '(m))
(cl:defmethod proximitySw-val ((m <dev_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:proximitySw-val is deprecated.  Use comm_msg-msg:proximitySw instead.")
  (proximitySw m))

(cl:ensure-generic-function 'chargeSw-val :lambda-list '(m))
(cl:defmethod chargeSw-val ((m <dev_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:chargeSw-val is deprecated.  Use comm_msg-msg:chargeSw instead.")
  (chargeSw m))

(cl:ensure-generic-function 'waterSw-val :lambda-list '(m))
(cl:defmethod waterSw-val ((m <dev_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:waterSw-val is deprecated.  Use comm_msg-msg:waterSw instead.")
  (waterSw m))

(cl:ensure-generic-function 'stationLockSw-val :lambda-list '(m))
(cl:defmethod stationLockSw-val ((m <dev_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:stationLockSw-val is deprecated.  Use comm_msg-msg:stationLockSw instead.")
  (stationLockSw m))

(cl:ensure-generic-function 'chargerTravelSw-val :lambda-list '(m))
(cl:defmethod chargerTravelSw-val ((m <dev_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:chargerTravelSw-val is deprecated.  Use comm_msg-msg:chargerTravelSw instead.")
  (chargerTravelSw m))

(cl:ensure-generic-function 'leftTravelSw-val :lambda-list '(m))
(cl:defmethod leftTravelSw-val ((m <dev_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:leftTravelSw-val is deprecated.  Use comm_msg-msg:leftTravelSw instead.")
  (leftTravelSw m))

(cl:ensure-generic-function 'rightTravelSw-val :lambda-list '(m))
(cl:defmethod rightTravelSw-val ((m <dev_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:rightTravelSw-val is deprecated.  Use comm_msg-msg:rightTravelSw instead.")
  (rightTravelSw m))

(cl:ensure-generic-function 'trashBinCoverOpenAngle-val :lambda-list '(m))
(cl:defmethod trashBinCoverOpenAngle-val ((m <dev_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:trashBinCoverOpenAngle-val is deprecated.  Use comm_msg-msg:trashBinCoverOpenAngle instead.")
  (trashBinCoverOpenAngle m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <dev_info>) ostream)
  "Serializes a message object of type '<dev_info>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'chargerMotorState)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'chargerMotorTravel)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'chargerMotorTravel)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'chargerMotorTravel)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'chargerMotorTravel)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'liftMotorState)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'rollMotorState)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'leftLiftMotorTravel)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'leftLiftMotorTravel)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'leftLiftMotorTravel)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'leftLiftMotorTravel)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'rightLiftMotorTravel)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'rightLiftMotorTravel)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'rightLiftMotorTravel)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'rightLiftMotorTravel)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'leftRollMotorTravel)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'leftRollMotorTravel)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'leftRollMotorTravel)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'leftRollMotorTravel)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'rightRollMotorTravel)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'rightRollMotorTravel)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'rightRollMotorTravel)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'rightRollMotorTravel)) ostream)
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'angleSensorOne))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'angleSensorTwo))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isStationEmergeStop)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isManualChargeMode)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'proximitySw)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'chargeSw)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'waterSw)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'stationLockSw)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'chargerTravelSw)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'leftTravelSw)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'rightTravelSw)) ostream)
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'trashBinCoverOpenAngle))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <dev_info>) istream)
  "Deserializes a message object of type '<dev_info>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'chargerMotorState)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'chargerMotorTravel)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'chargerMotorTravel)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'chargerMotorTravel)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'chargerMotorTravel)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'liftMotorState)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'rollMotorState)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'leftLiftMotorTravel)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'leftLiftMotorTravel)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'leftLiftMotorTravel)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'leftLiftMotorTravel)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'rightLiftMotorTravel)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'rightLiftMotorTravel)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'rightLiftMotorTravel)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'rightLiftMotorTravel)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'leftRollMotorTravel)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'leftRollMotorTravel)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'leftRollMotorTravel)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'leftRollMotorTravel)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'rightRollMotorTravel)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'rightRollMotorTravel)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'rightRollMotorTravel)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'rightRollMotorTravel)) (cl:read-byte istream))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'angleSensorOne) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'angleSensorTwo) (roslisp-utils:decode-single-float-bits bits)))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isStationEmergeStop)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isManualChargeMode)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'proximitySw)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'chargeSw)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'waterSw)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'stationLockSw)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'chargerTravelSw)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'leftTravelSw)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'rightTravelSw)) (cl:read-byte istream))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'trashBinCoverOpenAngle) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<dev_info>)))
  "Returns string type for a message object of type '<dev_info>"
  "comm_msg/dev_info")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'dev_info)))
  "Returns string type for a message object of type 'dev_info"
  "comm_msg/dev_info")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<dev_info>)))
  "Returns md5sum for a message object of type '<dev_info>"
  "17ff66948ac49c3e03faf3f4e7f62471")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'dev_info)))
  "Returns md5sum for a message object of type 'dev_info"
  "17ff66948ac49c3e03faf3f4e7f62471")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<dev_info>)))
  "Returns full string definition for message of type '<dev_info>"
  (cl:format cl:nil "# charger state~%uint8  chargerMotorState~%uint32 chargerMotorTravel~%~%# trash box state~%uint8   liftMotorState~%uint8   rollMotorState~%uint32  leftLiftMotorTravel~%uint32  rightLiftMotorTravel~%uint32  leftRollMotorTravel~%uint32  rightRollMotorTravel~%float32 angleSensorOne~%float32 angleSensorTwo~%~%# dev state~%uint8 isStationEmergeStop~%uint8 isManualChargeMode~%uint8 proximitySw~%uint8 chargeSw~%uint8 waterSw~%uint8 stationLockSw~%uint8 chargerTravelSw~%uint8 leftTravelSw~%uint8 rightTravelSw~%~%# trash cover state~%float32 trashBinCoverOpenAngle~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'dev_info)))
  "Returns full string definition for message of type 'dev_info"
  (cl:format cl:nil "# charger state~%uint8  chargerMotorState~%uint32 chargerMotorTravel~%~%# trash box state~%uint8   liftMotorState~%uint8   rollMotorState~%uint32  leftLiftMotorTravel~%uint32  rightLiftMotorTravel~%uint32  leftRollMotorTravel~%uint32  rightRollMotorTravel~%float32 angleSensorOne~%float32 angleSensorTwo~%~%# dev state~%uint8 isStationEmergeStop~%uint8 isManualChargeMode~%uint8 proximitySw~%uint8 chargeSw~%uint8 waterSw~%uint8 stationLockSw~%uint8 chargerTravelSw~%uint8 leftTravelSw~%uint8 rightTravelSw~%~%# trash cover state~%float32 trashBinCoverOpenAngle~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <dev_info>))
  (cl:+ 0
     1
     4
     1
     1
     4
     4
     4
     4
     4
     4
     1
     1
     1
     1
     1
     1
     1
     1
     1
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <dev_info>))
  "Converts a ROS message object to a list"
  (cl:list 'dev_info
    (cl:cons ':chargerMotorState (chargerMotorState msg))
    (cl:cons ':chargerMotorTravel (chargerMotorTravel msg))
    (cl:cons ':liftMotorState (liftMotorState msg))
    (cl:cons ':rollMotorState (rollMotorState msg))
    (cl:cons ':leftLiftMotorTravel (leftLiftMotorTravel msg))
    (cl:cons ':rightLiftMotorTravel (rightLiftMotorTravel msg))
    (cl:cons ':leftRollMotorTravel (leftRollMotorTravel msg))
    (cl:cons ':rightRollMotorTravel (rightRollMotorTravel msg))
    (cl:cons ':angleSensorOne (angleSensorOne msg))
    (cl:cons ':angleSensorTwo (angleSensorTwo msg))
    (cl:cons ':isStationEmergeStop (isStationEmergeStop msg))
    (cl:cons ':isManualChargeMode (isManualChargeMode msg))
    (cl:cons ':proximitySw (proximitySw msg))
    (cl:cons ':chargeSw (chargeSw msg))
    (cl:cons ':waterSw (waterSw msg))
    (cl:cons ':stationLockSw (stationLockSw msg))
    (cl:cons ':chargerTravelSw (chargerTravelSw msg))
    (cl:cons ':leftTravelSw (leftTravelSw msg))
    (cl:cons ':rightTravelSw (rightTravelSw msg))
    (cl:cons ':trashBinCoverOpenAngle (trashBinCoverOpenAngle msg))
))
