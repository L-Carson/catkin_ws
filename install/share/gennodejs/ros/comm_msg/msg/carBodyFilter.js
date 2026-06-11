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

class carBodyFilter {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.filter = null;
    }
    else {
      if (initObj.hasOwnProperty('filter')) {
        this.filter = initObj.filter
      }
      else {
        this.filter = 0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type carBodyFilter
    // Serialize message field [filter]
    bufferOffset = _serializer.uint8(obj.filter, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type carBodyFilter
    let len;
    let data = new carBodyFilter(null);
    // Deserialize message field [filter]
    data.filter = _deserializer.uint8(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 1;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/carBodyFilter';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'ca8270e9d6440d7014080687771919c9';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    # 0 全量构建
    # 1 增量构建
    # 2 保存并启用
    # 3 取消构建
    
    uint8 filter
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new carBodyFilter(null);
    if (msg.filter !== undefined) {
      resolved.filter = msg.filter;
    }
    else {
      resolved.filter = 0
    }

    return resolved;
    }
};

module.exports = carBodyFilter;
