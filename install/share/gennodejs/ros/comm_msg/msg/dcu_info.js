// Auto-generated. Do not edit!

// (in-package comm_msg.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class dcu_info {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.batteryPowerLeftRate = null;
      this.batteryTemperature = null;
      this.batteryVoltage = null;
      this.isAutoMode = null;
      this.isRemoteControl = null;
      this.isStopByEmergeSw = null;
      this.isStopByFrontEdgeContact = null;
      this.isStopByBackEdgeContact = null;
      this.isRaining = null;
      this.isWaterEnough = null;
      this.isRubbishFull = null;
      this.isGarbageCapacityAbn = null;
      this.isDcuFault = null;
      this.isUltraFault = null;
      this.isGpsFault = null;
      this.isMoveMotorFault = null;
      this.isTurnMotorFault = null;
      this.isBrakeSysFault = null;
      this.isBrushSysFault = null;
      this.isFanSysFault = null;
      this.isShakeSysFault = null;
      this.leftStretchState = null;
      this.rightStretchState = null;
      this.backElevatorState = null;
      this.frontElevatorState = null;
      this.baffleState = null;
      this.rollBrushElevatorState = null;
      this.isFrontBrushOpen = null;
      this.isBackBrushOpen = null;
      this.isRollBrushOpen = null;
      this.trashBinCoverState = null;
      this.trashBagClampState = null;
      this.trashBagSubterreneState = null;
      this.trashBagDrawState = null;
      this.isTrashBagExist = null;
      this.isTrashBagThrownOut = null;
      this.ultraDistToTrashBag = null;
      this.chargingDoorState = null;
    }
    else {
      if (initObj.hasOwnProperty('batteryPowerLeftRate')) {
        this.batteryPowerLeftRate = initObj.batteryPowerLeftRate
      }
      else {
        this.batteryPowerLeftRate = 0;
      }
      if (initObj.hasOwnProperty('batteryTemperature')) {
        this.batteryTemperature = initObj.batteryTemperature
      }
      else {
        this.batteryTemperature = 0;
      }
      if (initObj.hasOwnProperty('batteryVoltage')) {
        this.batteryVoltage = initObj.batteryVoltage
      }
      else {
        this.batteryVoltage = 0.0;
      }
      if (initObj.hasOwnProperty('isAutoMode')) {
        this.isAutoMode = initObj.isAutoMode
      }
      else {
        this.isAutoMode = 0;
      }
      if (initObj.hasOwnProperty('isRemoteControl')) {
        this.isRemoteControl = initObj.isRemoteControl
      }
      else {
        this.isRemoteControl = 0;
      }
      if (initObj.hasOwnProperty('isStopByEmergeSw')) {
        this.isStopByEmergeSw = initObj.isStopByEmergeSw
      }
      else {
        this.isStopByEmergeSw = 0;
      }
      if (initObj.hasOwnProperty('isStopByFrontEdgeContact')) {
        this.isStopByFrontEdgeContact = initObj.isStopByFrontEdgeContact
      }
      else {
        this.isStopByFrontEdgeContact = 0;
      }
      if (initObj.hasOwnProperty('isStopByBackEdgeContact')) {
        this.isStopByBackEdgeContact = initObj.isStopByBackEdgeContact
      }
      else {
        this.isStopByBackEdgeContact = 0;
      }
      if (initObj.hasOwnProperty('isRaining')) {
        this.isRaining = initObj.isRaining
      }
      else {
        this.isRaining = 0;
      }
      if (initObj.hasOwnProperty('isWaterEnough')) {
        this.isWaterEnough = initObj.isWaterEnough
      }
      else {
        this.isWaterEnough = 0;
      }
      if (initObj.hasOwnProperty('isRubbishFull')) {
        this.isRubbishFull = initObj.isRubbishFull
      }
      else {
        this.isRubbishFull = 0;
      }
      if (initObj.hasOwnProperty('isGarbageCapacityAbn')) {
        this.isGarbageCapacityAbn = initObj.isGarbageCapacityAbn
      }
      else {
        this.isGarbageCapacityAbn = 0;
      }
      if (initObj.hasOwnProperty('isDcuFault')) {
        this.isDcuFault = initObj.isDcuFault
      }
      else {
        this.isDcuFault = 0;
      }
      if (initObj.hasOwnProperty('isUltraFault')) {
        this.isUltraFault = initObj.isUltraFault
      }
      else {
        this.isUltraFault = 0;
      }
      if (initObj.hasOwnProperty('isGpsFault')) {
        this.isGpsFault = initObj.isGpsFault
      }
      else {
        this.isGpsFault = 0;
      }
      if (initObj.hasOwnProperty('isMoveMotorFault')) {
        this.isMoveMotorFault = initObj.isMoveMotorFault
      }
      else {
        this.isMoveMotorFault = 0;
      }
      if (initObj.hasOwnProperty('isTurnMotorFault')) {
        this.isTurnMotorFault = initObj.isTurnMotorFault
      }
      else {
        this.isTurnMotorFault = 0;
      }
      if (initObj.hasOwnProperty('isBrakeSysFault')) {
        this.isBrakeSysFault = initObj.isBrakeSysFault
      }
      else {
        this.isBrakeSysFault = 0;
      }
      if (initObj.hasOwnProperty('isBrushSysFault')) {
        this.isBrushSysFault = initObj.isBrushSysFault
      }
      else {
        this.isBrushSysFault = 0;
      }
      if (initObj.hasOwnProperty('isFanSysFault')) {
        this.isFanSysFault = initObj.isFanSysFault
      }
      else {
        this.isFanSysFault = 0;
      }
      if (initObj.hasOwnProperty('isShakeSysFault')) {
        this.isShakeSysFault = initObj.isShakeSysFault
      }
      else {
        this.isShakeSysFault = 0;
      }
      if (initObj.hasOwnProperty('leftStretchState')) {
        this.leftStretchState = initObj.leftStretchState
      }
      else {
        this.leftStretchState = 0;
      }
      if (initObj.hasOwnProperty('rightStretchState')) {
        this.rightStretchState = initObj.rightStretchState
      }
      else {
        this.rightStretchState = 0;
      }
      if (initObj.hasOwnProperty('backElevatorState')) {
        this.backElevatorState = initObj.backElevatorState
      }
      else {
        this.backElevatorState = 0;
      }
      if (initObj.hasOwnProperty('frontElevatorState')) {
        this.frontElevatorState = initObj.frontElevatorState
      }
      else {
        this.frontElevatorState = 0;
      }
      if (initObj.hasOwnProperty('baffleState')) {
        this.baffleState = initObj.baffleState
      }
      else {
        this.baffleState = 0;
      }
      if (initObj.hasOwnProperty('rollBrushElevatorState')) {
        this.rollBrushElevatorState = initObj.rollBrushElevatorState
      }
      else {
        this.rollBrushElevatorState = 0;
      }
      if (initObj.hasOwnProperty('isFrontBrushOpen')) {
        this.isFrontBrushOpen = initObj.isFrontBrushOpen
      }
      else {
        this.isFrontBrushOpen = 0;
      }
      if (initObj.hasOwnProperty('isBackBrushOpen')) {
        this.isBackBrushOpen = initObj.isBackBrushOpen
      }
      else {
        this.isBackBrushOpen = 0;
      }
      if (initObj.hasOwnProperty('isRollBrushOpen')) {
        this.isRollBrushOpen = initObj.isRollBrushOpen
      }
      else {
        this.isRollBrushOpen = 0;
      }
      if (initObj.hasOwnProperty('trashBinCoverState')) {
        this.trashBinCoverState = initObj.trashBinCoverState
      }
      else {
        this.trashBinCoverState = 0;
      }
      if (initObj.hasOwnProperty('trashBagClampState')) {
        this.trashBagClampState = initObj.trashBagClampState
      }
      else {
        this.trashBagClampState = 0;
      }
      if (initObj.hasOwnProperty('trashBagSubterreneState')) {
        this.trashBagSubterreneState = initObj.trashBagSubterreneState
      }
      else {
        this.trashBagSubterreneState = 0;
      }
      if (initObj.hasOwnProperty('trashBagDrawState')) {
        this.trashBagDrawState = initObj.trashBagDrawState
      }
      else {
        this.trashBagDrawState = 0;
      }
      if (initObj.hasOwnProperty('isTrashBagExist')) {
        this.isTrashBagExist = initObj.isTrashBagExist
      }
      else {
        this.isTrashBagExist = 0;
      }
      if (initObj.hasOwnProperty('isTrashBagThrownOut')) {
        this.isTrashBagThrownOut = initObj.isTrashBagThrownOut
      }
      else {
        this.isTrashBagThrownOut = 0;
      }
      if (initObj.hasOwnProperty('ultraDistToTrashBag')) {
        this.ultraDistToTrashBag = initObj.ultraDistToTrashBag
      }
      else {
        this.ultraDistToTrashBag = 0.0;
      }
      if (initObj.hasOwnProperty('chargingDoorState')) {
        this.chargingDoorState = initObj.chargingDoorState
      }
      else {
        this.chargingDoorState = 0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type dcu_info
    // Serialize message field [batteryPowerLeftRate]
    bufferOffset = _serializer.uint8(obj.batteryPowerLeftRate, buffer, bufferOffset);
    // Serialize message field [batteryTemperature]
    bufferOffset = _serializer.char(obj.batteryTemperature, buffer, bufferOffset);
    // Serialize message field [batteryVoltage]
    bufferOffset = _serializer.float32(obj.batteryVoltage, buffer, bufferOffset);
    // Serialize message field [isAutoMode]
    bufferOffset = _serializer.uint8(obj.isAutoMode, buffer, bufferOffset);
    // Serialize message field [isRemoteControl]
    bufferOffset = _serializer.uint8(obj.isRemoteControl, buffer, bufferOffset);
    // Serialize message field [isStopByEmergeSw]
    bufferOffset = _serializer.uint8(obj.isStopByEmergeSw, buffer, bufferOffset);
    // Serialize message field [isStopByFrontEdgeContact]
    bufferOffset = _serializer.uint8(obj.isStopByFrontEdgeContact, buffer, bufferOffset);
    // Serialize message field [isStopByBackEdgeContact]
    bufferOffset = _serializer.uint8(obj.isStopByBackEdgeContact, buffer, bufferOffset);
    // Serialize message field [isRaining]
    bufferOffset = _serializer.uint8(obj.isRaining, buffer, bufferOffset);
    // Serialize message field [isWaterEnough]
    bufferOffset = _serializer.uint8(obj.isWaterEnough, buffer, bufferOffset);
    // Serialize message field [isRubbishFull]
    bufferOffset = _serializer.uint8(obj.isRubbishFull, buffer, bufferOffset);
    // Serialize message field [isGarbageCapacityAbn]
    bufferOffset = _serializer.uint8(obj.isGarbageCapacityAbn, buffer, bufferOffset);
    // Serialize message field [isDcuFault]
    bufferOffset = _serializer.uint8(obj.isDcuFault, buffer, bufferOffset);
    // Serialize message field [isUltraFault]
    bufferOffset = _serializer.uint8(obj.isUltraFault, buffer, bufferOffset);
    // Serialize message field [isGpsFault]
    bufferOffset = _serializer.uint8(obj.isGpsFault, buffer, bufferOffset);
    // Serialize message field [isMoveMotorFault]
    bufferOffset = _serializer.uint8(obj.isMoveMotorFault, buffer, bufferOffset);
    // Serialize message field [isTurnMotorFault]
    bufferOffset = _serializer.uint8(obj.isTurnMotorFault, buffer, bufferOffset);
    // Serialize message field [isBrakeSysFault]
    bufferOffset = _serializer.uint8(obj.isBrakeSysFault, buffer, bufferOffset);
    // Serialize message field [isBrushSysFault]
    bufferOffset = _serializer.uint8(obj.isBrushSysFault, buffer, bufferOffset);
    // Serialize message field [isFanSysFault]
    bufferOffset = _serializer.uint8(obj.isFanSysFault, buffer, bufferOffset);
    // Serialize message field [isShakeSysFault]
    bufferOffset = _serializer.uint8(obj.isShakeSysFault, buffer, bufferOffset);
    // Serialize message field [leftStretchState]
    bufferOffset = _serializer.uint8(obj.leftStretchState, buffer, bufferOffset);
    // Serialize message field [rightStretchState]
    bufferOffset = _serializer.uint8(obj.rightStretchState, buffer, bufferOffset);
    // Serialize message field [backElevatorState]
    bufferOffset = _serializer.uint8(obj.backElevatorState, buffer, bufferOffset);
    // Serialize message field [frontElevatorState]
    bufferOffset = _serializer.uint8(obj.frontElevatorState, buffer, bufferOffset);
    // Serialize message field [baffleState]
    bufferOffset = _serializer.uint8(obj.baffleState, buffer, bufferOffset);
    // Serialize message field [rollBrushElevatorState]
    bufferOffset = _serializer.uint8(obj.rollBrushElevatorState, buffer, bufferOffset);
    // Serialize message field [isFrontBrushOpen]
    bufferOffset = _serializer.uint8(obj.isFrontBrushOpen, buffer, bufferOffset);
    // Serialize message field [isBackBrushOpen]
    bufferOffset = _serializer.uint8(obj.isBackBrushOpen, buffer, bufferOffset);
    // Serialize message field [isRollBrushOpen]
    bufferOffset = _serializer.uint8(obj.isRollBrushOpen, buffer, bufferOffset);
    // Serialize message field [trashBinCoverState]
    bufferOffset = _serializer.uint8(obj.trashBinCoverState, buffer, bufferOffset);
    // Serialize message field [trashBagClampState]
    bufferOffset = _serializer.uint8(obj.trashBagClampState, buffer, bufferOffset);
    // Serialize message field [trashBagSubterreneState]
    bufferOffset = _serializer.uint8(obj.trashBagSubterreneState, buffer, bufferOffset);
    // Serialize message field [trashBagDrawState]
    bufferOffset = _serializer.uint8(obj.trashBagDrawState, buffer, bufferOffset);
    // Serialize message field [isTrashBagExist]
    bufferOffset = _serializer.uint8(obj.isTrashBagExist, buffer, bufferOffset);
    // Serialize message field [isTrashBagThrownOut]
    bufferOffset = _serializer.uint8(obj.isTrashBagThrownOut, buffer, bufferOffset);
    // Serialize message field [ultraDistToTrashBag]
    bufferOffset = _serializer.float32(obj.ultraDistToTrashBag, buffer, bufferOffset);
    // Serialize message field [chargingDoorState]
    bufferOffset = _serializer.uint8(obj.chargingDoorState, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type dcu_info
    let len;
    let data = new dcu_info(null);
    // Deserialize message field [batteryPowerLeftRate]
    data.batteryPowerLeftRate = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [batteryTemperature]
    data.batteryTemperature = _deserializer.char(buffer, bufferOffset);
    // Deserialize message field [batteryVoltage]
    data.batteryVoltage = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [isAutoMode]
    data.isAutoMode = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [isRemoteControl]
    data.isRemoteControl = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [isStopByEmergeSw]
    data.isStopByEmergeSw = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [isStopByFrontEdgeContact]
    data.isStopByFrontEdgeContact = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [isStopByBackEdgeContact]
    data.isStopByBackEdgeContact = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [isRaining]
    data.isRaining = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [isWaterEnough]
    data.isWaterEnough = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [isRubbishFull]
    data.isRubbishFull = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [isGarbageCapacityAbn]
    data.isGarbageCapacityAbn = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [isDcuFault]
    data.isDcuFault = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [isUltraFault]
    data.isUltraFault = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [isGpsFault]
    data.isGpsFault = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [isMoveMotorFault]
    data.isMoveMotorFault = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [isTurnMotorFault]
    data.isTurnMotorFault = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [isBrakeSysFault]
    data.isBrakeSysFault = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [isBrushSysFault]
    data.isBrushSysFault = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [isFanSysFault]
    data.isFanSysFault = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [isShakeSysFault]
    data.isShakeSysFault = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [leftStretchState]
    data.leftStretchState = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [rightStretchState]
    data.rightStretchState = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [backElevatorState]
    data.backElevatorState = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [frontElevatorState]
    data.frontElevatorState = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [baffleState]
    data.baffleState = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [rollBrushElevatorState]
    data.rollBrushElevatorState = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [isFrontBrushOpen]
    data.isFrontBrushOpen = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [isBackBrushOpen]
    data.isBackBrushOpen = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [isRollBrushOpen]
    data.isRollBrushOpen = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [trashBinCoverState]
    data.trashBinCoverState = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [trashBagClampState]
    data.trashBagClampState = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [trashBagSubterreneState]
    data.trashBagSubterreneState = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [trashBagDrawState]
    data.trashBagDrawState = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [isTrashBagExist]
    data.isTrashBagExist = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [isTrashBagThrownOut]
    data.isTrashBagThrownOut = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [ultraDistToTrashBag]
    data.ultraDistToTrashBag = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [chargingDoorState]
    data.chargingDoorState = _deserializer.uint8(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 44;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/dcu_info';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '8ec425a682ee94ceaf30e5a49484bd60';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
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
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new dcu_info(null);
    if (msg.batteryPowerLeftRate !== undefined) {
      resolved.batteryPowerLeftRate = msg.batteryPowerLeftRate;
    }
    else {
      resolved.batteryPowerLeftRate = 0
    }

    if (msg.batteryTemperature !== undefined) {
      resolved.batteryTemperature = msg.batteryTemperature;
    }
    else {
      resolved.batteryTemperature = 0
    }

    if (msg.batteryVoltage !== undefined) {
      resolved.batteryVoltage = msg.batteryVoltage;
    }
    else {
      resolved.batteryVoltage = 0.0
    }

    if (msg.isAutoMode !== undefined) {
      resolved.isAutoMode = msg.isAutoMode;
    }
    else {
      resolved.isAutoMode = 0
    }

    if (msg.isRemoteControl !== undefined) {
      resolved.isRemoteControl = msg.isRemoteControl;
    }
    else {
      resolved.isRemoteControl = 0
    }

    if (msg.isStopByEmergeSw !== undefined) {
      resolved.isStopByEmergeSw = msg.isStopByEmergeSw;
    }
    else {
      resolved.isStopByEmergeSw = 0
    }

    if (msg.isStopByFrontEdgeContact !== undefined) {
      resolved.isStopByFrontEdgeContact = msg.isStopByFrontEdgeContact;
    }
    else {
      resolved.isStopByFrontEdgeContact = 0
    }

    if (msg.isStopByBackEdgeContact !== undefined) {
      resolved.isStopByBackEdgeContact = msg.isStopByBackEdgeContact;
    }
    else {
      resolved.isStopByBackEdgeContact = 0
    }

    if (msg.isRaining !== undefined) {
      resolved.isRaining = msg.isRaining;
    }
    else {
      resolved.isRaining = 0
    }

    if (msg.isWaterEnough !== undefined) {
      resolved.isWaterEnough = msg.isWaterEnough;
    }
    else {
      resolved.isWaterEnough = 0
    }

    if (msg.isRubbishFull !== undefined) {
      resolved.isRubbishFull = msg.isRubbishFull;
    }
    else {
      resolved.isRubbishFull = 0
    }

    if (msg.isGarbageCapacityAbn !== undefined) {
      resolved.isGarbageCapacityAbn = msg.isGarbageCapacityAbn;
    }
    else {
      resolved.isGarbageCapacityAbn = 0
    }

    if (msg.isDcuFault !== undefined) {
      resolved.isDcuFault = msg.isDcuFault;
    }
    else {
      resolved.isDcuFault = 0
    }

    if (msg.isUltraFault !== undefined) {
      resolved.isUltraFault = msg.isUltraFault;
    }
    else {
      resolved.isUltraFault = 0
    }

    if (msg.isGpsFault !== undefined) {
      resolved.isGpsFault = msg.isGpsFault;
    }
    else {
      resolved.isGpsFault = 0
    }

    if (msg.isMoveMotorFault !== undefined) {
      resolved.isMoveMotorFault = msg.isMoveMotorFault;
    }
    else {
      resolved.isMoveMotorFault = 0
    }

    if (msg.isTurnMotorFault !== undefined) {
      resolved.isTurnMotorFault = msg.isTurnMotorFault;
    }
    else {
      resolved.isTurnMotorFault = 0
    }

    if (msg.isBrakeSysFault !== undefined) {
      resolved.isBrakeSysFault = msg.isBrakeSysFault;
    }
    else {
      resolved.isBrakeSysFault = 0
    }

    if (msg.isBrushSysFault !== undefined) {
      resolved.isBrushSysFault = msg.isBrushSysFault;
    }
    else {
      resolved.isBrushSysFault = 0
    }

    if (msg.isFanSysFault !== undefined) {
      resolved.isFanSysFault = msg.isFanSysFault;
    }
    else {
      resolved.isFanSysFault = 0
    }

    if (msg.isShakeSysFault !== undefined) {
      resolved.isShakeSysFault = msg.isShakeSysFault;
    }
    else {
      resolved.isShakeSysFault = 0
    }

    if (msg.leftStretchState !== undefined) {
      resolved.leftStretchState = msg.leftStretchState;
    }
    else {
      resolved.leftStretchState = 0
    }

    if (msg.rightStretchState !== undefined) {
      resolved.rightStretchState = msg.rightStretchState;
    }
    else {
      resolved.rightStretchState = 0
    }

    if (msg.backElevatorState !== undefined) {
      resolved.backElevatorState = msg.backElevatorState;
    }
    else {
      resolved.backElevatorState = 0
    }

    if (msg.frontElevatorState !== undefined) {
      resolved.frontElevatorState = msg.frontElevatorState;
    }
    else {
      resolved.frontElevatorState = 0
    }

    if (msg.baffleState !== undefined) {
      resolved.baffleState = msg.baffleState;
    }
    else {
      resolved.baffleState = 0
    }

    if (msg.rollBrushElevatorState !== undefined) {
      resolved.rollBrushElevatorState = msg.rollBrushElevatorState;
    }
    else {
      resolved.rollBrushElevatorState = 0
    }

    if (msg.isFrontBrushOpen !== undefined) {
      resolved.isFrontBrushOpen = msg.isFrontBrushOpen;
    }
    else {
      resolved.isFrontBrushOpen = 0
    }

    if (msg.isBackBrushOpen !== undefined) {
      resolved.isBackBrushOpen = msg.isBackBrushOpen;
    }
    else {
      resolved.isBackBrushOpen = 0
    }

    if (msg.isRollBrushOpen !== undefined) {
      resolved.isRollBrushOpen = msg.isRollBrushOpen;
    }
    else {
      resolved.isRollBrushOpen = 0
    }

    if (msg.trashBinCoverState !== undefined) {
      resolved.trashBinCoverState = msg.trashBinCoverState;
    }
    else {
      resolved.trashBinCoverState = 0
    }

    if (msg.trashBagClampState !== undefined) {
      resolved.trashBagClampState = msg.trashBagClampState;
    }
    else {
      resolved.trashBagClampState = 0
    }

    if (msg.trashBagSubterreneState !== undefined) {
      resolved.trashBagSubterreneState = msg.trashBagSubterreneState;
    }
    else {
      resolved.trashBagSubterreneState = 0
    }

    if (msg.trashBagDrawState !== undefined) {
      resolved.trashBagDrawState = msg.trashBagDrawState;
    }
    else {
      resolved.trashBagDrawState = 0
    }

    if (msg.isTrashBagExist !== undefined) {
      resolved.isTrashBagExist = msg.isTrashBagExist;
    }
    else {
      resolved.isTrashBagExist = 0
    }

    if (msg.isTrashBagThrownOut !== undefined) {
      resolved.isTrashBagThrownOut = msg.isTrashBagThrownOut;
    }
    else {
      resolved.isTrashBagThrownOut = 0
    }

    if (msg.ultraDistToTrashBag !== undefined) {
      resolved.ultraDistToTrashBag = msg.ultraDistToTrashBag;
    }
    else {
      resolved.ultraDistToTrashBag = 0.0
    }

    if (msg.chargingDoorState !== undefined) {
      resolved.chargingDoorState = msg.chargingDoorState;
    }
    else {
      resolved.chargingDoorState = 0
    }

    return resolved;
    }
};

module.exports = dcu_info;
