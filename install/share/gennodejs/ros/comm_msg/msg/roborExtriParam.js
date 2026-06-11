// Auto-generated. Do not edit!

// (in-package comm_msg.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let sensorExtriParam = require('./sensorExtriParam.js');

//-----------------------------------------------------------

class roborExtriParam {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.gnss = null;
      this.imu = null;
      this.lidar = null;
    }
    else {
      if (initObj.hasOwnProperty('gnss')) {
        this.gnss = initObj.gnss
      }
      else {
        this.gnss = new sensorExtriParam();
      }
      if (initObj.hasOwnProperty('imu')) {
        this.imu = initObj.imu
      }
      else {
        this.imu = new sensorExtriParam();
      }
      if (initObj.hasOwnProperty('lidar')) {
        this.lidar = initObj.lidar
      }
      else {
        this.lidar = new sensorExtriParam();
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type roborExtriParam
    // Serialize message field [gnss]
    bufferOffset = sensorExtriParam.serialize(obj.gnss, buffer, bufferOffset);
    // Serialize message field [imu]
    bufferOffset = sensorExtriParam.serialize(obj.imu, buffer, bufferOffset);
    // Serialize message field [lidar]
    bufferOffset = sensorExtriParam.serialize(obj.lidar, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type roborExtriParam
    let len;
    let data = new roborExtriParam(null);
    // Deserialize message field [gnss]
    data.gnss = sensorExtriParam.deserialize(buffer, bufferOffset);
    // Deserialize message field [imu]
    data.imu = sensorExtriParam.deserialize(buffer, bufferOffset);
    // Deserialize message field [lidar]
    data.lidar = sensorExtriParam.deserialize(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 75;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/roborExtriParam';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'ad756381b7e6a7c7a09b59fa09842fbd';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    sensorExtriParam gnss
    sensorExtriParam imu
    sensorExtriParam lidar
    ================================================================================
    MSG: comm_msg/sensorExtriParam
    bool isSupport 
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
    const resolved = new roborExtriParam(null);
    if (msg.gnss !== undefined) {
      resolved.gnss = sensorExtriParam.Resolve(msg.gnss)
    }
    else {
      resolved.gnss = new sensorExtriParam()
    }

    if (msg.imu !== undefined) {
      resolved.imu = sensorExtriParam.Resolve(msg.imu)
    }
    else {
      resolved.imu = new sensorExtriParam()
    }

    if (msg.lidar !== undefined) {
      resolved.lidar = sensorExtriParam.Resolve(msg.lidar)
    }
    else {
      resolved.lidar = new sensorExtriParam()
    }

    return resolved;
    }
};

module.exports = roborExtriParam;
