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

class pose3D {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.posX = null;
      this.posY = null;
      this.posZ = null;
      this.roll = null;
      this.pitch = null;
      this.yaw = null;
    }
    else {
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
      if (initObj.hasOwnProperty('posZ')) {
        this.posZ = initObj.posZ
      }
      else {
        this.posZ = 0.0;
      }
      if (initObj.hasOwnProperty('roll')) {
        this.roll = initObj.roll
      }
      else {
        this.roll = 0.0;
      }
      if (initObj.hasOwnProperty('pitch')) {
        this.pitch = initObj.pitch
      }
      else {
        this.pitch = 0.0;
      }
      if (initObj.hasOwnProperty('yaw')) {
        this.yaw = initObj.yaw
      }
      else {
        this.yaw = 0.0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type pose3D
    // Serialize message field [posX]
    bufferOffset = _serializer.float32(obj.posX, buffer, bufferOffset);
    // Serialize message field [posY]
    bufferOffset = _serializer.float32(obj.posY, buffer, bufferOffset);
    // Serialize message field [posZ]
    bufferOffset = _serializer.float32(obj.posZ, buffer, bufferOffset);
    // Serialize message field [roll]
    bufferOffset = _serializer.float32(obj.roll, buffer, bufferOffset);
    // Serialize message field [pitch]
    bufferOffset = _serializer.float32(obj.pitch, buffer, bufferOffset);
    // Serialize message field [yaw]
    bufferOffset = _serializer.float32(obj.yaw, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type pose3D
    let len;
    let data = new pose3D(null);
    // Deserialize message field [posX]
    data.posX = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [posY]
    data.posY = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [posZ]
    data.posZ = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [roll]
    data.roll = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [pitch]
    data.pitch = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [yaw]
    data.yaw = _deserializer.float32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 24;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/pose3D';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'b0f6a63f05aca4ae94f3fa6431f6fc26';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    float32 posX
    float32 posY
    float32 posZ
    float32 roll
    float32 pitch
    float32 yaw
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new pose3D(null);
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

    if (msg.posZ !== undefined) {
      resolved.posZ = msg.posZ;
    }
    else {
      resolved.posZ = 0.0
    }

    if (msg.roll !== undefined) {
      resolved.roll = msg.roll;
    }
    else {
      resolved.roll = 0.0
    }

    if (msg.pitch !== undefined) {
      resolved.pitch = msg.pitch;
    }
    else {
      resolved.pitch = 0.0
    }

    if (msg.yaw !== undefined) {
      resolved.yaw = msg.yaw;
    }
    else {
      resolved.yaw = 0.0
    }

    return resolved;
    }
};

module.exports = pose3D;
