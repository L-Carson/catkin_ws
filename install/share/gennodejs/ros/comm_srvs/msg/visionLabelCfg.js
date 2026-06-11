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

class visionLabelCfg {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.vectDetLabel = null;
      this.vectSegLabel = null;
      this.isSupportThickPipe = null;
      this.thickPipeWidth = null;
    }
    else {
      if (initObj.hasOwnProperty('vectDetLabel')) {
        this.vectDetLabel = initObj.vectDetLabel
      }
      else {
        this.vectDetLabel = [];
      }
      if (initObj.hasOwnProperty('vectSegLabel')) {
        this.vectSegLabel = initObj.vectSegLabel
      }
      else {
        this.vectSegLabel = [];
      }
      if (initObj.hasOwnProperty('isSupportThickPipe')) {
        this.isSupportThickPipe = initObj.isSupportThickPipe
      }
      else {
        this.isSupportThickPipe = false;
      }
      if (initObj.hasOwnProperty('thickPipeWidth')) {
        this.thickPipeWidth = initObj.thickPipeWidth
      }
      else {
        this.thickPipeWidth = 0.0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type visionLabelCfg
    // Serialize message field [vectDetLabel]
    bufferOffset = _arraySerializer.uint32(obj.vectDetLabel, buffer, bufferOffset, null);
    // Serialize message field [vectSegLabel]
    bufferOffset = _arraySerializer.uint32(obj.vectSegLabel, buffer, bufferOffset, null);
    // Serialize message field [isSupportThickPipe]
    bufferOffset = _serializer.bool(obj.isSupportThickPipe, buffer, bufferOffset);
    // Serialize message field [thickPipeWidth]
    bufferOffset = _serializer.float32(obj.thickPipeWidth, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type visionLabelCfg
    let len;
    let data = new visionLabelCfg(null);
    // Deserialize message field [vectDetLabel]
    data.vectDetLabel = _arrayDeserializer.uint32(buffer, bufferOffset, null)
    // Deserialize message field [vectSegLabel]
    data.vectSegLabel = _arrayDeserializer.uint32(buffer, bufferOffset, null)
    // Deserialize message field [isSupportThickPipe]
    data.isSupportThickPipe = _deserializer.bool(buffer, bufferOffset);
    // Deserialize message field [thickPipeWidth]
    data.thickPipeWidth = _deserializer.float32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += 4 * object.vectDetLabel.length;
    length += 4 * object.vectSegLabel.length;
    return length + 13;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_srvs/visionLabelCfg';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'c57896cd544874b34ea95938d00716d9';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
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
    const resolved = new visionLabelCfg(null);
    if (msg.vectDetLabel !== undefined) {
      resolved.vectDetLabel = msg.vectDetLabel;
    }
    else {
      resolved.vectDetLabel = []
    }

    if (msg.vectSegLabel !== undefined) {
      resolved.vectSegLabel = msg.vectSegLabel;
    }
    else {
      resolved.vectSegLabel = []
    }

    if (msg.isSupportThickPipe !== undefined) {
      resolved.isSupportThickPipe = msg.isSupportThickPipe;
    }
    else {
      resolved.isSupportThickPipe = false
    }

    if (msg.thickPipeWidth !== undefined) {
      resolved.thickPipeWidth = msg.thickPipeWidth;
    }
    else {
      resolved.thickPipeWidth = 0.0
    }

    return resolved;
    }
};

module.exports = visionLabelCfg;
