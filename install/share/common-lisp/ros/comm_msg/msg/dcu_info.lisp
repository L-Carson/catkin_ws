; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude dcu_info.msg.html

(cl:defclass <dcu_info> (roslisp-msg-protocol:ros-message)
  ((batteryPowerLeftRate
    :reader batteryPowerLeftRate
    :initarg :batteryPowerLeftRate
    :type cl:fixnum
    :initform 0)
   (batteryTemperature
    :reader batteryTemperature
    :initarg :batteryTemperature
    :type cl:integer
    :initform 0)
   (batteryVoltage
    :reader batteryVoltage
    :initarg :batteryVoltage
    :type cl:float
    :initform 0.0)
   (isAutoMode
    :reader isAutoMode
    :initarg :isAutoMode
    :type cl:fixnum
    :initform 0)
   (isRemoteControl
    :reader isRemoteControl
    :initarg :isRemoteControl
    :type cl:fixnum
    :initform 0)
   (isStopByEmergeSw
    :reader isStopByEmergeSw
    :initarg :isStopByEmergeSw
    :type cl:fixnum
    :initform 0)
   (isStopByFrontEdgeContact
    :reader isStopByFrontEdgeContact
    :initarg :isStopByFrontEdgeContact
    :type cl:fixnum
    :initform 0)
   (isStopByBackEdgeContact
    :reader isStopByBackEdgeContact
    :initarg :isStopByBackEdgeContact
    :type cl:fixnum
    :initform 0)
   (isRaining
    :reader isRaining
    :initarg :isRaining
    :type cl:fixnum
    :initform 0)
   (isWaterEnough
    :reader isWaterEnough
    :initarg :isWaterEnough
    :type cl:fixnum
    :initform 0)
   (isRubbishFull
    :reader isRubbishFull
    :initarg :isRubbishFull
    :type cl:fixnum
    :initform 0)
   (isGarbageCapacityAbn
    :reader isGarbageCapacityAbn
    :initarg :isGarbageCapacityAbn
    :type cl:fixnum
    :initform 0)
   (isDcuFault
    :reader isDcuFault
    :initarg :isDcuFault
    :type cl:fixnum
    :initform 0)
   (isUltraFault
    :reader isUltraFault
    :initarg :isUltraFault
    :type cl:fixnum
    :initform 0)
   (isGpsFault
    :reader isGpsFault
    :initarg :isGpsFault
    :type cl:fixnum
    :initform 0)
   (isMoveMotorFault
    :reader isMoveMotorFault
    :initarg :isMoveMotorFault
    :type cl:fixnum
    :initform 0)
   (isTurnMotorFault
    :reader isTurnMotorFault
    :initarg :isTurnMotorFault
    :type cl:fixnum
    :initform 0)
   (isBrakeSysFault
    :reader isBrakeSysFault
    :initarg :isBrakeSysFault
    :type cl:fixnum
    :initform 0)
   (isBrushSysFault
    :reader isBrushSysFault
    :initarg :isBrushSysFault
    :type cl:fixnum
    :initform 0)
   (isFanSysFault
    :reader isFanSysFault
    :initarg :isFanSysFault
    :type cl:fixnum
    :initform 0)
   (isShakeSysFault
    :reader isShakeSysFault
    :initarg :isShakeSysFault
    :type cl:fixnum
    :initform 0)
   (leftStretchState
    :reader leftStretchState
    :initarg :leftStretchState
    :type cl:fixnum
    :initform 0)
   (rightStretchState
    :reader rightStretchState
    :initarg :rightStretchState
    :type cl:fixnum
    :initform 0)
   (backElevatorState
    :reader backElevatorState
    :initarg :backElevatorState
    :type cl:fixnum
    :initform 0)
   (frontElevatorState
    :reader frontElevatorState
    :initarg :frontElevatorState
    :type cl:fixnum
    :initform 0)
   (baffleState
    :reader baffleState
    :initarg :baffleState
    :type cl:fixnum
    :initform 0)
   (rollBrushElevatorState
    :reader rollBrushElevatorState
    :initarg :rollBrushElevatorState
    :type cl:fixnum
    :initform 0)
   (isFrontBrushOpen
    :reader isFrontBrushOpen
    :initarg :isFrontBrushOpen
    :type cl:fixnum
    :initform 0)
   (isBackBrushOpen
    :reader isBackBrushOpen
    :initarg :isBackBrushOpen
    :type cl:fixnum
    :initform 0)
   (isRollBrushOpen
    :reader isRollBrushOpen
    :initarg :isRollBrushOpen
    :type cl:fixnum
    :initform 0)
   (trashBinCoverState
    :reader trashBinCoverState
    :initarg :trashBinCoverState
    :type cl:fixnum
    :initform 0)
   (trashBagClampState
    :reader trashBagClampState
    :initarg :trashBagClampState
    :type cl:fixnum
    :initform 0)
   (trashBagSubterreneState
    :reader trashBagSubterreneState
    :initarg :trashBagSubterreneState
    :type cl:fixnum
    :initform 0)
   (trashBagDrawState
    :reader trashBagDrawState
    :initarg :trashBagDrawState
    :type cl:fixnum
    :initform 0)
   (isTrashBagExist
    :reader isTrashBagExist
    :initarg :isTrashBagExist
    :type cl:fixnum
    :initform 0)
   (isTrashBagThrownOut
    :reader isTrashBagThrownOut
    :initarg :isTrashBagThrownOut
    :type cl:fixnum
    :initform 0)
   (ultraDistToTrashBag
    :reader ultraDistToTrashBag
    :initarg :ultraDistToTrashBag
    :type cl:float
    :initform 0.0)
   (chargingDoorState
    :reader chargingDoorState
    :initarg :chargingDoorState
    :type cl:fixnum
    :initform 0))
)

