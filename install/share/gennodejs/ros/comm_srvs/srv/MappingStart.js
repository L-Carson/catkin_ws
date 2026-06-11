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

class MappingStartRequest {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.mode = null;
      this.state = null;
      this.frontEndMethod = null;
      this.currFrameLeafSize = null;
      this.localMapLeafSize = null;
      this.localMapWindowSize = null;
      this.loopDetectDist = null;
      this.cfgDir = null;
      this.dstDir = null;
      this.bagPath = null;
      this.srcDir = null;
    }
    else {
      if (initObj.hasOwnProperty('mode')) {
        this.mode = initObj.mode
      }
      else {
        this.mode = 0;
      }
      if (initObj.hasOwnProperty('state')) {
        this.state = initObj.state
      }
      else {
        this.state = 0;
      }
      if (initObj.hasOwnProperty('frontEndMethod')) {
        this.frontEndMethod = initObj.frontEndMethod
      }
      else {
        this.frontEndMethod = 0;
      }
      if (initObj.hasOwnProperty('currFrameLeafSize')) {
        this.currFrameLeafSize = initObj.currFrameLeafSize
      }
      else {
        this.currFrameLeafSize = 0.0;
      }
      if (initObj.hasOwnProperty('localMapLeafSize')) {
        this.localMapLeafSize = initObj.localMapLeafSize
      }
      else {
        this.localMapLeafSize = 0.0;
      }
      if (initObj.hasOwnProperty('localMapWindowSize')) {
        this.localMapWindowSize = initObj.localMapWindowSize
      }
      else {
        this.localMapWindowSize = 0;
      }
      if (initObj.hasOwnProperty('loopDetectDist')) {
        this.loopDetectDist = initObj.loopDetectDist
      }
      else {
        this.loopDetectDist = 0.0;
      }
      if (initObj.hasOwnProperty('cfgDir')) {
        this.cfgDir = initObj.cfgDir
      }
      else {
        this.cfgDir = '';
      }
      if (initObj.hasOwnProperty('dstDir')) {
        this.dstDir = initObj.dstDir
      }
      else {
        this.dstDir = '';
      }
      if (initObj.hasOwnProperty('bagPath')) {
        this.bagPath = initObj.bagPath
      }
      else {
        this.bagPath = '';
      }
      if (initObj.hasOwnProperty('srcDir')) {
        this.srcDir = initObj.srcDir
      }
      else {
        this.srcDir = '';
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type MappingStartRequest
    // Serialize message field [mode]
    bufferOffset = _serializer.int32(obj.mode, buffer, bufferOffset);
    // Serialize message field [state]
    bufferOffset = _serializer.int32(obj.state, buffer, bufferOffset);
    // Serialize message field [frontEndMethod]
    bufferOffset = _serializer.int32(obj.frontEndMethod, buffer, bufferOffset);
    // Serialize message field [currFrameLeafSize]
    bufferOffset = _serializer.float32(obj.currFrameLeafSize, buffer, bufferOffset);
    // Serialize message field [localMapLeafSize]
    bufferOffset = _serializer.float32(obj.localMapLeafSize, buffer, bufferOffset);
    // Serialize message field [localMapWindowSize]
    bufferOffset = _serializer.int32(obj.localMapWindowSize, buffer, bufferOffset);
    // Serialize message field [loopDetectDist]
    bufferOffset = _serializer.float32(obj.loopDetectDist, buffer, bufferOffset);
    // Serialize message field [cfgDir]
    bufferOffset = _serializer.string(obj.cfgDir, buffer, bufferOffset);
    // Serialize message field [dstDir]
    bufferOffset = _serializer.string(obj.dstDir, buffer, bufferOffset);
    // Serialize message field [bagPath]
    bufferOffset = _serializer.string(obj.bagPath, buffer, bufferOffset);
    // Serialize message field [srcDir]
    bufferOffset = _serializer.string(obj.srcDir, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type MappingStartRequest
    let len;
    let data = new MappingStartRequest(null);
    // Deserialize message field [mode]
    data.mode = _deserializer.int32(buffer, bufferOffset);
    // Deserialize message field [state]
    data.state = _deserializer.int32(buffer, bufferOffset);
    // Deserialize message field [frontEndMethod]
    data.frontEndMethod = _deserializer.int32(buffer, bufferOffset);
    // Deserialize message field [currFrameLeafSize]
    data.currFrameLeafSize = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [localMapLeafSize]
    data.localMapLeafSize = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [localMapWindowSize]
    data.localMapWindowSize = _deserializer.int32(buffer, bufferOffset);
    // Deserialize message field [loopDetectDist]
    data.loopDetectDist = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [cfgDir]
    data.cfgDir = _deserializer.string(buffer, bufferOffset);
    // Deserialize message field [dstDir]
    data.dstDir = _deserializer.string(buffer, bufferOffset);
    // Deserialize message field [bagPath]
    data.bagPath = _deserializer.string(buffer, bufferOffset);
    // Deserialize message field [srcDir]
    data.srcDir = _deserializer.string(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += _getByteLength(object.cfgDir);
    length += _getByteLength(object.dstDir);
    length += _getByteLength(object.bagPath);
    length += _getByteLength(object.srcDir);
    return length + 44;
  }

  static datatype() {
    // Returns string type for a service object
    return 'comm_srvs/MappingStartRequest';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '6c476bc57cf87c5acb54461af13deabb';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    int32   mode
    int32   state
    int32   frontEndMethod
    float32 currFrameLeafSize
    float32 localMapLeafSize  
    int32   localMapWindowSize 
    float32 loopDetectDist 
    string  cfgDir
    string  dstDir
    string  bagPath
    string  srcDir
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new MappingStartRequest(null);
    if (msg.mode !== undefined) {
      resolved.mode = msg.mode;
    }
    else {
      resolved.mode = 0
    }

    if (msg.state !== undefined) {
      resolved.state = msg.state;
    }
    else {
      resolved.state = 0
    }

    if (msg.frontEndMethod !== undefined) {
      resolved.frontEndMethod = msg.frontEndMethod;
    }
    else {
      resolved.frontEndMethod = 0
    }

    if (msg.currFrameLeafSize !== undefined) {
      resolved.currFrameLeafSize = msg.currFrameLeafSize;
    }
    else {
      resolved.currFrameLeafSize = 0.0
    }

    if (msg.localMapLeafSize !== undefined) {
      resolved.localMapLeafSize = msg.localMapLeafSize;
    }
    else {
      resolved.localMapLeafSize = 0.0
    }

    if (msg.localMapWindowSize !== undefined) {
      resolved.localMapWindowSize = msg.localMapWindowSize;
    }
    else {
      resolved.localMapWindowSize = 0
    }

    if (msg.loopDetectDist !== undefined) {
      resolved.loopDetectDist = msg.loopDetectDist;
    }
    else {
      resolved.loopDetectDist = 0.0
    }

    if (msg.cfgDir !== undefined) {
      resolved.cfgDir = msg.cfgDir;
    }
    else {
      resolved.cfgDir = ''
    }

    if (msg.dstDir !== undefined) {
      resolved.dstDir = msg.dstDir;
    }
    else {
      resolved.dstDir = ''
    }

    if (msg.bagPath !== undefined) {
      resolved.bagPath = msg.bagPath;
    }
    else {
      resolved.bagPath = ''
    }

    if (msg.srcDir !== undefined) {
      resolved.srcDir = msg.srcDir;
    }
    else {
      resolved.srcDir = ''
    }

    return resolved;
    }
};

class MappingStartResponse {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.success = null;
      this.message = null;
    }
    else {
      if (initObj.hasOwnProperty('success')) {
        this.success = initObj.success
      }
      else {
        this.success = false;
      }
      if (initObj.hasOwnProperty('message')) {
        this.message = initObj.message
      }
      else {
        this.message = '';
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type MappingStartResponse
    // Serialize message field [success]
    bufferOffset = _serializer.bool(obj.success, buffer, bufferOffset);
    // Serialize message field [message]
    bufferOffset = _serializer.string(obj.message, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type MappingStartResponse
    let len;
    let data = new MappingStartResponse(null);
    // Deserialize message field [success]
    data.success = _deserializer.bool(buffer, bufferOffset);
    // Deserialize message field [message]
    data.message = _deserializer.string(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += _getByteLength(object.message);
    return length + 5;
  }

  static datatype() {
    // Returns string type for a service object
    return 'comm_srvs/MappingStartResponse';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '937c9679a518e3a18d831e57125ea522';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    bool   success
    string message
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new MappingStartResponse(null);
    if (msg.success !== undefined) {
      resolved.success = msg.success;
    }
    else {
      resolved.success = false
    }

    if (msg.message !== undefined) {
      resolved.message = msg.message;
    }
    else {
      resolved.message = ''
    }

    return resolved;
    }
};

module.exports = {
  Request: MappingStartRequest,
  Response: MappingStartResponse,
  md5sum() { return '50d318bcef824d9f76f338ae7c891086'; },
  datatype() { return 'comm_srvs/MappingStart'; }
};
