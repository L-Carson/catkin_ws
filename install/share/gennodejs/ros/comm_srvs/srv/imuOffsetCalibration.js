// Auto-generated. Do not edit!

// (in-package comm_srvs.srv)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let sensorInstallParam = require('../msg/sensorInstallParam.js');

//-----------------------------------------------------------


//-----------------------------------------------------------

class imuOffsetCalibrationRequest {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.imuOffsetParam = null;
    }
    else {
      if (initObj.hasOwnProperty('imuOffsetParam')) {
        this.imuOffsetParam = initObj.imuOffsetParam
      }
      else {
        this.imuOffsetParam = new sensorInstallParam();
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type imuOffsetCalibrationRequest
    // Serialize message field [imuOffsetParam]
    bufferOffset = sensorInstallParam.serialize(obj.imuOffsetParam, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type imuOffsetCalibrationRequest
    let len;
    let data = new imuOffsetCalibrationRequest(null);
    // Deserialize message field [imuOffsetParam]
    data.imuOffsetParam = sensorInstallParam.deserialize(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += sensorInstallParam.getMessageSize(object.imuOffsetParam);
    return length;
  }

  static datatype() {
    // Returns string type for a service object
    return 'comm_srvs/imuOffsetCalibrationRequest';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'd2739ef2f80783702df9e25beefc94a7';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    sensorInstallParam imuOffsetParam
    
    ================================================================================
    MSG: comm_srvs/sensorInstallParam
    string  name 
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
    const resolved = new imuOffsetCalibrationRequest(null);
    if (msg.imuOffsetParam !== undefined) {
      resolved.imuOffsetParam = sensorInstallParam.Resolve(msg.imuOffsetParam)
    }
    else {
      resolved.imuOffsetParam = new sensorInstallParam()
    }

    return resolved;
    }
};

class imuOffsetCalibrationResponse {
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
    // Serializes a message object of type imuOffsetCalibrationResponse
    // Serialize message field [success]
    bufferOffset = _serializer.bool(obj.success, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type imuOffsetCalibrationResponse
    let len;
    let data = new imuOffsetCalibrationResponse(null);
    // Deserialize message field [success]
    data.success = _deserializer.bool(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 1;
  }

  static datatype() {
    // Returns string type for a service object
    return 'comm_srvs/imuOffsetCalibrationResponse';
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
    const resolved = new imuOffsetCalibrationResponse(null);
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
  Request: imuOffsetCalibrationRequest,
  Response: imuOffsetCalibrationResponse,
  md5sum() { return 'c4b920e716f4c5969027af9eec7a747b'; },
  datatype() { return 'comm_srvs/imuOffsetCalibration'; }
};
