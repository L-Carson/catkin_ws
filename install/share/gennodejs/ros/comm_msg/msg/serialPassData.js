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

class serialPassData {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.dataStream = null;
    }
    else {
      if (initObj.hasOwnProperty('dataStream')) {
        this.dataStream = initObj.dataStream
      }
      else {
        this.dataStream = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type serialPassData
    // Serialize message field [dataStream]
    bufferOffset = _arraySerializer.uint8(obj.dataStream, buffer, bufferOffset, null);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type serialPassData
    let len;
    let data = new serialPassData(null);
    // Deserialize message field [dataStream]
    data.dataStream = _arrayDeserializer.uint8(buffer, bufferOffset, null)
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += object.dataStream.length;
    return length + 4;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/serialPassData';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '356d46a591b661111ed0ede4b4ae78ca';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    # serial byte stream
    uint8[] dataStream
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new serialPassData(null);
    if (msg.dataStream !== undefined) {
      resolved.dataStream = msg.dataStream;
    }
    else {
      resolved.dataStream = []
    }

    return resolved;
    }
};

module.exports = serialPassData;
