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

class module_abnormal_info {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.module = null;
      this.errCode = null;
    }
    else {
      if (initObj.hasOwnProperty('module')) {
        this.module = initObj.module
      }
      else {
        this.module = 0;
      }
      if (initObj.hasOwnProperty('errCode')) {
        this.errCode = initObj.errCode
      }
      else {
        this.errCode = 0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type module_abnormal_info
    // Serialize message field [module]
    bufferOffset = _serializer.uint32(obj.module, buffer, bufferOffset);
    // Serialize message field [errCode]
    bufferOffset = _serializer.int32(obj.errCode, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type module_abnormal_info
    let len;
    let data = new module_abnormal_info(null);
    // Deserialize message field [module]
    data.module = _deserializer.uint32(buffer, bufferOffset);
    // Deserialize message field [errCode]
    data.errCode = _deserializer.int32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 8;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/module_abnormal_info';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '84ac86691c35a13d550d35161d31a61a';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    uint32 module
    int32  errCode          # 0 表示正常, 其它都表示异常，后续扩展
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new module_abnormal_info(null);
    if (msg.module !== undefined) {
      resolved.module = msg.module;
    }
    else {
      resolved.module = 0
    }

    if (msg.errCode !== undefined) {
      resolved.errCode = msg.errCode;
    }
    else {
      resolved.errCode = 0
    }

    return resolved;
    }
};

module.exports = module_abnormal_info;
