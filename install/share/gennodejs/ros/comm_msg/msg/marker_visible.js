// Auto-generated. Do not edit!

// (in-package comm_msg.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let geometry_msgs = _finder('geometry_msgs');

//-----------------------------------------------------------

class marker_visible {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.point_P = null;
      this.point_V = null;
      this.velocity = null;
    }
    else {
      if (initObj.hasOwnProperty('point_P')) {
        this.point_P = initObj.point_P
      }
      else {
        this.point_P = [];
      }
      if (initObj.hasOwnProperty('point_V')) {
        this.point_V = initObj.point_V
      }
      else {
        this.point_V = [];
      }
      if (initObj.hasOwnProperty('velocity')) {
        this.velocity = initObj.velocity
      }
      else {
        this.velocity = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type marker_visible
    // Serialize message field [point_P]
    // Serialize the length for message field [point_P]
    bufferOffset = _serializer.uint32(obj.point_P.length, buffer, bufferOffset);
    obj.point_P.forEach((val) => {
      bufferOffset = geometry_msgs.msg.Point.serialize(val, buffer, bufferOffset);
    });
    // Serialize message field [point_V]
    // Serialize the length for message field [point_V]
    bufferOffset = _serializer.uint32(obj.point_V.length, buffer, bufferOffset);
    obj.point_V.forEach((val) => {
      bufferOffset = geometry_msgs.msg.Point.serialize(val, buffer, bufferOffset);
    });
    // Serialize message field [velocity]
    bufferOffset = _arraySerializer.float32(obj.velocity, buffer, bufferOffset, null);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type marker_visible
    let len;
    let data = new marker_visible(null);
    // Deserialize message field [point_P]
    // Deserialize array length for message field [point_P]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.point_P = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.point_P[i] = geometry_msgs.msg.Point.deserialize(buffer, bufferOffset)
    }
    // Deserialize message field [point_V]
    // Deserialize array length for message field [point_V]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.point_V = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.point_V[i] = geometry_msgs.msg.Point.deserialize(buffer, bufferOffset)
    }
    // Deserialize message field [velocity]
    data.velocity = _arrayDeserializer.float32(buffer, bufferOffset, null)
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += 24 * object.point_P.length;
    length += 24 * object.point_V.length;
    length += 4 * object.velocity.length;
    return length + 12;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/marker_visible';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '8c091b688cfba7fce69c6312f128ed30';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    geometry_msgs/Point[] 	point_P
    geometry_msgs/Point[] 	point_V
    
    float32[]               velocity
    
    ================================================================================
    MSG: geometry_msgs/Point
    # This contains the position of a point in free space
    float64 x
    float64 y
    float64 z
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new marker_visible(null);
    if (msg.point_P !== undefined) {
      resolved.point_P = new Array(msg.point_P.length);
      for (let i = 0; i < resolved.point_P.length; ++i) {
        resolved.point_P[i] = geometry_msgs.msg.Point.Resolve(msg.point_P[i]);
      }
    }
    else {
      resolved.point_P = []
    }

    if (msg.point_V !== undefined) {
      resolved.point_V = new Array(msg.point_V.length);
      for (let i = 0; i < resolved.point_V.length; ++i) {
        resolved.point_V[i] = geometry_msgs.msg.Point.Resolve(msg.point_V[i]);
      }
    }
    else {
      resolved.point_V = []
    }

    if (msg.velocity !== undefined) {
      resolved.velocity = msg.velocity;
    }
    else {
      resolved.velocity = []
    }

    return resolved;
    }
};

module.exports = marker_visible;
