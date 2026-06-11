// Auto-generated. Do not edit!

// (in-package comm_srvs.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class visionFuncCfg {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.funcId = null;
      this.ipu = null;
      this.hz = null;
    }
    else {
      if (initObj.hasOwnProperty('funcId')) {
        this.funcId = initObj.funcId
      }
      else {
        this.funcId = 0;
      }
      if (initObj.hasOwnProperty('ipu')) {
        this.ipu = initObj.ipu
      }
      else {
        this.ipu = false;
      }
      if (initObj.hasOwnProperty('hz')) {
        this.hz = initObj.hz
      }
      else {
        this.hz = 0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type visionFuncCfg
    // Serialize message field [funcId]
    bufferOffset = _serializer.uint8(obj.funcId, buffer, bufferOffset);
    // Serialize message field [ipu]
    bufferOffset = _serializer.bool(obj.ipu, buffer, bufferOffset);
    // Serialize message field [hz]
    bufferOffset = _serializer.uint8(obj.hz, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type visionFuncCfg
    let len;
    let data = new visionFuncCfg(null);
    // Deserialize message field [funcId]
    data.funcId = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [ipu]
    data.ipu = _deserializer.bool(buffer, bufferOffset);
    // Deserialize message field [hz]
    data.hz = _deserializer.uint8(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 3;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_srvs/visionFuncCfg';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '788b771f7f7a06c374a8246119139c2d';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    uint8   funcId      # 功能ID
    bool    ipu         # 是否使用图像处理单元
    uint8   hz          # 执行频率
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new visionFuncCfg(null);
    if (msg.funcId !== undefined) {
      resolved.funcId = msg.funcId;
    }
    else {
      resolved.funcId = 0
    }

    if (msg.ipu !== undefined) {
      resolved.ipu = msg.ipu;
    }
    else {
      resolved.ipu = false
    }

    if (msg.hz !== undefined) {
      resolved.hz = msg.hz;
    }
    else {
      resolved.hz = 0
    }

    return resolved;
    }
};

module.exports = visionFuncCfg;
