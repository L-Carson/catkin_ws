// Auto-generated. Do not edit!

// (in-package comm_srvs.srv)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let LidarInstallParam = require('../msg/LidarInstallParam.js');

//-----------------------------------------------------------


//-----------------------------------------------------------

class lidarCalibrationRequest {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.lidarsClbtParam = null;
    }
    else {
      if (initObj.hasOwnProperty('lidarsClbtParam')) {
        this.lidarsClbtParam = initObj.lidarsClbtParam
      }
      else {
        this.lidarsClbtParam = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type lidarCalibrationRequest
    // Serialize message field [lidarsClbtParam]
    // Serialize the length for message field [lidarsClbtParam]
    bufferOffset = _serializer.uint32(obj.lidarsClbtParam.length, buffer, bufferOffset);
    obj.lidarsClbtParam.forEach((val) => {
      bufferOffset = LidarInstallParam.serialize(val, buffer, bufferOffset);
    });
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type lidarCalibrationRequest
    let len;
    let data = new lidarCalibrationRequest(null);
    // Deserialize message field [lidarsClbtParam]
    // Deserialize array length for message field [lidarsClbtParam]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.lidarsClbtParam = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.lidarsClbtParam[i] = LidarInstallParam.deserialize(buffer, bufferOffset)
    }
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    object.lidarsClbtParam.forEach((val) => {
      length += LidarInstallParam.getMessageSize(val);
    });
    return length + 4;
  }

  static datatype() {
    // Returns string type for a service object
    return 'comm_srvs/lidarCalibrationRequest';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '303245169c7a62314dd1620235065c87';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    LidarInstallParam[] lidarsClbtParam
    
    ================================================================================
    MSG: comm_srvs/LidarInstallParam
    string  name    #MainLidar BlindLidar BlindLidar2
    string  serialNum
    float32 x
    float32 y
    float32 z
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
    const resolved = new lidarCalibrationRequest(null);
    if (msg.lidarsClbtParam !== undefined) {
      resolved.lidarsClbtParam = new Array(msg.lidarsClbtParam.length);
      for (let i = 0; i < resolved.lidarsClbtParam.length; ++i) {
        resolved.lidarsClbtParam[i] = LidarInstallParam.Resolve(msg.lidarsClbtParam[i]);
      }
    }
    else {
      resolved.lidarsClbtParam = []
    }

    return resolved;
    }
};

class lidarCalibrationResponse {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.success = null;
    }
    else {
      if (initObj.hasOwnProperty('success')) {
        this.success = initObj.success
      }
      else {
        this.success = false;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type lidarCalibrationResponse
    // Serialize message field [success]
    bufferOffset = _serializer.bool(obj.success, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type lidarCalibrationResponse
    let len;
    let data = new lidarCalibrationResponse(null);
    // Deserialize message field [success]
    data.success = _deserializer.bool(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 1;
  }

  static datatype() {
    // Returns string type for a service object
    return 'comm_srvs/lidarCalibrationResponse';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '358e233cde0c8a8bcfea4ce193f8fc15';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    bool        success
    
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new lidarCalibrationResponse(null);
    if (msg.success !== undefined) {
      resolved.success = msg.success;
    }
    else {
      resolved.success = false
    }

    return resolved;
    }
};

module.exports = {
  Request: lidarCalibrationRequest,
  Response: lidarCalibrationResponse,
  md5sum() { return 'a672746d2ead307649fd58de19b13665'; },
  datatype() { return 'comm_srvs/lidarCalibration'; }
};
