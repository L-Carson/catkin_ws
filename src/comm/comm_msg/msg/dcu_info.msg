# battery state
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
