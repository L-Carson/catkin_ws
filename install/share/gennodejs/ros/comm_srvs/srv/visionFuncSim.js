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


//-----------------------------------------------------------

class visionFuncSimRequest {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.qrCodeId = null;
      this.funcId = null;
      this.isUseIpu = null;
      this.hz = null;
      this.camera = null;
    }
    else {
      if (initObj.hasOwnProperty('qrCodeId')) {
        this.qrCodeId = initObj.qrCodeId
      }
      else {
        this.qrCodeId = 0;
      }
      if (initObj.hasOwnProperty('funcId')) {
        this.funcId = initObj.funcId
      }
      else {
        this.funcId = 0;
      }
      if (initObj.hasOwnProperty('isUseIpu')) {
        this.isUseIpu = initObj.isUseIpu
      }
      else {
        this.isUseIpu = false;
      }
      if (initObj.hasOwnProperty('hz')) {
        this.hz = initObj.hz
      }
      else {
        this.hz = 0;
      }
      if (initObj.hasOwnProperty('camera')) {
        this.camera = initObj.camera
      }
      else {
        this.camera = '';
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type visionFuncSimRequest
    // Serialize message field [qrCodeId]
    bufferOffset = _serializer.uint8(obj.qrCodeId, buffer, bufferOffset);
    // Serialize message field [funcId]
    bufferOffset = _serializer.uint8(obj.funcId, buffer, bufferOffset);
    // Serialize message field [isUseIpu]
    bufferOffset = _serializer.bool(obj.isUseIpu, buffer, bufferOffset);
    // Serialize message field [hz]
    bufferOffset = _serializer.uint8(obj.hz, buffer, bufferOffset);
    // Serialize message field [camera]
    bufferOffset = _serializer.string(obj.camera, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type visionFuncSimRequest
    let len;
    let data = new visionFuncSimRequest(null);
    // Deserialize message field [qrCodeId]
    data.qrCodeId = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [funcId]
    data.funcId = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [isUseIpu]
    data.isUseIpu = _deserializer.bool(buffer, bufferOffset);
    // Deserialize message field [hz]
    data.hz = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [camera]
    data.camera = _deserializer.string(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += _getByteLength(object.camera);
    return length + 8;
  }

  static datatype() {
    // Returns string type for a service object
    return 'comm_srvs/visionFuncSimRequest';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'da544cf58d9de4cf161b40840247c52c';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    uint8       qrCodeId
    uint8       funcId
    bool        isUseIpu
    uint8       hz
    string      camera
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new visionFuncSimRequest(null);
    if (msg.qrCodeId !== undefined) {
      resolved.qrCodeId = msg.qrCodeId;
    }
    else {
      resolved.qrCodeId = 0
    }

    if (msg.funcId !== undefined) {
      resolved.funcId = msg.funcId;
    }
    else {
      resolved.funcId = 0
    }

    if (msg.isUseIpu !== undefined) {
      resolved.isUseIpu = msg.isUseIpu;
    }
    else {
      resolved.isUseIpu = false
    }

    if (msg.hz !== undefined) {
      resolved.hz = msg.hz;
    }
    else {
      resolved.hz = 0
    }

    if (msg.camera !== undefined) {
      resolved.camera = msg.camera;
    }
    else {
      resolved.camera = ''
    }

    return resolved;
    }
};

class visionFuncSimResponse {
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
    // Serializes a message object of type visionFuncSimResponse
    // Serialize message field [success]
    bufferOffset = _serializer.bool(obj.success, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type visionFuncSimResponse
    let len;
    let data = new visionFuncSimResponse(null);
    // Deserialize message field [success]
    data.success = _deserializer.bool(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 1;
  }

  static datatype() {
    // Returns string type for a service object
    return 'comm_srvs/visionFuncSimResponse';
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
    const resolved = new visionFuncSimResponse(null);
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
  Request: visionFuncSimRequest,
  Response: visionFuncSimResponse,
  md5sum() { return '0a3c827bca110ade616a89594700a208'; },
  datatype() { return 'comm_srvs/visionFuncSim'; }
};
