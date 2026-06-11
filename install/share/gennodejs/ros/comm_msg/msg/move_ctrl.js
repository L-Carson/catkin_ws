// Auto-generated. Do not edit!

// (in-package comm_msg.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let std_msgs = _finder('std_msgs');

//-----------------------------------------------------------

class move_ctrl {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.header = null;
      this.isExistTask = null;
      this.isTeleCtrl = null;
      this.isCleaning = null;
      this.steeringAngle = null;
      this.moveLinearVel = null;
      this.isMoveSmooth = null;
      this.isStopSmooth = null;
      this.isUseSecurityRedundancy = null;
      this.isIgnoreLaserData = null;
      this.isIgnoreUltrasonicData = null;
      this.planningModel = null;
      this.brushModel = null;
    }
    else {
      if (initObj.hasOwnProperty('header')) {
        this.header = initObj.header
      }
      else {
        this.header = new std_msgs.msg.Header();
      }
      if (initObj.hasOwnProperty('isExistTask')) {
        this.isExistTask = initObj.isExistTask
      }
      else {
        this.isExistTask = 0;
      }
      if (initObj.hasOwnProperty('isTeleCtrl')) {
        this.isTeleCtrl = initObj.isTeleCtrl
      }
      else {
        this.isTeleCtrl = 0;
      }
      if (initObj.hasOwnProperty('isCleaning')) {
        this.isCleaning = initObj.isCleaning
      }
      else {
        this.isCleaning = 0;
      }
      if (initObj.hasOwnProperty('steeringAngle')) {
        this.steeringAngle = initObj.steeringAngle
      }
      else {
        this.steeringAngle = 0.0;
      }
      if (initObj.hasOwnProperty('moveLinearVel')) {
        this.moveLinearVel = initObj.moveLinearVel
      }
      else {
        this.moveLinearVel = 0.0;
      }
      if (initObj.hasOwnProperty('isMoveSmooth')) {
        this.isMoveSmooth = initObj.isMoveSmooth
      }
      else {
        this.isMoveSmooth = 0;
      }
      if (initObj.hasOwnProperty('isStopSmooth')) {
        this.isStopSmooth = initObj.isStopSmooth
      }
      else {
        this.isStopSmooth = 0;
      }
      if (initObj.hasOwnProperty('isUseSecurityRedundancy')) {
        this.isUseSecurityRedundancy = initObj.isUseSecurityRedundancy
      }
      else {
        this.isUseSecurityRedundancy = 0;
      }
      if (initObj.hasOwnProperty('isIgnoreLaserData')) {
        this.isIgnoreLaserData = initObj.isIgnoreLaserData
      }
      else {
        this.isIgnoreLaserData = 0;
      }
      if (initObj.hasOwnProperty('isIgnoreUltrasonicData')) {
        this.isIgnoreUltrasonicData = initObj.isIgnoreUltrasonicData
      }
      else {
        this.isIgnoreUltrasonicData = 0;
      }
      if (initObj.hasOwnProperty('planningModel')) {
        this.planningModel = initObj.planningModel
      }
      else {
        this.planningModel = 0;
      }
      if (initObj.hasOwnProperty('brushModel')) {
        this.brushModel = initObj.brushModel
      }
      else {
        this.brushModel = 0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type move_ctrl
    // Serialize message field [header]
    bufferOffset = std_msgs.msg.Header.serialize(obj.header, buffer, bufferOffset);
    // Serialize message field [isExistTask]
    bufferOffset = _serializer.uint8(obj.isExistTask, buffer, bufferOffset);
    // Serialize message field [isTeleCtrl]
    bufferOffset = _serializer.uint8(obj.isTeleCtrl, buffer, bufferOffset);
    // Serialize message field [isCleaning]
    bufferOffset = _serializer.uint8(obj.isCleaning, buffer, bufferOffset);
    // Serialize message field [steeringAngle]
    bufferOffset = _serializer.float32(obj.steeringAngle, buffer, bufferOffset);
    // Serialize message field [moveLinearVel]
    bufferOffset = _serializer.float32(obj.moveLinearVel, buffer, bufferOffset);
    // Serialize message field [isMoveSmooth]
    bufferOffset = _serializer.uint8(obj.isMoveSmooth, buffer, bufferOffset);
    // Serialize message field [isStopSmooth]
    bufferOffset = _serializer.uint8(obj.isStopSmooth, buffer, bufferOffset);
    // Serialize message field [isUseSecurityRedundancy]
    bufferOffset = _serializer.uint8(obj.isUseSecurityRedundancy, buffer, bufferOffset);
    // Serialize message field [isIgnoreLaserData]
    bufferOffset = _serializer.uint8(obj.isIgnoreLaserData, buffer, bufferOffset);
    // Serialize message field [isIgnoreUltrasonicData]
    bufferOffset = _serializer.uint8(obj.isIgnoreUltrasonicData, buffer, bufferOffset);
    // Serialize message field [planningModel]
    bufferOffset = _serializer.uint8(obj.planningModel, buffer, bufferOffset);
    // Serialize message field [brushModel]
    bufferOffset = _serializer.uint8(obj.brushModel, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type move_ctrl
    let len;
    let data = new move_ctrl(null);
    // Deserialize message field [header]
    data.header = std_msgs.msg.Header.deserialize(buffer, bufferOffset);
    // Deserialize message field [isExistTask]
    data.isExistTask = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [isTeleCtrl]
    data.isTeleCtrl = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [isCleaning]
    data.isCleaning = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [steeringAngle]
    data.steeringAngle = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [moveLinearVel]
    data.moveLinearVel = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [isMoveSmooth]
    data.isMoveSmooth = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [isStopSmooth]
    data.isStopSmooth = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [isUseSecurityRedundancy]
    data.isUseSecurityRedundancy = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [isIgnoreLaserData]
    data.isIgnoreLaserData = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [isIgnoreUltrasonicData]
    data.isIgnoreUltrasonicData = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [planningModel]
    data.planningModel = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [brushModel]
    data.brushModel = _deserializer.uint8(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += std_msgs.msg.Header.getMessageSize(object.header);
    return length + 18;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/move_ctrl';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '50e3072a553418d33ffdbff20878da88';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    Header  header
    
    uint8   isExistTask
    uint8   isTeleCtrl
    uint8   isCleaning
    float32 steeringAngle
    float32 moveLinearVel
    uint8   isMoveSmooth
    uint8   isStopSmooth
    
    # 发给安全冗余模块新增的消息
    uint8   isUseSecurityRedundancy
    uint8   isIgnoreLaserData
    uint8   isIgnoreUltrasonicData
    uint8   planningModel          # 0无效模式, 1贴边模式, 2跟线模式, 3发点远程遥控模式, 4方向远程遥控模式, 5调试工具遥控车辆
    uint8   brushModel             # 0关刷模式，1开刷模式
    ================================================================================
    MSG: std_msgs/Header
    # Standard metadata for higher-level stamped data types.
    # This is generally used to communicate timestamped data 
    # in a particular coordinate frame.
    # 
    # sequence ID: consecutively increasing ID 
    uint32 seq
    #Two-integer timestamp that is expressed as:
    # * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
    # * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
    # time-handling sugar is provided by the client library
    time stamp
    #Frame this data is associated with
    string frame_id
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new move_ctrl(null);
    if (msg.header !== undefined) {
      resolved.header = std_msgs.msg.Header.Resolve(msg.header)
    }
    else {
      resolved.header = new std_msgs.msg.Header()
    }

    if (msg.isExistTask !== undefined) {
      resolved.isExistTask = msg.isExistTask;
    }
    else {
      resolved.isExistTask = 0
    }

    if (msg.isTeleCtrl !== undefined) {
      resolved.isTeleCtrl = msg.isTeleCtrl;
    }
    else {
      resolved.isTeleCtrl = 0
    }

    if (msg.isCleaning !== undefined) {
      resolved.isCleaning = msg.isCleaning;
    }
    else {
      resolved.isCleaning = 0
    }

    if (msg.steeringAngle !== undefined) {
      resolved.steeringAngle = msg.steeringAngle;
    }
    else {
      resolved.steeringAngle = 0.0
    }

    if (msg.moveLinearVel !== undefined) {
      resolved.moveLinearVel = msg.moveLinearVel;
    }
    else {
      resolved.moveLinearVel = 0.0
    }

    if (msg.isMoveSmooth !== undefined) {
      resolved.isMoveSmooth = msg.isMoveSmooth;
    }
    else {
      resolved.isMoveSmooth = 0
    }

    if (msg.isStopSmooth !== undefined) {
      resolved.isStopSmooth = msg.isStopSmooth;
    }
    else {
      resolved.isStopSmooth = 0
    }

    if (msg.isUseSecurityRedundancy !== undefined) {
      resolved.isUseSecurityRedundancy = msg.isUseSecurityRedundancy;
    }
    else {
      resolved.isUseSecurityRedundancy = 0
    }

    if (msg.isIgnoreLaserData !== undefined) {
      resolved.isIgnoreLaserData = msg.isIgnoreLaserData;
    }
    else {
      resolved.isIgnoreLaserData = 0
    }

    if (msg.isIgnoreUltrasonicData !== undefined) {
      resolved.isIgnoreUltrasonicData = msg.isIgnoreUltrasonicData;
    }
    else {
      resolved.isIgnoreUltrasonicData = 0
    }

    if (msg.planningModel !== undefined) {
      resolved.planningModel = msg.planningModel;
    }
    else {
      resolved.planningModel = 0
    }

    if (msg.brushModel !== undefined) {
      resolved.brushModel = msg.brushModel;
    }
    else {
      resolved.brushModel = 0
    }

    return resolved;
    }
};

module.exports = move_ctrl;
