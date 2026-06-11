// Auto-generated. Do not edit!

// (in-package comm_srvs.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class cameraParam {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.name_id = null;
      this.port_name = null;
      this.frame_hz = null;
      this.height = null;
      this.width = null;
      this.tx = null;
      this.ty = null;
      this.tz = null;
      this.roll = null;
      this.pitch = null;
      this.yaw = null;
      this.camera_matrix = null;
      this.distortion_coefficients = null;
    }
    else {
      if (initObj.hasOwnProperty('name_id')) {
        this.name_id = initObj.name_id
      }
      else {
        this.name_id = '';
      }
      if (initObj.hasOwnProperty('port_name')) {
        this.port_name = initObj.port_name
      }
      else {
        this.port_name = '';
      }
      if (initObj.hasOwnProperty('frame_hz')) {
        this.frame_hz = initObj.frame_hz
      }
      else {
        this.frame_hz = 0;
      }
      if (initObj.hasOwnProperty('height')) {
        this.height = initObj.height
      }
      else {
        this.height = 0;
      }
      if (initObj.hasOwnProperty('width')) {
        this.width = initObj.width
      }
      else {
        this.width = 0;
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
      if (initObj.hasOwnProperty('camera_matrix')) {
        this.camera_matrix = initObj.camera_matrix
      }
      else {
        this.camera_matrix = [];
      }
      if (initObj.hasOwnProperty('distortion_coefficients')) {
        this.distortion_coefficients = initObj.distortion_coefficients
      }
      else {
        this.distortion_coefficients = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type cameraParam
    // Serialize message field [name_id]
    bufferOffset = _serializer.string(obj.name_id, buffer, bufferOffset);
    // Serialize message field [port_name]
    bufferOffset = _serializer.string(obj.port_name, buffer, bufferOffset);
    // Serialize message field [frame_hz]
    bufferOffset = _serializer.uint32(obj.frame_hz, buffer, bufferOffset);
    // Serialize message field [height]
    bufferOffset = _serializer.uint32(obj.height, buffer, bufferOffset);
    // Serialize message field [width]
    bufferOffset = _serializer.uint32(obj.width, buffer, bufferOffset);
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
    // Serialize message field [camera_matrix]
    bufferOffset = _arraySerializer.float64(obj.camera_matrix, buffer, bufferOffset, null);
    // Serialize message field [distortion_coefficients]
    bufferOffset = _arraySerializer.float64(obj.distortion_coefficients, buffer, bufferOffset, null);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type cameraParam
    let len;
    let data = new cameraParam(null);
    // Deserialize message field [name_id]
    data.name_id = _deserializer.string(buffer, bufferOffset);
    // Deserialize message field [port_name]
    data.port_name = _deserializer.string(buffer, bufferOffset);
    // Deserialize message field [frame_hz]
    data.frame_hz = _deserializer.uint32(buffer, bufferOffset);
    // Deserialize message field [height]
    data.height = _deserializer.uint32(buffer, bufferOffset);
    // Deserialize message field [width]
    data.width = _deserializer.uint32(buffer, bufferOffset);
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
    // Deserialize message field [camera_matrix]
    data.camera_matrix = _arrayDeserializer.float64(buffer, bufferOffset, null)
    // Deserialize message field [distortion_coefficients]
    data.distortion_coefficients = _arrayDeserializer.float64(buffer, bufferOffset, null)
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += _getByteLength(object.name_id);
    length += _getByteLength(object.port_name);
    length += 8 * object.camera_matrix.length;
    length += 8 * object.distortion_coefficients.length;
    return length + 52;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_srvs/cameraParam';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '4f9f9ccd79905f101e4555a21c54abfd';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
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
    const resolved = new cameraParam(null);
    if (msg.name_id !== undefined) {
      resolved.name_id = msg.name_id;
    }
    else {
      resolved.name_id = ''
    }

    if (msg.port_name !== undefined) {
      resolved.port_name = msg.port_name;
    }
    else {
      resolved.port_name = ''
    }

    if (msg.frame_hz !== undefined) {
      resolved.frame_hz = msg.frame_hz;
    }
    else {
      resolved.frame_hz = 0
    }

    if (msg.height !== undefined) {
      resolved.height = msg.height;
    }
    else {
      resolved.height = 0
    }

    if (msg.width !== undefined) {
      resolved.width = msg.width;
    }
    else {
      resolved.width = 0
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

    if (msg.camera_matrix !== undefined) {
      resolved.camera_matrix = msg.camera_matrix;
    }
    else {
      resolved.camera_matrix = []
    }

    if (msg.distortion_coefficients !== undefined) {
      resolved.distortion_coefficients = msg.distortion_coefficients;
    }
    else {
      resolved.distortion_coefficients = []
    }

    return resolved;
    }
};

module.exports = cameraParam;
