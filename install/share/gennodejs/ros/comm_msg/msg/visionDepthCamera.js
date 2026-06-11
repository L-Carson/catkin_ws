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

class visionDepthCamera {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.selectDepthCamera = null;
    }
    else {
      if (initObj.hasOwnProperty('selectDepthCamera')) {
        this.selectDepthCamera = initObj.selectDepthCamera
      }
      else {
        this.selectDepthCamera = 0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type visionDepthCamera
    // Serialize message field [selectDepthCamera]
    bufferOffset = _serializer.uint8(obj.selectDepthCamera, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type visionDepthCamera
    let len;
    let data = new visionDepthCamera(null);
    // Deserialize message field [selectDepthCamera]
    data.selectDepthCamera = _deserializer.uint8(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 1;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/visionDepthCamera';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '81b78f084b6d77e189e72dcfcb2c3fa3';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    # Select Depth Camera
    # 0: not select depth camera
    # 1: the head depth camera
    # 2: the back depth camera
    # 3: the left depth camera
    # 4: the right depth camera
    
    uint8 selectDepthCamera
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new visionDepthCamera(null);
    if (msg.selectDepthCamera !== undefined) {
      resolved.selectDepthCamera = msg.selectDepthCamera;
    }
    else {
      resolved.selectDepthCamera = 0
    }

    return resolved;
    }
};

module.exports = visionDepthCamera;
