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

class cameraCalibrationRequest {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.camera_name = null;
      this.camera_matrix = null;
      this.distortion_coefficients = null;
      this.tx = null;
      this.ty = null;
      this.tz = null;
      this.roll = null;
      this.pitch = null;
      this.yaw = null;
    }
    else {
      if (initObj.hasOwnProperty('camera_name')) {
        this.camera_name = initObj.camera_name
      }
      else {
        this.camera_name = '';
      }
      if (initObj.hasOwnProperty('camera_matrix')) {
        this.camera_matrix = initObj.camera_matrix
      }
      else {
        this.camera_matrix = '';
      }
      if (initObj.hasOwnProperty('distortion_coefficients')) {
        this.distortion_coefficients = initObj.distortion_coefficients
      }
      else {
        this.distortion_coefficients = '';
      }
      if (initObj.hasOwnProperty('tx')) {
        this.tx = initObj.tx
      }
      else {
        this.tx = 0.0;
      }
      if (initObj.hasOwnProperty('ty')) {
        this.ty = initObj.ty
      }
      else {
        this.ty = 0.0;
      }
      if (initObj.hasOwnProperty('tz')) {
        this.tz = initObj.tz
      }
      else {
        this.tz = 0.0;
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
    // Serializes a message object of type cameraCalibrationRequest
    // Serialize message field [camera_name]
    bufferOffset = _serializer.string(obj.camera_name, buffer, bufferOffset);
    // Serialize message field [camera_matrix]
    bufferOffset = _serializer.string(obj.camera_matrix, buffer, bufferOffset);
    // Serialize message field [distortion_coefficients]
    bufferOffset = _serializer.string(obj.distortion_coefficients, buffer, bufferOffset);
    // Serialize message field [tx]
    bufferOffset = _serializer.float32(obj.tx, buffer, bufferOffset);
    // Serialize message field [ty]
    bufferOffset = _serializer.float32(obj.ty, buffer, bufferOffset);
    // Serialize message field [tz]
    bufferOffset = _serializer.float32(obj.tz, buffer, bufferOffset);
    // Serialize message field [roll]
    bufferOffset = _serializer.float32(obj.roll, buffer, bufferOffset);
    // Serialize message field [pitch]
    bufferOffset = _serializer.float32(obj.pitch, buffer, bufferOffset);
    // Serialize message field [yaw]
    bufferOffset = _serializer.float32(obj.yaw, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type cameraCalibrationRequest
    let len;
    let data = new cameraCalibrationRequest(null);
    // Deserialize message field [camera_name]
    data.camera_name = _deserializer.string(buffer, bufferOffset);
    // Deserialize message field [camera_matrix]
    data.camera_matrix = _deserializer.string(buffer, bufferOffset);
    // Deserialize message field [distortion_coefficients]
    data.distortion_coefficients = _deserializer.string(buffer, bufferOffset);
    // Deserialize message field [tx]
    data.tx = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [ty]
    data.ty = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [tz]
    data.tz = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [roll]
    data.roll = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [pitch]
    data.pitch = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [yaw]
    data.yaw = _deserializer.float32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += _getByteLength(object.camera_name);
    length += _getByteLength(object.camera_matrix);
    length += _getByteLength(object.distortion_coefficients);
    return length + 36;
  }

  static datatype() {
    // Returns string type for a service object
    return 'comm_srvs/cameraCalibrationRequest';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'd1f61d55054eb6053da0982bdfcf94d9';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    string  camera_name             #相机名称
    string  camera_matrix           #相机内参
    string  distortion_coefficients #畸变系数
    float32 tx
    float32 ty
    float32 tz
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
    const resolved = new cameraCalibrationRequest(null);
    if (msg.camera_name !== undefined) {
      resolved.camera_name = msg.camera_name;
    }
    else {
      resolved.camera_name = ''
    }

    if (msg.camera_matrix !== undefined) {
      resolved.camera_matrix = msg.camera_matrix;
    }
    else {
      resolved.camera_matrix = ''
    }

    if (msg.distortion_coefficients !== undefined) {
      resolved.distortion_coefficients = msg.distortion_coefficients;
    }
    else {
      resolved.distortion_coefficients = ''
    }

    if (msg.tx !== undefined) {
      resolved.tx = msg.tx;
    }
    else {
      resolved.tx = 0.0
    }

    if (msg.ty !== undefined) {
      resolved.ty = msg.ty;
    }
    else {
      resolved.ty = 0.0
    }

    if (msg.tz !== undefined) {
      resolved.tz = msg.tz;
    }
    else {
      resolved.tz = 0.0
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

class cameraCalibrationResponse {
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
    // Serializes a message object of type cameraCalibrationResponse
    // Serialize message field [success]
    bufferOffset = _serializer.bool(obj.success, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type cameraCalibrationResponse
    let len;
    let data = new cameraCalibrationResponse(null);
    // Deserialize message field [success]
    data.success = _deserializer.bool(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 1;
  }

  static datatype() {
    // Returns string type for a service object
    return 'comm_srvs/cameraCalibrationResponse';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '358e233cde0c8a8bcfea4ce193f8fc15';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    bool    success
    
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new cameraCalibrationResponse(null);
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
  Request: cameraCalibrationRequest,
  Response: cameraCalibrationResponse,
  md5sum() { return '79a62af09061e3caf062e9248160d26b'; },
  datatype() { return 'comm_srvs/cameraCalibration'; }
};
