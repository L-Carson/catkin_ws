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

let visionFuncCfg = require('../msg/visionFuncCfg.js');
let visionLabelCfg = require('../msg/visionLabelCfg.js');

//-----------------------------------------------------------

class visionFuncRequest {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
    }
    else {
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type visionFuncRequest
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type visionFuncRequest
    let len;
    let data = new visionFuncRequest(null);
    return data;
  }

  static getMessageSize(object) {
    return 0;
  }

  static datatype() {
    // Returns string type for a service object
    return 'comm_srvs/visionFuncRequest';
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
    const resolved = new visionFuncRequest(null);
    return resolved;
    }
};

class visionFuncResponse {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.success = null;
      this.isUseApi = null;
      this.visionFuncCfgArray = null;
      this.visionLabelCfg = null;
    }
    else {
      if (initObj.hasOwnProperty('success')) {
        this.success = initObj.success
      }
      else {
        this.success = false;
      }
      if (initObj.hasOwnProperty('isUseApi')) {
        this.isUseApi = initObj.isUseApi
      }
      else {
        this.isUseApi = false;
      }
      if (initObj.hasOwnProperty('visionFuncCfgArray')) {
        this.visionFuncCfgArray = initObj.visionFuncCfgArray
      }
      else {
        this.visionFuncCfgArray = [];
      }
      if (initObj.hasOwnProperty('visionLabelCfg')) {
        this.visionLabelCfg = initObj.visionLabelCfg
      }
      else {
        this.visionLabelCfg = new visionLabelCfg();
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type visionFuncResponse
    // Serialize message field [success]
    bufferOffset = _serializer.bool(obj.success, buffer, bufferOffset);
    // Serialize message field [isUseApi]
    bufferOffset = _serializer.bool(obj.isUseApi, buffer, bufferOffset);
    // Serialize message field [visionFuncCfgArray]
    // Serialize the length for message field [visionFuncCfgArray]
    bufferOffset = _serializer.uint32(obj.visionFuncCfgArray.length, buffer, bufferOffset);
    obj.visionFuncCfgArray.forEach((val) => {
      bufferOffset = visionFuncCfg.serialize(val, buffer, bufferOffset);
    });
    // Serialize message field [visionLabelCfg]
    bufferOffset = visionLabelCfg.serialize(obj.visionLabelCfg, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type visionFuncResponse
    let len;
    let data = new visionFuncResponse(null);
    // Deserialize message field [success]
    data.success = _deserializer.bool(buffer, bufferOffset);
    // Deserialize message field [isUseApi]
    data.isUseApi = _deserializer.bool(buffer, bufferOffset);
    // Deserialize message field [visionFuncCfgArray]
    // Deserialize array length for message field [visionFuncCfgArray]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.visionFuncCfgArray = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.visionFuncCfgArray[i] = visionFuncCfg.deserialize(buffer, bufferOffset)
    }
    // Deserialize message field [visionLabelCfg]
    data.visionLabelCfg = visionLabelCfg.deserialize(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += 3 * object.visionFuncCfgArray.length;
    length += visionLabelCfg.getMessageSize(object.visionLabelCfg);
    return length + 6;
  }

  static datatype() {
    // Returns string type for a service object
    return 'comm_srvs/visionFuncResponse';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'adf5e73fc1530eec7e82d92a5cfadc9f';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    bool                success
    bool                isUseApi
    visionFuncCfg[]     visionFuncCfgArray
    visionLabelCfg      visionLabelCfg
    
    ================================================================================
    MSG: comm_srvs/visionFuncCfg
    uint8   funcId      # 功能ID
    bool    ipu         # 是否使用图像处理单元
    uint8   hz          # 执行频率
    ================================================================================
    MSG: comm_srvs/visionLabelCfg
    uint32[] vectDetLabel
    uint32[] vectSegLabel
    
    bool    isSupportThickPipe #是否支持粗水管类别
    float32 thickPipeWidth     #粗水管宽度阈值
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new visionFuncResponse(null);
    if (msg.success !== undefined) {
      resolved.success = msg.success;
    }
    else {
      resolved.success = false
    }

    if (msg.isUseApi !== undefined) {
      resolved.isUseApi = msg.isUseApi;
    }
    else {
      resolved.isUseApi = false
    }

    if (msg.visionFuncCfgArray !== undefined) {
      resolved.visionFuncCfgArray = new Array(msg.visionFuncCfgArray.length);
      for (let i = 0; i < resolved.visionFuncCfgArray.length; ++i) {
        resolved.visionFuncCfgArray[i] = visionFuncCfg.Resolve(msg.visionFuncCfgArray[i]);
      }
    }
    else {
      resolved.visionFuncCfgArray = []
    }

    if (msg.visionLabelCfg !== undefined) {
      resolved.visionLabelCfg = visionLabelCfg.Resolve(msg.visionLabelCfg)
    }
    else {
      resolved.visionLabelCfg = new visionLabelCfg()
    }

    return resolved;
    }
};

module.exports = {
  Request: visionFuncRequest,
  Response: visionFuncResponse,
  md5sum() { return 'adf5e73fc1530eec7e82d92a5cfadc9f'; },
  datatype() { return 'comm_srvs/visionFunc'; }
};
