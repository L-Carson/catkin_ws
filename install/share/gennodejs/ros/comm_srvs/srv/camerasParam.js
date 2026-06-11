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

let cameraParam = require('../msg/cameraParam.js');

//-----------------------------------------------------------

class camerasParamRequest {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
    }
    else {
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type camerasParamRequest
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type camerasParamRequest
    let len;
    let data = new camerasParamRequest(null);
    return data;
  }

  static getMessageSize(object) {
    return 0;
  }

  static datatype() {
    // Returns string type for a service object
    return 'comm_srvs/camerasParamRequest';
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
    const resolved = new camerasParamRequest(null);
    return resolved;
    }
};

class camerasParamResponse {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.success = null;
      this.cameraParamArray = null;
    }
    else {
      if (initObj.hasOwnProperty('success')) {
        this.success = initObj.success
      }
      else {
        this.success = false;
      }
      if (initObj.hasOwnProperty('cameraParamArray')) {
        this.cameraParamArray = initObj.cameraParamArray
      }
      else {
        this.cameraParamArray = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type camerasParamResponse
    // Serialize message field [success]
    bufferOffset = _serializer.bool(obj.success, buffer, bufferOffset);
    // Serialize message field [cameraParamArray]
    // Serialize the length for message field [cameraParamArray]
    bufferOffset = _serializer.uint32(obj.cameraParamArray.length, buffer, bufferOffset);
    obj.cameraParamArray.forEach((val) => {
      bufferOffset = cameraParam.serialize(val, buffer, bufferOffset);
    });
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type camerasParamResponse
    let len;
    let data = new camerasParamResponse(null);
    // Deserialize message field [success]
    data.success = _deserializer.bool(buffer, bufferOffset);
    // Deserialize message field [cameraParamArray]
    // Deserialize array length for message field [cameraParamArray]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.cameraParamArray = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.cameraParamArray[i] = cameraParam.deserialize(buffer, bufferOffset)
    }
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    object.cameraParamArray.forEach((val) => {
      length += cameraParam.getMessageSize(val);
    });
    return length + 5;
  }

  static datatype() {
    // Returns string type for a service object
    return 'comm_srvs/camerasParamResponse';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'fd18540e50d2c9de8ab7b244039cf247';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    bool          success
    cameraParam[] cameraParamArray
    
    ================================================================================
    MSG: comm_srvs/cameraParam
    string      name_id
    string      port_name
    uint32      frame_hz
    
    uint32      height
    uint32      width
    
    float32     tx
    float32     ty
    float32     tz
    float32     roll
    float32     pitch
    float32     yaw
    
    float64[]   camera_matrix
    float64[]   distortion_coefficients
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new camerasParamResponse(null);
    if (msg.success !== undefined) {
      resolved.success = msg.success;
    }
    else {
      resolved.success = false
    }

    if (msg.cameraParamArray !== undefined) {
      resolved.cameraParamArray = new Array(msg.cameraParamArray.length);
      for (let i = 0; i < resolved.cameraParamArray.length; ++i) {
        resolved.cameraParamArray[i] = cameraParam.Resolve(msg.cameraParamArray[i]);
      }
    }
    else {
      resolved.cameraParamArray = []
    }

    return resolved;
    }
};

module.exports = {
  Request: camerasParamRequest,
  Response: camerasParamResponse,
  md5sum() { return 'fd18540e50d2c9de8ab7b244039cf247'; },
  datatype() { return 'comm_srvs/camerasParam'; }
};
