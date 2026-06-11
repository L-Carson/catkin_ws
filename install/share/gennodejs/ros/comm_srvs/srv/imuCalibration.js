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

class imuCalibrationRequest {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.imuClbtParam = null;
    }
    else {
      if (initObj.hasOwnProperty('imuClbtParam')) {
        this.imuClbtParam = initObj.imuClbtParam
      }
      else {
        this.imuClbtParam = new sensorInstallParam();
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type imuCalibrationRequest
    // Serialize message field [imuClbtParam]
    bufferOffset = sensorInstallParam.serialize(obj.imuClbtParam, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type imuCalibrationRequest
    let len;
    let data = new imuCalibrationRequest(null);
    // Deserialize message field [imuClbtParam]
    data.imuClbtParam = sensorInstallParam.deserialize(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += sensorInstallParam.getMessageSize(object.imuClbtParam);
    return length;
  }

  static datatype() {
    // Returns string type for a service object
    return 'comm_srvs/imuCalibrationRequest';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '444d7ce317880639a96b68c203549e26';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    sensorInstallParam imuClbtParam
    
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
    const resolved = new imuCalibrationRequest(null);
    if (msg.imuClbtParam !== undefined) {
      resolved.imuClbtParam = sensorInstallParam.Resolve(msg.imuClbtParam)
    }
    else {
      resolved.imuClbtParam = new sensorInstallParam()
    }

    return resolved;
    }
};

class imuCalibrationResponse {
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
    // Serializes a message object of type imuCalibrationResponse
    // Serialize message field [success]
    bufferOffset = _serializer.bool(obj.success, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type imuCalibrationResponse
    let len;
    let data = new imuCalibrationResponse(null);
    // Deserialize message field [success]
    data.success = _deserializer.bool(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 1;
  }

  static datatype() {
    // Returns string type for a service object
    return 'comm_srvs/imuCalibrationResponse';
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
    const resolved = new imuCalibrationResponse(null);
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
  Request: imuCalibrationRequest,
  Response: imuCalibrationResponse,
  md5sum() { return 'bf5a50f919bcad97898b19cc4dfd4322'; },
  datatype() { return 'comm_srvs/imuCalibration'; }
};
