# charger state
uint8  chargerMotorState
uint32 chargerMotorTravel

# trash box state
uint8   liftMotorState
uint8   rollMotorState
uint32  leftLiftMotorTravel
uint32  rightLiftMotorTravel
uint32  leftRollMotorTravel
uint32  rightRollMotorTravel
float32 angleSensorOne
float32 angleSensorTwo

# dev state
uint8 isStationEmergeStop
uint8 isManualChargeMode
uint8 proximitySw
uint8 chargeSw
uint8 waterSw
uint8 stationLockSw
uint8 chargerTravelSw
uint8 leftTravelSw
uint8 rightTravelSw

# trash cover state
float32 trashBinCoverOpenAngle