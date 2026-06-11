;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::dcu_info)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'dcu_info (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::DCU_INFO")
  (make-package "COMM_MSG::DCU_INFO"))

(in-package "ROS")
;;//! \htmlinclude dcu_info.msg.html


(defclass comm_msg::dcu_info
  :super ros::object
  :slots (_batteryPowerLeftRate _batteryTemperature _batteryVoltage _isAutoMode _isRemoteControl _isStopByEmergeSw _isStopByFrontEdgeContact _isStopByBackEdgeContact _isRaining _isWaterEnough _isRubbishFull _isGarbageCapacityAbn _isDcuFault _isUltraFault _isGpsFault _isMoveMotorFault _isTurnMotorFault _isBrakeSysFault _isBrushSysFault _isFanSysFault _isShakeSysFault _leftStretchState _rightStretchState _backElevatorState _frontElevatorState _baffleState _rollBrushElevatorState _isFrontBrushOpen _isBackBrushOpen _isRollBrushOpen _trashBinCoverState _trashBagClampState _trashBagSubterreneState _trashBagDrawState _isTrashBagExist _isTrashBagThrownOut _ultraDistToTrashBag _chargingDoorState ))

(defmethod comm_msg::dcu_info
  (:init
   (&key
    ((:batteryPowerLeftRate __batteryPowerLeftRate) 0)
    ((:batteryTemperature __batteryTemperature) 0)
    ((:batteryVoltage __batteryVoltage) 0.0)
    ((:isAutoMode __isAutoMode) 0)
    ((:isRemoteControl __isRemoteControl) 0)
    ((:isStopByEmergeSw __isStopByEmergeSw) 0)
    ((:isStopByFrontEdgeContact __isStopByFrontEdgeContact) 0)
    ((:isStopByBackEdgeContact __isStopByBackEdgeContact) 0)
    ((:isRaining __isRaining) 0)
    ((:isWaterEnough __isWaterEnough) 0)
    ((:isRubbishFull __isRubbishFull) 0)
    ((:isGarbageCapacityAbn __isGarbageCapacityAbn) 0)
    ((:isDcuFault __isDcuFault) 0)
    ((:isUltraFault __isUltraFault) 0)
    ((:isGpsFault __isGpsFault) 0)
    ((:isMoveMotorFault __isMoveMotorFault) 0)
    ((:isTurnMotorFault __isTurnMotorFault) 0)
    ((:isBrakeSysFault __isBrakeSysFault) 0)
    ((:isBrushSysFault __isBrushSysFault) 0)
    ((:isFanSysFault __isFanSysFault) 0)
    ((:isShakeSysFault __isShakeSysFault) 0)
    ((:leftStretchState __leftStretchState) 0)
    ((:rightStretchState __rightStretchState) 0)
    ((:backElevatorState __backElevatorState) 0)
    ((:frontElevatorState __frontElevatorState) 0)
    ((:baffleState __baffleState) 0)
    ((:rollBrushElevatorState __rollBrushElevatorState) 0)
    ((:isFrontBrushOpen __isFrontBrushOpen) 0)
    ((:isBackBrushOpen __isBackBrushOpen) 0)
    ((:isRollBrushOpen __isRollBrushOpen) 0)
    ((:trashBinCoverState __trashBinCoverState) 0)
    ((:trashBagClampState __trashBagClampState) 0)
    ((:trashBagSubterreneState __trashBagSubterreneState) 0)
    ((:trashBagDrawState __trashBagDrawState) 0)
    ((:isTrashBagExist __isTrashBagExist) 0)
    ((:isTrashBagThrownOut __isTrashBagThrownOut) 0)
    ((:ultraDistToTrashBag __ultraDistToTrashBag) 0.0)
    ((:chargingDoorState __chargingDoorState) 0)
    )
   (send-super :init)
   (setq _batteryPowerLeftRate (round __batteryPowerLeftRate))
   (setq _batteryTemperature (round __batteryTemperature))
   (setq _batteryVoltage (float __batteryVoltage))
   (setq _isAutoMode (round __isAutoMode))
   (setq _isRemoteControl (round __isRemoteControl))
   (setq _isStopByEmergeSw (round __isStopByEmergeSw))
   (setq _isStopByFrontEdgeContact (round __isStopByFrontEdgeContact))
   (setq _isStopByBackEdgeContact (round __isStopByBackEdgeContact))
   (setq _isRaining (round __isRaining))
   (setq _isWaterEnough (round __isWaterEnough))
   (setq _isRubbishFull (round __isRubbishFull))
   (setq _isGarbageCapacityAbn (round __isGarbageCapacityAbn))
   (setq _isDcuFault (round __isDcuFault))
   (setq _isUltraFault (round __isUltraFault))
   (setq _isGpsFault (round __isGpsFault))
   (setq _isMoveMotorFault (round __isMoveMotorFault))
   (setq _isTurnMotorFault (round __isTurnMotorFault))
   (setq _isBrakeSysFault (round __isBrakeSysFault))
   (setq _isBrushSysFault (round __isBrushSysFault))
   (setq _isFanSysFault (round __isFanSysFault))
   (setq _isShakeSysFault (round __isShakeSysFault))
   (setq _leftStretchState (round __leftStretchState))
   (setq _rightStretchState (round __rightStretchState))
   (setq _backElevatorState (round __backElevatorState))
   (setq _frontElevatorState (round __frontElevatorState))
   (setq _baffleState (round __baffleState))
   (setq _rollBrushElevatorState (round __rollBrushElevatorState))
   (setq _isFrontBrushOpen (round __isFrontBrushOpen))
   (setq _isBackBrushOpen (round __isBackBrushOpen))
   (setq _isRollBrushOpen (round __isRollBrushOpen))
   (setq _trashBinCoverState (round __trashBinCoverState))
   (setq _trashBagClampState (round __trashBagClampState))
   (setq _trashBagSubterreneState (round __trashBagSubterreneState))
   (setq _trashBagDrawState (round __trashBagDrawState))
   (setq _isTrashBagExist (round __isTrashBagExist))
   (setq _isTrashBagThrownOut (round __isTrashBagThrownOut))
   (setq _ultraDistToTrashBag (float __ultraDistToTrashBag))
   (setq _chargingDoorState (round __chargingDoorState))
   self)
  (:batteryPowerLeftRate
   (&optional __batteryPowerLeftRate)
   (if __batteryPowerLeftRate (setq _batteryPowerLeftRate __batteryPowerLeftRate)) _batteryPowerLeftRate)
  (:batteryTemperature
   (&optional __batteryTemperature)
   (if __batteryTemperature (setq _batteryTemperature __batteryTemperature)) _batteryTemperature)
  (:batteryVoltage
   (&optional __batteryVoltage)
   (if __batteryVoltage (setq _batteryVoltage __batteryVoltage)) _batteryVoltage)
  (:isAutoMode
   (&optional __isAutoMode)
   (if __isAutoMode (setq _isAutoMode __isAutoMode)) _isAutoMode)
  (:isRemoteControl
   (&optional __isRemoteControl)
   (if __isRemoteControl (setq _isRemoteControl __isRemoteControl)) _isRemoteControl)
  (:isStopByEmergeSw
   (&optional __isStopByEmergeSw)
   (if __isStopByEmergeSw (setq _isStopByEmergeSw __isStopByEmergeSw)) _isStopByEmergeSw)
  (:isStopByFrontEdgeContact
   (&optional __isStopByFrontEdgeContact)
   (if __isStopByFrontEdgeContact (setq _isStopByFrontEdgeContact __isStopByFrontEdgeContact)) _isStopByFrontEdgeContact)
  (:isStopByBackEdgeContact
   (&optional __isStopByBackEdgeContact)
   (if __isStopByBackEdgeContact (setq _isStopByBackEdgeContact __isStopByBackEdgeContact)) _isStopByBackEdgeContact)
  (:isRaining
   (&optional __isRaining)
   (if __isRaining (setq _isRaining __isRaining)) _isRaining)
  (:isWaterEnough
   (&optional __isWaterEnough)
   (if __isWaterEnough (setq _isWaterEnough __isWaterEnough)) _isWaterEnough)
  (:isRubbishFull
   (&optional __isRubbishFull)
   (if __isRubbishFull (setq _isRubbishFull __isRubbishFull)) _isRubbishFull)
  (:isGarbageCapacityAbn
   (&optional __isGarbageCapacityAbn)
   (if __isGarbageCapacityAbn (setq _isGarbageCapacityAbn __isGarbageCapacityAbn)) _isGarbageCapacityAbn)
  (:isDcuFault
   (&optional __isDcuFault)
   (if __isDcuFault (setq _isDcuFault __isDcuFault)) _isDcuFault)
  (:isUltraFault
   (&optional __isUltraFault)
   (if __isUltraFault (setq _isUltraFault __isUltraFault)) _isUltraFault)
  (:isGpsFault
   (&optional __isGpsFault)
   (if __isGpsFault (setq _isGpsFault __isGpsFault)) _isGpsFault)
  (:isMoveMotorFault
   (&optional __isMoveMotorFault)
   (if __isMoveMotorFault (setq _isMoveMotorFault __isMoveMotorFault)) _isMoveMotorFault)
  (:isTurnMotorFault
   (&optional __isTurnMotorFault)
   (if __isTurnMotorFault (setq _isTurnMotorFault __isTurnMotorFault)) _isTurnMotorFault)
  (:isBrakeSysFault
   (&optional __isBrakeSysFault)
   (if __isBrakeSysFault (setq _isBrakeSysFault __isBrakeSysFault)) _isBrakeSysFault)
  (:isBrushSysFault
   (&optional __isBrushSysFault)
   (if __isBrushSysFault (setq _isBrushSysFault __isBrushSysFault)) _isBrushSysFault)
  (:isFanSysFault
   (&optional __isFanSysFault)
   (if __isFanSysFault (setq _isFanSysFault __isFanSysFault)) _isFanSysFault)
  (:isShakeSysFault
   (&optional __isShakeSysFault)
   (if __isShakeSysFault (setq _isShakeSysFault __isShakeSysFault)) _isShakeSysFault)
  (:leftStretchState
   (&optional __leftStretchState)
   (if __leftStretchState (setq _leftStretchState __leftStretchState)) _leftStretchState)
  (:rightStretchState
   (&optional __rightStretchState)
   (if __rightStretchState (setq _rightStretchState __rightStretchState)) _rightStretchState)
  (:backElevatorState
   (&optional __backElevatorState)
   (if __backElevatorState (setq _backElevatorState __backElevatorState)) _backElevatorState)
  (:frontElevatorState
   (&optional __frontElevatorState)
   (if __frontElevatorState (setq _frontElevatorState __frontElevatorState)) _frontElevatorState)
  (:baffleState
   (&optional __baffleState)
   (if __baffleState (setq _baffleState __baffleState)) _baffleState)
  (:rollBrushElevatorState
   (&optional __rollBrushElevatorState)
   (if __rollBrushElevatorState (setq _rollBrushElevatorState __rollBrushElevatorState)) _rollBrushElevatorState)
  (:isFrontBrushOpen
   (&optional __isFrontBrushOpen)
   (if __isFrontBrushOpen (setq _isFrontBrushOpen __isFrontBrushOpen)) _isFrontBrushOpen)
  (:isBackBrushOpen
   (&optional __isBackBrushOpen)
   (if __isBackBrushOpen (setq _isBackBrushOpen __isBackBrushOpen)) _isBackBrushOpen)
  (:isRollBrushOpen
   (&optional __isRollBrushOpen)
   (if __isRollBrushOpen (setq _isRollBrushOpen __isRollBrushOpen)) _isRollBrushOpen)
  (:trashBinCoverState
   (&optional __trashBinCoverState)
   (if __trashBinCoverState (setq _trashBinCoverState __trashBinCoverState)) _trashBinCoverState)
  (:trashBagClampState
   (&optional __trashBagClampState)
   (if __trashBagClampState (setq _trashBagClampState __trashBagClampState)) _trashBagClampState)
  (:trashBagSubterreneState
   (&optional __trashBagSubterreneState)
   (if __trashBagSubterreneState (setq _trashBagSubterreneState __trashBagSubterreneState)) _trashBagSubterreneState)
  (:trashBagDrawState
   (&optional __trashBagDrawState)
   (if __trashBagDrawState (setq _trashBagDrawState __trashBagDrawState)) _trashBagDrawState)
  (:isTrashBagExist
   (&optional __isTrashBagExist)
   (if __isTrashBagExist (setq _isTrashBagExist __isTrashBagExist)) _isTrashBagExist)
  (:isTrashBagThrownOut
   (&optional __isTrashBagThrownOut)
   (if __isTrashBagThrownOut (setq _isTrashBagThrownOut __isTrashBagThrownOut)) _isTrashBagThrownOut)
  (:ultraDistToTrashBag
   (&optional __ultraDistToTrashBag)
   (if __ultraDistToTrashBag (setq _ultraDistToTrashBag __ultraDistToTrashBag)) _ultraDistToTrashBag)
  (:chargingDoorState
   (&optional __chargingDoorState)
   (if __chargingDoorState (setq _chargingDoorState __chargingDoorState)) _chargingDoorState)
  (:serialization-length
   ()
   (+
    ;; uint8 _batteryPowerLeftRate
    1
    ;; char _batteryTemperature
    1
    ;; float32 _batteryVoltage
    4
    ;; uint8 _isAutoMode
    1
    ;; uint8 _isRemoteControl
    1
    ;; uint8 _isStopByEmergeSw
    1
    ;; uint8 _isStopByFrontEdgeContact
    1
    ;; uint8 _isStopByBackEdgeContact
    1
    ;; uint8 _isRaining
    1
    ;; uint8 _isWaterEnough
    1
    ;; uint8 _isRubbishFull
    1
    ;; uint8 _isGarbageCapacityAbn
    1
    ;; uint8 _isDcuFault
    1
    ;; uint8 _isUltraFault
    1
    ;; uint8 _isGpsFault
    1
    ;; uint8 _isMoveMotorFault
    1
    ;; uint8 _isTurnMotorFault
    1
    ;; uint8 _isBrakeSysFault
    1
    ;; uint8 _isBrushSysFault
    1
    ;; uint8 _isFanSysFault
    1
    ;; uint8 _isShakeSysFault
    1
    ;; uint8 _leftStretchState
    1
    ;; uint8 _rightStretchState
    1
    ;; uint8 _backElevatorState
    1
    ;; uint8 _frontElevatorState
    1
    ;; uint8 _baffleState
    1
    ;; uint8 _rollBrushElevatorState
    1
    ;; uint8 _isFrontBrushOpen
    1
    ;; uint8 _isBackBrushOpen
    1
    ;; uint8 _isRollBrushOpen
    1
    ;; uint8 _trashBinCoverState
    1
    ;; uint8 _trashBagClampState
    1
    ;; uint8 _trashBagSubterreneState
    1
    ;; uint8 _trashBagDrawState
    1
    ;; uint8 _isTrashBagExist
    1
    ;; uint8 _isTrashBagThrownOut
    1
    ;; float32 _ultraDistToTrashBag
    4
    ;; uint8 _chargingDoorState
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint8 _batteryPowerLeftRate
       (write-byte _batteryPowerLeftRate s)
     ;; char _batteryTemperature
       (write-byte _batteryTemperature s)
     ;; float32 _batteryVoltage
       (sys::poke _batteryVoltage (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; uint8 _isAutoMode
       (write-byte _isAutoMode s)
     ;; uint8 _isRemoteControl
       (write-byte _isRemoteControl s)
     ;; uint8 _isStopByEmergeSw
       (write-byte _isStopByEmergeSw s)
     ;; uint8 _isStopByFrontEdgeContact
       (write-byte _isStopByFrontEdgeContact s)
     ;; uint8 _isStopByBackEdgeContact
       (write-byte _isStopByBackEdgeContact s)
     ;; uint8 _isRaining
       (write-byte _isRaining s)
     ;; uint8 _isWaterEnough
       (write-byte _isWaterEnough s)
     ;; uint8 _isRubbishFull
       (write-byte _isRubbishFull s)
     ;; uint8 _isGarbageCapacityAbn
       (write-byte _isGarbageCapacityAbn s)
     ;; uint8 _isDcuFault
       (write-byte _isDcuFault s)
     ;; uint8 _isUltraFault
       (write-byte _isUltraFault s)
     ;; uint8 _isGpsFault
       (write-byte _isGpsFault s)
     ;; uint8 _isMoveMotorFault
       (write-byte _isMoveMotorFault s)
     ;; uint8 _isTurnMotorFault
       (write-byte _isTurnMotorFault s)
     ;; uint8 _isBrakeSysFault
       (write-byte _isBrakeSysFault s)
     ;; uint8 _isBrushSysFault
       (write-byte _isBrushSysFault s)
     ;; uint8 _isFanSysFault
       (write-byte _isFanSysFault s)
     ;; uint8 _isShakeSysFault
       (write-byte _isShakeSysFault s)
     ;; uint8 _leftStretchState
       (write-byte _leftStretchState s)
     ;; uint8 _rightStretchState
       (write-byte _rightStretchState s)
     ;; uint8 _backElevatorState
       (write-byte _backElevatorState s)
     ;; uint8 _frontElevatorState
       (write-byte _frontElevatorState s)
     ;; uint8 _baffleState
       (write-byte _baffleState s)
     ;; uint8 _rollBrushElevatorState
       (write-byte _rollBrushElevatorState s)
     ;; uint8 _isFrontBrushOpen
       (write-byte _isFrontBrushOpen s)
     ;; uint8 _isBackBrushOpen
       (write-byte _isBackBrushOpen s)
     ;; uint8 _isRollBrushOpen
       (write-byte _isRollBrushOpen s)
     ;; uint8 _trashBinCoverState
       (write-byte _trashBinCoverState s)
     ;; uint8 _trashBagClampState
       (write-byte _trashBagClampState s)
     ;; uint8 _trashBagSubterreneState
       (write-byte _trashBagSubterreneState s)
     ;; uint8 _trashBagDrawState
       (write-byte _trashBagDrawState s)
     ;; uint8 _isTrashBagExist
       (write-byte _isTrashBagExist s)
     ;; uint8 _isTrashBagThrownOut
       (write-byte _isTrashBagThrownOut s)
     ;; float32 _ultraDistToTrashBag
       (sys::poke _ultraDistToTrashBag (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; uint8 _chargingDoorState
       (write-byte _chargingDoorState s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint8 _batteryPowerLeftRate
     (setq _batteryPowerLeftRate (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; char _batteryTemperature
     (setq _batteryTemperature (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; float32 _batteryVoltage
     (setq _batteryVoltage (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; uint8 _isAutoMode
     (setq _isAutoMode (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _isRemoteControl
     (setq _isRemoteControl (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _isStopByEmergeSw
     (setq _isStopByEmergeSw (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _isStopByFrontEdgeContact
     (setq _isStopByFrontEdgeContact (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _isStopByBackEdgeContact
     (setq _isStopByBackEdgeContact (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _isRaining
     (setq _isRaining (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _isWaterEnough
     (setq _isWaterEnough (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _isRubbishFull
     (setq _isRubbishFull (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _isGarbageCapacityAbn
     (setq _isGarbageCapacityAbn (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _isDcuFault
     (setq _isDcuFault (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _isUltraFault
     (setq _isUltraFault (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _isGpsFault
     (setq _isGpsFault (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _isMoveMotorFault
     (setq _isMoveMotorFault (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _isTurnMotorFault
     (setq _isTurnMotorFault (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _isBrakeSysFault
     (setq _isBrakeSysFault (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _isBrushSysFault
     (setq _isBrushSysFault (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _isFanSysFault
     (setq _isFanSysFault (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _isShakeSysFault
     (setq _isShakeSysFault (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _leftStretchState
     (setq _leftStretchState (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _rightStretchState
     (setq _rightStretchState (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _backElevatorState
     (setq _backElevatorState (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _frontElevatorState
     (setq _frontElevatorState (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _baffleState
     (setq _baffleState (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _rollBrushElevatorState
     (setq _rollBrushElevatorState (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _isFrontBrushOpen
     (setq _isFrontBrushOpen (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _isBackBrushOpen
     (setq _isBackBrushOpen (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _isRollBrushOpen
     (setq _isRollBrushOpen (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _trashBinCoverState
     (setq _trashBinCoverState (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _trashBagClampState
     (setq _trashBagClampState (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _trashBagSubterreneState
     (setq _trashBagSubterreneState (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _trashBagDrawState
     (setq _trashBagDrawState (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _isTrashBagExist
     (setq _isTrashBagExist (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _isTrashBagThrownOut
     (setq _isTrashBagThrownOut (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; float32 _ultraDistToTrashBag
     (setq _ultraDistToTrashBag (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; uint8 _chargingDoorState
     (setq _chargingDoorState (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;;
   self)
  )

(setf (get comm_msg::dcu_info :md5sum-) "8ec425a682ee94ceaf30e5a49484bd60")
(setf (get comm_msg::dcu_info :datatype-) "comm_msg/dcu_info")
(setf (get comm_msg::dcu_info :definition-)
      "# battery state
uint8   batteryPowerLeftRate
char    batteryTemperature
float32 batteryVoltage

# move state
uint8 isAutoMode
uint8 isRemoteControl
uint8 isStopByEmergeSw
uint8 isStopByFrontEdgeContact
uint8 isStopByBackEdgeContact

# other state
uint8 isRaining
uint8 isWaterEnough
uint8 isRubbishFull
uint8 isGarbageCapacityAbn

# module fault state
uint8 isDcuFault
uint8 isUltraFault
uint8 isGpsFault
uint8 isMoveMotorFault
uint8 isTurnMotorFault
uint8 isBrakeSysFault
uint8 isBrushSysFault
uint8 isFanSysFault
uint8 isShakeSysFault

# stretch state
uint8 leftStretchState
uint8 rightStretchState
uint8 backElevatorState
uint8 frontElevatorState
uint8 baffleState
uint8 rollBrushElevatorState
uint8 isFrontBrushOpen
uint8 isBackBrushOpen
uint8 isRollBrushOpen

# Trash Pack
uint8 trashBinCoverState
uint8 trashBagClampState
uint8 trashBagSubterreneState
uint8 trashBagDrawState
uint8 isTrashBagExist
uint8 isTrashBagThrownOut
float32 ultraDistToTrashBag

# Charging Door State
uint8 chargingDoorState

")



(provide :comm_msg/dcu_info "8ec425a682ee94ceaf30e5a49484bd60")


