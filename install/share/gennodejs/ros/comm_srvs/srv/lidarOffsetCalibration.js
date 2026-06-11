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

class lidarOffsetCalibrationRequest {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.lidarOffsetParam = null;
    }
    else {
      if (initObj.hasOwnProperty('lidarOffsetParam')) {
        this.lidarOffsetParam = initObj.lidarOffsetParam
      }
      else {
        this.lidarOffsetParam = new sensorInstallParam();
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type lidarOffsetCalibrationRequest
    // Serialize message field [lidarOffsetParam]
    bufferOffset = sensorInstallParam.serialize(obj.lidarOffsetParam, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type lidarOffsetCalibrationRequest
    let len;
    let data = new lidarOffsetCalibrationRequest(null);
    // Deserialize message field [lidarOffsetParam]
    data.lidarOffsetParam = sensorInstallParam.deserialize(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += sensorInstallParam.getMessageSize(object.lidarOffsetParam);
    return length;
  }

  static datatype() {
    // Returns string type for a service object
    return 'comm_srvs/lidarOffsetCalibrationRequest';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'a058fba49a87770dedd8fc5190bc8a14';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    sensorInstallParam lidarOffsetParam
    
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
    const resolved = new lidarOffsetCalibrationRequest(null);
    if (msg.lidarOffsetParam !== undefined) {
      resolved.lidarOffsetParam = sensorInstallParam.Resolve(msg.lidarOffsetParam)
    }
    else {
      resolved.lidarOffsetParam = new sensorInstallParam()
    }

    return resolved;
    }
};

class lidarOffsetCalibrationResponse {
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
    // Serializes a message object of type lidarOffsetCalibrationResponse
    // Serialize message field [success]
    bufferOffset = _serializer.bool(obj.success, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type lidarOffsetCalibrationResponse
    let len;
    let data = new lidarOffsetCalibrationResponse(null);
    // Deserialize message field [success]
    data.success = _deserializer.bool(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 1;
  }

  static datatype() {
    // Returns string type for a service object
    return 'comm_srvs/lidarOffsetCalibrationResponse';
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
    const resolved = new lidarOffsetCalibrationResponse(null);
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
  Request: lidarOffsetCalibrationRequest,
  Response: lidarOffsetCalibrationResponse,
  md5sum() { return 'c4589f9ccc2d3ea9fba7e7bffbeb08d8'; },
  datatype() { return 'comm_srvs/lidarOffsetCalibration'; }
};
