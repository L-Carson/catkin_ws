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

class locationInfo {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.status = null;
      this.subMapNo = null;
      this.posX = null;
      this.posY = null;
      this.dirAngle = null;
    }
    else {
      if (initObj.hasOwnProperty('status')) {
        this.status = initObj.status
      }
      else {
        this.status = 0;
      }
      if (initObj.hasOwnProperty('subMapNo')) {
        this.subMapNo = initObj.subMapNo
      }
      else {
        this.subMapNo = 0;
      }
      if (initObj.hasOwnProperty('posX')) {
        this.posX = initObj.posX
      }
      else {
        this.posX = 0.0;
      }
      if (initObj.hasOwnProperty('posY')) {
        this.posY = initObj.posY
      }
      else {
        this.posY = 0.0;
      }
      if (initObj.hasOwnProperty('dirAngle')) {
        this.dirAngle = initObj.dirAngle
      }
      else {
        this.dirAngle = 0.0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type locationInfo
    // Serialize message field [status]
    bufferOffset = _serializer.int32(obj.status, buffer, bufferOffset);
    // Serialize message field [subMapNo]
    bufferOffset = _serializer.int32(obj.subMapNo, buffer, bufferOffset);
    // Serialize message field [posX]
    bufferOffset = _serializer.float32(obj.posX, buffer, bufferOffset);
    // Serialize message field [posY]
    bufferOffset = _serializer.float32(obj.posY, buffer, bufferOffset);
    // Serialize message field [dirAngle]
    bufferOffset = _serializer.float32(obj.dirAngle, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type locationInfo
    let len;
    let data = new locationInfo(null);
    // Deserialize message field [status]
    data.status = _deserializer.int32(buffer, bufferOffset);
    // Deserialize message field [subMapNo]
    data.subMapNo = _deserializer.int32(buffer, bufferOffset);
    // Deserialize message field [posX]
    data.posX = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [posY]
    data.posY = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [dirAngle]
    data.dirAngle = _deserializer.float32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 20;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/locationInfo';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'b3a48261c7b0aa3907b3479915747d21';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    # location status, refer to MAP_NODE_STATE_ENUM
    int32 status
    
    # sub map no, this is valid when >= 0, if -ing status, this is the new sub map no
    int32 subMapNo
    
    # Comb Map pose, which is valid When status is > 0.
    float32 posX
    float32 posY
    float32 dirAngle
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new locationInfo(null);
    if (msg.status !== undefined) {
      resolved.status = msg.status;
    }
    else {
      resolved.status = 0
    }

    if (msg.subMapNo !== undefined) {
      resolved.subMapNo = msg.subMapNo;
    }
    else {
      resolved.subMapNo = 0
    }

    if (msg.posX !== undefined) {
      resolved.posX = msg.posX;
    }
    else {
      resolved.posX = 0.0
    }

    if (msg.posY !== undefined) {
      resolved.posY = msg.posY;
    }
    else {
      resolved.posY = 0.0
    }

    if (msg.dirAngle !== undefined) {
      resolved.dirAngle = msg.dirAngle;
    }
    else {
      resolved.dirAngle = 0.0
    }

    return resolved;
    }
};

module.exports = locationInfo;
