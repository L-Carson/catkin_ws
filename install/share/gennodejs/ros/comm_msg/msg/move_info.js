// Auto-generated. Do not edit!

// (in-package comm_msg.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let std_msgs = _finder('std_msgs');

//-----------------------------------------------------------

class move_info {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.header = null;
      this.steeringAngle = null;
      this.moveLinearVel = null;
      this.yawAngle = null;
      this.yawAngleVel = null;
      this.accX = null;
      this.accY = null;
      this.accZ = null;
    }
    else {
      if (initObj.hasOwnProperty('header')) {
        this.header = initObj.header
      }
      else {
        this.header = new std_msgs.msg.Header();
      }
      if (initObj.hasOwnProperty('steeringAngle')) {
        this.steeringAngle = initObj.steeringAngle
      }
      else {
        this.steeringAngle = 0.0;
      }
      if (initObj.hasOwnProperty('moveLinearVel')) {
        this.moveLinearVel = initObj.moveLinearVel
      }
      else {
        this.moveLinearVel = 0.0;
      }
      if (initObj.hasOwnProperty('yawAngle')) {
        this.yawAngle = initObj.yawAngle
      }
      else {
        this.yawAngle = 0.0;
      }
      if (initObj.hasOwnProperty('yawAngleVel')) {
        this.yawAngleVel = initObj.yawAngleVel
      }
      else {
        this.yawAngleVel = 0.0;
      }
      if (initObj.hasOwnProperty('accX')) {
        this.accX = initObj.accX
      }
      else {
        this.accX = 0.0;
      }
      if (initObj.hasOwnProperty('accY')) {
        this.accY = initObj.accY
      }
      else {
        this.accY = 0.0;
      }
      if (initObj.hasOwnProperty('accZ')) {
        this.accZ = initObj.accZ
      }
      else {
        this.accZ = 0.0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type move_info
    // Serialize message field [header]
    bufferOffset = std_msgs.msg.Header.serialize(obj.header, buffer, bufferOffset);
    // Serialize message field [steeringAngle]
    bufferOffset = _serializer.float32(obj.steeringAngle, buffer, bufferOffset);
    // Serialize message field [moveLinearVel]
    bufferOffset = _serializer.float32(obj.moveLinearVel, buffer, bufferOffset);
    // Serialize message field [yawAngle]
    bufferOffset = _serializer.float32(obj.yawAngle, buffer, bufferOffset);
    // Serialize message field [yawAngleVel]
    bufferOffset = _serializer.float32(obj.yawAngleVel, buffer, bufferOffset);
    // Serialize message field [accX]
    bufferOffset = _serializer.float32(obj.accX, buffer, bufferOffset);
    // Serialize message field [accY]
    bufferOffset = _serializer.float32(obj.accY, buffer, bufferOffset);
    // Serialize message field [accZ]
    bufferOffset = _serializer.float32(obj.accZ, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type move_info
    let len;
    let data = new move_info(null);
    // Deserialize message field [header]
    data.header = std_msgs.msg.Header.deserialize(buffer, bufferOffset);
    // Deserialize message field [steeringAngle]
    data.steeringAngle = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [moveLinearVel]
    data.moveLinearVel = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [yawAngle]
    data.yawAngle = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [yawAngleVel]
    data.yawAngleVel = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [accX]
    data.accX = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [accY]
    data.accY = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [accZ]
    data.accZ = _deserializer.float32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += std_msgs.msg.Header.getMessageSize(object.header);
    return length + 28;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/move_info';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '5651dd8f0d48de47c0d8061b96a98beb';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    Header  header
    
    float32 steeringAngle
    float32 moveLinearVel
    float32 yawAngle
    float32 yawAngleVel
    float32 accX
    float32 accY
    float32 accZ
    ================================================================================
    MSG: std_msgs/Header
    # Standard metadata for higher-level stamped data types.
    # This is generally used to communicate timestamped data 
    # in a particular coordinate frame.
    # 
    # sequence ID: consecutively increasing ID 
    uint32 seq
    #Two-integer timestamp that is expressed as:
    # * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
    # * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
    # time-handling sugar is provided by the client library
    time stamp
    #Frame this data is associated with
    string frame_id
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new move_info(null);
    if (msg.header !== undefined) {
      resolved.header = std_msgs.msg.Header.Resolve(msg.header)
    }
    else {
      resolved.header = new std_msgs.msg.Header()
    }

    if (msg.steeringAngle !== undefined) {
      resolved.steeringAngle = msg.steeringAngle;
    }
    else {
      resolved.steeringAngle = 0.0
    }

    if (msg.moveLinearVel !== undefined) {
      resolved.moveLinearVel = msg.moveLinearVel;
    }
    else {
      resolved.moveLinearVel = 0.0
    }

    if (msg.yawAngle !== undefined) {
      resolved.yawAngle = msg.yawAngle;
    }
    else {
      resolved.yawAngle = 0.0
    }

    if (msg.yawAngleVel !== undefined) {
      resolved.yawAngleVel = msg.yawAngleVel;
    }
    else {
      resolved.yawAngleVel = 0.0
    }

    if (msg.accX !== undefined) {
      resolved.accX = msg.accX;
    }
    else {
      resolved.accX = 0.0
    }

    if (msg.accY !== undefined) {
      resolved.accY = msg.accY;
    }
    else {
      resolved.accY = 0.0
    }

    if (msg.accZ !== undefined) {
      resolved.accZ = msg.accZ;
    }
    else {
      resolved.accZ = 0.0
    }

    return resolved;
    }
};

module.exports = move_info;
