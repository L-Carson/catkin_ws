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

class dev_info {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.chargerMotorState = null;
      this.chargerMotorTravel = null;
      this.liftMotorState = null;
      this.rollMotorState = null;
      this.leftLiftMotorTravel = null;
      this.rightLiftMotorTravel = null;
      this.leftRollMotorTravel = null;
      this.rightRollMotorTravel = null;
      this.angleSensorOne = null;
      this.angleSensorTwo = null;
      this.isStationEmergeStop = null;
      this.isManualChargeMode = null;
      this.proximitySw = null;
      this.chargeSw = null;
      this.waterSw = null;
      this.stationLockSw = null;
      this.chargerTravelSw = null;
      this.leftTravelSw = null;
      this.rightTravelSw = null;
      this.trashBinCoverOpenAngle = null;
    }
    else {
      if (initObj.hasOwnProperty('chargerMotorState')) {
        this.chargerMotorState = initObj.chargerMotorState
      }
      else {
        this.chargerMotorState = 0;
      }
      if (initObj.hasOwnProperty('chargerMotorTravel')) {
        this.chargerMotorTravel = initObj.chargerMotorTravel
      }
      else {
        this.chargerMotorTravel = 0;
      }
      if (initObj.hasOwnProperty('liftMotorState')) {
        this.liftMotorState = initObj.liftMotorState
      }
      else {
        this.liftMotorState = 0;
      }
      if (initObj.hasOwnProperty('rollMotorState')) {
        this.rollMotorState = initObj.rollMotorState
      }
      else {
        this.rollMotorState = 0;
      }
      if (initObj.hasOwnProperty('leftLiftMotorTravel')) {
        this.leftLiftMotorTravel = initObj.leftLiftMotorTravel
      }
      else {
        this.leftLiftMotorTravel = 0;
      }
      if (initObj.hasOwnProperty('rightLiftMotorTravel')) {
        this.rightLiftMotorTravel = initObj.rightLiftMotorTravel
      }
      else {
        this.rightLiftMotorTravel = 0;
      }
      if (initObj.hasOwnProperty('leftRollMotorTravel')) {
        this.leftRollMotorTravel = initObj.leftRollMotorTravel
      }
      else {
        this.leftRollMotorTravel = 0;
      }
      if (initObj.hasOwnProperty('rightRollMotorTravel')) {
        this.rightRollMotorTravel = initObj.rightRollMotorTravel
      }
      else {
        this.rightRollMotorTravel = 0;
      }
      if (initObj.hasOwnProperty('angleSensorOne')) {
        this.angleSensorOne = initObj.angleSensorOne
      }
      else {
        this.angleSensorOne = 0.0;
      }
      if (initObj.hasOwnProperty('angleSensorTwo')) {
        this.angleSensorTwo = initObj.angleSensorTwo
      }
      else {
        this.angleSensorTwo = 0.0;
      }
      if (initObj.hasOwnProperty('isStationEmergeStop')) {
        this.isStationEmergeStop = initObj.isStationEmergeStop
      }
      else {
        this.isStationEmergeStop = 0;
      }
      if (initObj.hasOwnProperty('isManualChargeMode')) {
        this.isManualChargeMode = initObj.isManualChargeMode
      }
      else {
        this.isManualChargeMode = 0;
      }
      if (initObj.hasOwnProperty('proximitySw')) {
        this.proximitySw = initObj.proximitySw
      }
      else {
        this.proximitySw = 0;
      }
      if (initObj.hasOwnProperty('chargeSw')) {
        this.chargeSw = initObj.chargeSw
      }
      else {
        this.chargeSw = 0;
      }
      if (initObj.hasOwnProperty('waterSw')) {
        this.waterSw = initObj.waterSw
      }
      else {
        this.waterSw = 0;
      }
      if (initObj.hasOwnProperty('stationLockSw')) {
        this.stationLockSw = initObj.stationLockSw
      }
      else {
        this.stationLockSw = 0;
      }
      if (initObj.hasOwnProperty('chargerTravelSw')) {
        this.chargerTravelSw = initObj.chargerTravelSw
      }
      else {
        this.chargerTravelSw = 0;
      }
      if (initObj.hasOwnProperty('leftTravelSw')) {
        this.leftTravelSw = initObj.leftTravelSw
      }
      else {
        this.leftTravelSw = 0;
      }
      if (initObj.hasOwnProperty('rightTravelSw')) {
        this.rightTravelSw = initObj.rightTravelSw
      }
      else {
        this.rightTravelSw = 0;
      }
      if (initObj.hasOwnProperty('trashBinCoverOpenAngle')) {
        this.trashBinCoverOpenAngle = initObj.trashBinCoverOpenAngle
      }
      else {
        this.trashBinCoverOpenAngle = 0.0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type dev_info
    // Serialize message field [chargerMotorState]
    bufferOffset = _serializer.uint8(obj.chargerMotorState, buffer, bufferOffset);
    // Serialize message field [chargerMotorTravel]
    bufferOffset = _serializer.uint32(obj.chargerMotorTravel, buffer, bufferOffset);
    // Serialize message field [liftMotorState]
    bufferOffset = _serializer.uint8(obj.liftMotorState, buffer, bufferOffset);
    // Serialize message field [rollMotorState]
    bufferOffset = _serializer.uint8(obj.rollMotorState, buffer, bufferOffset);
    // Serialize message field [leftLiftMotorTravel]
    bufferOffset = _serializer.uint32(obj.leftLiftMotorTravel, buffer, bufferOffset);
    // Serialize message field [rightLiftMotorTravel]
    bufferOffset = _serializer.uint32(obj.rightLiftMotorTravel, buffer, bufferOffset);
    // Serialize message field [leftRollMotorTravel]
    bufferOffset = _serializer.uint32(obj.leftRollMotorTravel, buffer, bufferOffset);
    // Serialize message field [rightRollMotorTravel]
    bufferOffset = _serializer.uint32(obj.rightRollMotorTravel, buffer, bufferOffset);
    // Serialize message field [angleSensorOne]
    bufferOffset = _serializer.float32(obj.angleSensorOne, buffer, bufferOffset);
    // Serialize message field [angleSensorTwo]
    bufferOffset = _serializer.float32(obj.angleSensorTwo, buffer, bufferOffset);
    // Serialize message field [isStationEmergeStop]
    bufferOffset = _serializer.uint8(obj.isStationEmergeStop, buffer, bufferOffset);
    // Serialize message field [isManualChargeMode]
    bufferOffset = _serializer.uint8(obj.isManualChargeMode, buffer, bufferOffset);
    // Serialize message field [proximitySw]
    bufferOffset = _serializer.uint8(obj.proximitySw, buffer, bufferOffset);
    // Serialize message field [chargeSw]
    bufferOffset = _serializer.uint8(obj.chargeSw, buffer, bufferOffset);
    // Serialize message field [waterSw]
    bufferOffset = _serializer.uint8(obj.waterSw, buffer, bufferOffset);
    // Serialize message field [stationLockSw]
    bufferOffset = _serializer.uint8(obj.stationLockSw, buffer, bufferOffset);
    // Serialize message field [chargerTravelSw]
    bufferOffset = _serializer.uint8(obj.chargerTravelSw, buffer, bufferOffset);
    // Serialize message field [leftTravelSw]
    bufferOffset = _serializer.uint8(obj.leftTravelSw, buffer, bufferOffset);
    // Serialize message field [rightTravelSw]
    bufferOffset = _serializer.uint8(obj.rightTravelSw, buffer, bufferOffset);
    // Serialize message field [trashBinCoverOpenAngle]
    bufferOffset = _serializer.float32(obj.trashBinCoverOpenAngle, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type dev_info
    let len;
    let data = new dev_info(null);
    // Deserialize message field [chargerMotorState]
    data.chargerMotorState = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [chargerMotorTravel]
    data.chargerMotorTravel = _deserializer.uint32(buffer, bufferOffset);
    // Deserialize message field [liftMotorState]
    data.liftMotorState = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [rollMotorState]
    data.rollMotorState = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [leftLiftMotorTravel]
    data.leftLiftMotorTravel = _deserializer.uint32(buffer, bufferOffset);
    // Deserialize message field [rightLiftMotorTravel]
    data.rightLiftMotorTravel = _deserializer.uint32(buffer, bufferOffset);
    // Deserialize message field [leftRollMotorTravel]
    data.leftRollMotorTravel = _deserializer.uint32(buffer, bufferOffset);
    // Deserialize message field [rightRollMotorTravel]
    data.rightRollMotorTravel = _deserializer.uint32(buffer, bufferOffset);
    // Deserialize message field [angleSensorOne]
    data.angleSensorOne = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [angleSensorTwo]
    data.angleSensorTwo = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [isStationEmergeStop]
    data.isStationEmergeStop = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [isManualChargeMode]
    data.isManualChargeMode = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [proximitySw]
    data.proximitySw = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [chargeSw]
    data.chargeSw = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [waterSw]
    data.waterSw = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [stationLockSw]
    data.stationLockSw = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [chargerTravelSw]
    data.chargerTravelSw = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [leftTravelSw]
    data.leftTravelSw = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [rightTravelSw]
    data.rightTravelSw = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [trashBinCoverOpenAngle]
    data.trashBinCoverOpenAngle = _deserializer.float32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 44;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/dev_info';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '17ff66948ac49c3e03faf3f4e7f62471';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
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
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new dev_info(null);
    if (msg.chargerMotorState !== undefined) {
      resolved.chargerMotorState = msg.chargerMotorState;
    }
    else {
      resolved.chargerMotorState = 0
    }

    if (msg.chargerMotorTravel !== undefined) {
      resolved.chargerMotorTravel = msg.chargerMotorTravel;
    }
    else {
      resolved.chargerMotorTravel = 0
    }

    if (msg.liftMotorState !== undefined) {
      resolved.liftMotorState = msg.liftMotorState;
    }
    else {
      resolved.liftMotorState = 0
    }

    if (msg.rollMotorState !== undefined) {
      resolved.rollMotorState = msg.rollMotorState;
    }
    else {
      resolved.rollMotorState = 0
    }

    if (msg.leftLiftMotorTravel !== undefined) {
      resolved.leftLiftMotorTravel = msg.leftLiftMotorTravel;
    }
    else {
      resolved.leftLiftMotorTravel = 0
    }

    if (msg.rightLiftMotorTravel !== undefined) {
      resolved.rightLiftMotorTravel = msg.rightLiftMotorTravel;
    }
    else {
      resolved.rightLiftMotorTravel = 0
    }

    if (msg.leftRollMotorTravel !== undefined) {
      resolved.leftRollMotorTravel = msg.leftRollMotorTravel;
    }
    else {
      resolved.leftRollMotorTravel = 0
    }

    if (msg.rightRollMotorTravel !== undefined) {
      resolved.rightRollMotorTravel = msg.rightRollMotorTravel;
    }
    else {
      resolved.rightRollMotorTravel = 0
    }

    if (msg.angleSensorOne !== undefined) {
      resolved.angleSensorOne = msg.angleSensorOne;
    }
    else {
      resolved.angleSensorOne = 0.0
    }

    if (msg.angleSensorTwo !== undefined) {
      resolved.angleSensorTwo = msg.angleSensorTwo;
    }
    else {
      resolved.angleSensorTwo = 0.0
    }

    if (msg.isStationEmergeStop !== undefined) {
      resolved.isStationEmergeStop = msg.isStationEmergeStop;
    }
    else {
      resolved.isStationEmergeStop = 0
    }

    if (msg.isManualChargeMode !== undefined) {
      resolved.isManualChargeMode = msg.isManualChargeMode;
    }
    else {
      resolved.isManualChargeMode = 0
    }

    if (msg.proximitySw !== undefined) {
      resolved.proximitySw = msg.proximitySw;
    }
    else {
      resolved.proximitySw = 0
    }

    if (msg.chargeSw !== undefined) {
      resolved.chargeSw = msg.chargeSw;
    }
    else {
      resolved.chargeSw = 0
    }

    if (msg.waterSw !== undefined) {
      resolved.waterSw = msg.waterSw;
    }
    else {
      resolved.waterSw = 0
    }

    if (msg.stationLockSw !== undefined) {
      resolved.stationLockSw = msg.stationLockSw;
    }
    else {
      resolved.stationLockSw = 0
    }

    if (msg.chargerTravelSw !== undefined) {
      resolved.chargerTravelSw = msg.chargerTravelSw;
    }
    else {
      resolved.chargerTravelSw = 0
    }

    if (msg.leftTravelSw !== undefined) {
      resolved.leftTravelSw = msg.leftTravelSw;
    }
    else {
      resolved.leftTravelSw = 0
    }

    if (msg.rightTravelSw !== undefined) {
      resolved.rightTravelSw = msg.rightTravelSw;
    }
    else {
      resolved.rightTravelSw = 0
    }

    if (msg.trashBinCoverOpenAngle !== undefined) {
      resolved.trashBinCoverOpenAngle = msg.trashBinCoverOpenAngle;
    }
    else {
      resolved.trashBinCoverOpenAngle = 0.0
    }

    return resolved;
    }
};

module.exports = dev_info;