(cl:defclass dcu_info (<dcu_info>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <dcu_info>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'dcu_info)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<dcu_info> is deprecated: use comm_msg-msg:dcu_info instead.")))

(cl:ensure-generic-function 'batteryPowerLeftRate-val :lambda-list '(m))
(cl:defmethod batteryPowerLeftRate-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:batteryPowerLeftRate-val is deprecated.  Use comm_msg-msg:batteryPowerLeftRate instead.")
  (batteryPowerLeftRate m))

(cl:ensure-generic-function 'batteryTemperature-val :lambda-list '(m))
(cl:defmethod batteryTemperature-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:batteryTemperature-val is deprecated.  Use comm_msg-msg:batteryTemperature instead.")
  (batteryTemperature m))

(cl:ensure-generic-function 'batteryVoltage-val :lambda-list '(m))
(cl:defmethod batteryVoltage-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:batteryVoltage-val is deprecated.  Use comm_msg-msg:batteryVoltage instead.")
  (batteryVoltage m))

(cl:ensure-generic-function 'isAutoMode-val :lambda-list '(m))
(cl:defmethod isAutoMode-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isAutoMode-val is deprecated.  Use comm_msg-msg:isAutoMode instead.")
  (isAutoMode m))

(cl:ensure-generic-function 'isRemoteControl-val :lambda-list '(m))
(cl:defmethod isRemoteControl-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isRemoteControl-val is deprecated.  Use comm_msg-msg:isRemoteControl instead.")
  (isRemoteControl m))

(cl:ensure-generic-function 'isStopByEmergeSw-val :lambda-list '(m))
(cl:defmethod isStopByEmergeSw-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isStopByEmergeSw-val is deprecated.  Use comm_msg-msg:isStopByEmergeSw instead.")
  (isStopByEmergeSw m))

(cl:ensure-generic-function 'isStopByFrontEdgeContact-val :lambda-list '(m))
(cl:defmethod isStopByFrontEdgeContact-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isStopByFrontEdgeContact-val is deprecated.  Use comm_msg-msg:isStopByFrontEdgeContact instead.")
  (isStopByFrontEdgeContact m))

(cl:ensure-generic-function 'isStopByBackEdgeContact-val :lambda-list '(m))
(cl:defmethod isStopByBackEdgeContact-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isStopByBackEdgeContact-val is deprecated.  Use comm_msg-msg:isStopByBackEdgeContact instead.")
  (isStopByBackEdgeContact m))

(cl:ensure-generic-function 'isRaining-val :lambda-list '(m))
(cl:defmethod isRaining-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isRaining-val is deprecated.  Use comm_msg-msg:isRaining instead.")
  (isRaining m))

(cl:ensure-generic-function 'isWaterEnough-val :lambda-list '(m))
(cl:defmethod isWaterEnough-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isWaterEnough-val is deprecated.  Use comm_msg-msg:isWaterEnough instead.")
  (isWaterEnough m))

(cl:ensure-generic-function 'isRubbishFull-val :lambda-list '(m))
(cl:defmethod isRubbishFull-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isRubbishFull-val is deprecated.  Use comm_msg-msg:isRubbishFull instead.")
  (isRubbishFull m))

(cl:ensure-generic-function 'isGarbageCapacityAbn-val :lambda-list '(m))
(cl:defmethod isGarbageCapacityAbn-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isGarbageCapacityAbn-val is deprecated.  Use comm_msg-msg:isGarbageCapacityAbn instead.")
  (isGarbageCapacityAbn m))

(cl:ensure-generic-function 'isDcuFault-val :lambda-list '(m))
(cl:defmethod isDcuFault-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isDcuFault-val is deprecated.  Use comm_msg-msg:isDcuFault instead.")
  (isDcuFault m))

(cl:ensure-generic-function 'isUltraFault-val :lambda-list '(m))
(cl:defmethod isUltraFault-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isUltraFault-val is deprecated.  Use comm_msg-msg:isUltraFault instead.")
  (isUltraFault m))

(cl:ensure-generic-function 'isGpsFault-val :lambda-list '(m))
(cl:defmethod isGpsFault-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isGpsFault-val is deprecated.  Use comm_msg-msg:isGpsFault instead.")
  (isGpsFault m))

(cl:ensure-generic-function 'isMoveMotorFault-val :lambda-list '(m))
(cl:defmethod isMoveMotorFault-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isMoveMotorFault-val is deprecated.  Use comm_msg-msg:isMoveMotorFault instead.")
  (isMoveMotorFault m))

(cl:ensure-generic-function 'isTurnMotorFault-val :lambda-list '(m))
(cl:defmethod isTurnMotorFault-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isTurnMotorFault-val is deprecated.  Use comm_msg-msg:isTurnMotorFault instead.")
  (isTurnMotorFault m))

(cl:ensure-generic-function 'isBrakeSysFault-val :lambda-list '(m))
(cl:defmethod isBrakeSysFault-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isBrakeSysFault-val is deprecated.  Use comm_msg-msg:isBrakeSysFault instead.")
  (isBrakeSysFault m))

(cl:ensure-generic-function 'isBrushSysFault-val :lambda-list '(m))
(cl:defmethod isBrushSysFault-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isBrushSysFault-val is deprecated.  Use comm_msg-msg:isBrushSysFault instead.")
  (isBrushSysFault m))

(cl:ensure-generic-function 'isFanSysFault-val :lambda-list '(m))
(cl:defmethod isFanSysFault-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isFanSysFault-val is deprecated.  Use comm_msg-msg:isFanSysFault instead.")
  (isFanSysFault m))

(cl:ensure-generic-function 'isShakeSysFault-val :lambda-list '(m))
(cl:defmethod isShakeSysFault-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isShakeSysFault-val is deprecated.  Use comm_msg-msg:isShakeSysFault instead.")
  (isShakeSysFault m))

(cl:ensure-generic-function 'leftStretchState-val :lambda-list '(m))
(cl:defmethod leftStretchState-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:leftStretchState-val is deprecated.  Use comm_msg-msg:leftStretchState instead.")
  (leftStretchState m))

(cl:ensure-generic-function 'rightStretchState-val :lambda-list '(m))
(cl:defmethod rightStretchState-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:rightStretchState-val is deprecated.  Use comm_msg-msg:rightStretchState instead.")
  (rightStretchState m))

(cl:ensure-generic-function 'backElevatorState-val :lambda-list '(m))
(cl:defmethod backElevatorState-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:backElevatorState-val is deprecated.  Use comm_msg-msg:backElevatorState instead.")
  (backElevatorState m))

(cl:ensure-generic-function 'frontElevatorState-val :lambda-list '(m))
(cl:defmethod frontElevatorState-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:frontElevatorState-val is deprecated.  Use comm_msg-msg:frontElevatorState instead.")
  (frontElevatorState m))

(cl:ensure-generic-function 'baffleState-val :lambda-list '(m))
(cl:defmethod baffleState-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:baffleState-val is deprecated.  Use comm_msg-msg:baffleState instead.")
  (baffleState m))

(cl:ensure-generic-function 'rollBrushElevatorState-val :lambda-list '(m))
(cl:defmethod rollBrushElevatorState-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:rollBrushElevatorState-val is deprecated.  Use comm_msg-msg:rollBrushElevatorState instead.")
  (rollBrushElevatorState m))

(cl:ensure-generic-function 'isFrontBrushOpen-val :lambda-list '(m))
(cl:defmethod isFrontBrushOpen-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isFrontBrushOpen-val is deprecated.  Use comm_msg-msg:isFrontBrushOpen instead.")
  (isFrontBrushOpen m))

(cl:ensure-generic-function 'isBackBrushOpen-val :lambda-list '(m))
(cl:defmethod isBackBrushOpen-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isBackBrushOpen-val is deprecated.  Use comm_msg-msg:isBackBrushOpen instead.")
  (isBackBrushOpen m))

(cl:ensure-generic-function 'isRollBrushOpen-val :lambda-list '(m))
(cl:defmethod isRollBrushOpen-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isRollBrushOpen-val is deprecated.  Use comm_msg-msg:isRollBrushOpen instead.")
  (isRollBrushOpen m))

(cl:ensure-generic-function 'trashBinCoverState-val :lambda-list '(m))
(cl:defmethod trashBinCoverState-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:trashBinCoverState-val is deprecated.  Use comm_msg-msg:trashBinCoverState instead.")
  (trashBinCoverState m))

(cl:ensure-generic-function 'trashBagClampState-val :lambda-list '(m))
(cl:defmethod trashBagClampState-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:trashBagClampState-val is deprecated.  Use comm_msg-msg:trashBagClampState instead.")
  (trashBagClampState m))

(cl:ensure-generic-function 'trashBagSubterreneState-val :lambda-list '(m))
(cl:defmethod trashBagSubterreneState-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:trashBagSubterreneState-val is deprecated.  Use comm_msg-msg:trashBagSubterreneState instead.")
  (trashBagSubterreneState m))

(cl:ensure-generic-function 'trashBagDrawState-val :lambda-list '(m))
(cl:defmethod trashBagDrawState-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:trashBagDrawState-val is deprecated.  Use comm_msg-msg:trashBagDrawState instead.")
  (trashBagDrawState m))

(cl:ensure-generic-function 'isTrashBagExist-val :lambda-list '(m))
(cl:defmethod isTrashBagExist-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isTrashBagExist-val is deprecated.  Use comm_msg-msg:isTrashBagExist instead.")
  (isTrashBagExist m))

(cl:ensure-generic-function 'isTrashBagThrownOut-val :lambda-list '(m))
(cl:defmethod isTrashBagThrownOut-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isTrashBagThrownOut-val is deprecated.  Use comm_msg-msg:isTrashBagThrownOut instead.")
  (isTrashBagThrownOut m))

(cl:ensure-generic-function 'ultraDistToTrashBag-val :lambda-list '(m))
(cl:defmethod ultraDistToTrashBag-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:ultraDistToTrashBag-val is deprecated.  Use comm_msg-msg:ultraDistToTrashBag instead.")
  (ultraDistToTrashBag m))

(cl:ensure-generic-function 'chargingDoorState-val :lambda-list '(m))
(cl:defmethod chargingDoorState-val ((m <dcu_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:chargingDoorState-val is deprecated.  Use comm_msg-msg:chargingDoorState instead.")
  (chargingDoorState m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <dcu_info>) ostream)
  "Serializes a message object of type '<dcu_info>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'batteryPowerLeftRate)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'batteryTemperature)) ostream)
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'batteryVoltage))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isAutoMode)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isRemoteControl)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isStopByEmergeSw)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isStopByFrontEdgeContact)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isStopByBackEdgeContact)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isRaining)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isWaterEnough)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isRubbishFull)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isGarbageCapacityAbn)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isDcuFault)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isUltraFault)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isGpsFault)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isMoveMotorFault)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isTurnMotorFault)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isBrakeSysFault)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isBrushSysFault)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isFanSysFault)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isShakeSysFault)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'leftStretchState)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'rightStretchState)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'backElevatorState)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'frontElevatorState)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'baffleState)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'rollBrushElevatorState)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isFrontBrushOpen)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isBackBrushOpen)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isRollBrushOpen)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'trashBinCoverState)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'trashBagClampState)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'trashBagSubterreneState)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'trashBagDrawState)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isTrashBagExist)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isTrashBagThrownOut)) ostream)
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'ultraDistToTrashBag))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'chargingDoorState)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <dcu_info>) istream)
  "Deserializes a message object of type '<dcu_info>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'batteryPowerLeftRate)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'batteryTemperature)) (cl:read-byte istream))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'batteryVoltage) (roslisp-utils:decode-single-float-bits bits)))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isAutoMode)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isRemoteControl)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isStopByEmergeSw)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isStopByFrontEdgeContact)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isStopByBackEdgeContact)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isRaining)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isWaterEnough)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isRubbishFull)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isGarbageCapacityAbn)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isDcuFault)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isUltraFault)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isGpsFault)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isMoveMotorFault)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isTurnMotorFault)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isBrakeSysFault)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isBrushSysFault)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isFanSysFault)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isShakeSysFault)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'leftStretchState)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'rightStretchState)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'backElevatorState)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'frontElevatorState)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'baffleState)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'rollBrushElevatorState)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isFrontBrushOpen)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isBackBrushOpen)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isRollBrushOpen)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'trashBinCoverState)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'trashBagClampState)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'trashBagSubterreneState)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'trashBagDrawState)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isTrashBagExist)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isTrashBagThrownOut)) (cl:read-byte istream))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'ultraDistToTrashBag) (roslisp-utils:decode-single-float-bits bits)))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'chargingDoorState)) (cl:read-byte istream))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<dcu_info>)))
  "Returns string type for a message object of type '<dcu_info>"
  "comm_msg/dcu_info")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'dcu_info)))
  "Returns string type for a message object of type 'dcu_info"
  "comm_msg/dcu_info")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<dcu_info>)))
  "Returns md5sum for a message object of type '<dcu_info>"
  "8ec425a682ee94ceaf30e5a49484bd60")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'dcu_info)))
  "Returns md5sum for a message object of type 'dcu_info"
  "8ec425a682ee94ceaf30e5a49484bd60")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<dcu_info>)))
  "Returns full string definition for message of type '<dcu_info>"
  (cl:format cl:nil "# battery state~%uint8   batteryPowerLeftRate~%char    batteryTemperature~%float32 batteryVoltage~%~%# move state~%uint8 isAutoMode~%uint8 isRemoteControl~%uint8 isStopByEmergeSw~%uint8 isStopByFrontEdgeContact~%uint8 isStopByBackEdgeContact~%~%# other state~%uint8 isRaining~%uint8 isWaterEnough~%uint8 isRubbishFull~%uint8 isGarbageCapacityAbn~%~%# module fault state~%uint8 isDcuFault~%uint8 isUltraFault~%uint8 isGpsFault~%uint8 isMoveMotorFault~%uint8 isTurnMotorFault~%uint8 isBrakeSysFault~%uint8 isBrushSysFault~%uint8 isFanSysFault~%uint8 isShakeSysFault~%~%# stretch state~%uint8 leftStretchState~%uint8 rightStretchState~%uint8 backElevatorState~%uint8 frontElevatorState~%uint8 baffleState~%uint8 rollBrushElevatorState~%uint8 isFrontBrushOpen~%uint8 isBackBrushOpen~%uint8 isRollBrushOpen~%~%# Trash Pack~%uint8 trashBinCoverState~%uint8 trashBagClampState~%uint8 trashBagSubterreneState~%uint8 trashBagDrawState~%uint8 isTrashBagExist~%uint8 isTrashBagThrownOut~%float32 ultraDistToTrashBag~%~%# Charging Door State~%uint8 chargingDoorState~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'dcu_info)))
  "Returns full string definition for message of type 'dcu_info"
  (cl:format cl:nil "# battery state~%uint8   batteryPowerLeftRate~%char    batteryTemperature~%float32 batteryVoltage~%~%# move state~%uint8 isAutoMode~%uint8 isRemoteControl~%uint8 isStopByEmergeSw~%uint8 isStopByFrontEdgeContact~%uint8 isStopByBackEdgeContact~%~%# other state~%uint8 isRaining~%uint8 isWaterEnough~%uint8 isRubbishFull~%uint8 isGarbageCapacityAbn~%~%# module fault state~%uint8 isDcuFault~%uint8 isUltraFault~%uint8 isGpsFault~%uint8 isMoveMotorFault~%uint8 isTurnMotorFault~%uint8 isBrakeSysFault~%uint8 isBrushSysFault~%uint8 isFanSysFault~%uint8 isShakeSysFault~%~%# stretch state~%uint8 leftStretchState~%uint8 rightStretchState~%uint8 backElevatorState~%uint8 frontElevatorState~%uint8 baffleState~%uint8 rollBrushElevatorState~%uint8 isFrontBrushOpen~%uint8 isBackBrushOpen~%uint8 isRollBrushOpen~%~%# Trash Pack~%uint8 trashBinCoverState~%uint8 trashBagClampState~%uint8 trashBagSubterreneState~%uint8 trashBagDrawState~%uint8 isTrashBagExist~%uint8 isTrashBagThrownOut~%float32 ultraDistToTrashBag~%~%# Charging Door State~%uint8 chargingDoorState~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <dcu_info>))
  (cl:+ 0
     1
     1
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
     1
     1
     1
     1
     1
     1
     1
     1
     1
     1
     1
     1
     1
     1
     1
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
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <dcu_info>))
  "Converts a ROS message object to a list"
  (cl:list 'dcu_info
    (cl:cons ':batteryPowerLeftRate (batteryPowerLeftRate msg))
    (cl:cons ':batteryTemperature (batteryTemperature msg))
    (cl:cons ':batteryVoltage (batteryVoltage msg))
    (cl:cons ':isAutoMode (isAutoMode msg))
    (cl:cons ':isRemoteControl (isRemoteControl msg))
    (cl:cons ':isStopByEmergeSw (isStopByEmergeSw msg))
    (cl:cons ':isStopByFrontEdgeContact (isStopByFrontEdgeContact msg))
    (cl:cons ':isStopByBackEdgeContact (isStopByBackEdgeContact msg))
    (cl:cons ':isRaining (isRaining msg))
    (cl:cons ':isWaterEnough (isWaterEnough msg))
    (cl:cons ':isRubbishFull (isRubbishFull msg))
    (cl:cons ':isGarbageCapacityAbn (isGarbageCapacityAbn msg))
    (cl:cons ':isDcuFault (isDcuFault msg))
    (cl:cons ':isUltraFault (isUltraFault msg))
    (cl:cons ':isGpsFault (isGpsFault msg))
    (cl:cons ':isMoveMotorFault (isMoveMotorFault msg))
    (cl:cons ':isTurnMotorFault (isTurnMotorFault msg))
    (cl:cons ':isBrakeSysFault (isBrakeSysFault msg))
    (cl:cons ':isBrushSysFault (isBrushSysFault msg))
    (cl:cons ':isFanSysFault (isFanSysFault msg))
    (cl:cons ':isShakeSysFault (isShakeSysFault msg))
    (cl:cons ':leftStretchState (leftStretchState msg))
    (cl:cons ':rightStretchState (rightStretchState msg))
    (cl:cons ':backElevatorState (backElevatorState msg))
    (cl:cons ':frontElevatorState (frontElevatorState msg))
    (cl:cons ':baffleState (baffleState msg))
    (cl:cons ':rollBrushElevatorState (rollBrushElevatorState msg))
    (cl:cons ':isFrontBrushOpen (isFrontBrushOpen msg))
    (cl:cons ':isBackBrushOpen (isBackBrushOpen msg))
    (cl:cons ':isRollBrushOpen (isRollBrushOpen msg))
    (cl:cons ':trashBinCoverState (trashBinCoverState msg))
    (cl:cons ':trashBagClampState (trashBagClampState msg))
    (cl:cons ':trashBagSubterreneState (trashBagSubterreneState msg))
    (cl:cons ':trashBagDrawState (trashBagDrawState msg))
    (cl:cons ':isTrashBagExist (isTrashBagExist msg))
    (cl:cons ':isTrashBagThrownOut (isTrashBagThrownOut msg))
    (cl:cons ':ultraDistToTrashBag (ultraDistToTrashBag msg))
    (cl:cons ':chargingDoorState (chargingDoorState msg))
))
