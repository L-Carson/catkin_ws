// Auto-generated. Do not edit!

// (in-package comm_srvs.srv)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

let LidarInstallParam = require('../msg/LidarInstallParam.js');

//-----------------------------------------------------------

class PcdNetRequest {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
    }
    else {
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type PcdNetRequest
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type PcdNetRequest
    let len;
    let data = new PcdNetRequest(null);
    return data;
  }

  static getMessageSize(object) {
    return 0;
  }

  static datatype() {
    // Returns string type for a service object
    return 'comm_srvs/PcdNetRequest';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'd41d8cd98f00b204e9800998ecf8427e';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new PcdNetRequest(null);
    return resolved;
    }
};

class PcdNetResponse {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.success = null;
      this.isOpen = null;
      this.lidarInstallParamArray = null;
    }
    else {
      if (initObj.hasOwnProperty('success')) {
        this.success = initObj.success
      }
      else {
        this.success = false;
      }
      if (initObj.hasOwnProperty('isOpen')) {
        this.isOpen = initObj.isOpen
      }
      else {
        this.isOpen = false;
      }
      if (initObj.hasOwnProperty('lidarInstallParamArray')) {
        this.lidarInstallParamArray = initObj.lidarInstallParamArray
      }
      else {
        this.lidarInstallParamArray = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type PcdNetResponse
    // Serialize message field [success]
    bufferOffset = _serializer.bool(obj.success, buffer, bufferOffset);
    // Serialize message field [isOpen]
    bufferOffset = _serializer.bool(obj.isOpen, buffer, bufferOffset);
    // Serialize message field [lidarInstallParamArray]
    // Serialize the length for message field [lidarInstallParamArray]
    bufferOffset = _serializer.uint32(obj.lidarInstallParamArray.length, buffer, bufferOffset);
    obj.lidarInstallParamArray.forEach((val) => {
      bufferOffset = LidarInstallParam.serialize(val, buffer, bufferOffset);
    });
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type PcdNetResponse
    let len;
    let data = new PcdNetResponse(null);
    // Deserialize message field [success]
    data.success = _deserializer.bool(buffer, bufferOffset);
    // Deserialize message field [isOpen]
    data.isOpen = _deserializer.bool(buffer, bufferOffset);
    // Deserialize message field [lidarInstallParamArray]
    // Deserialize array length for message field [lidarInstallParamArray]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.lidarInstallParamArray = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.lidarInstallParamArray[i] = LidarInstallParam.deserialize(buffer, bufferOffset)
    }
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    object.lidarInstallParamArray.forEach((val) => {
      length += LidarInstallParam.getMessageSize(val);
    });
    return length + 6;
  }

  static datatype() {
    // Returns string type for a service object
    return 'comm_srvs/PcdNetResponse';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'd49762f82989f849a33296dad5e777da';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    bool    success
    bool    isOpen
    LidarInstallParam[] lidarInstallParamArray
    
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
    const resolved = new PcdNetResponse(null);
    if (msg.success !== undefined) {
      resolved.success = msg.success;
    }
    else {
      resolved.success = false
    }

    if (msg.isOpen !== undefined) {
      resolved.isOpen = msg.isOpen;
    }
    else {
      resolved.isOpen = false
    }

    if (msg.lidarInstallParamArray !== undefined) {
      resolved.lidarInstallParamArray = new Array(msg.lidarInstallParamArray.length);
      for (let i = 0; i < resolved.lidarInstallParamArray.length; ++i) {
        resolved.lidarInstallParamArray[i] = LidarInstallParam.Resolve(msg.lidarInstallParamArray[i]);
      }
    }
    else {
      resolved.lidarInstallParamArray = []
    }

    return resolved;
    }
};

module.exports = {
  Request: PcdNetRequest,
  Response: PcdNetResponse,
  md5sum() { return 'd49762f82989f849a33296dad5e777da'; },
  datatype() { return 'comm_srvs/PcdNet'; }
};
