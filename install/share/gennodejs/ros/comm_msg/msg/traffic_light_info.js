// Auto-generated. Do not edit!

// (in-package comm_msg.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class traffic_light_info {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.type = null;
      this.pos_x = null;
      this.pos_y = null;
      this.pos_z = null;
      this.row_length = null;
      this.col_length = null;
    }
    else {
      if (initObj.hasOwnProperty('type')) {
        this.type = initObj.type
      }
      else {
        this.type = 0;
      }
      if (initObj.hasOwnProperty('pos_x')) {
        this.pos_x = initObj.pos_x
      }
      else {
        this.pos_x = 0.0;
      }
      if (initObj.hasOwnProperty('pos_y')) {
        this.pos_y = initObj.pos_y
      }
      else {
        this.pos_y = 0.0;
      }
      if (initObj.hasOwnProperty('pos_z')) {
        this.pos_z = initObj.pos_z
      }
      else {
        this.pos_z = 0.0;
      }
      if (initObj.hasOwnProperty('row_length')) {
        this.row_length = initObj.row_length
      }
      else {
        this.row_length = 0.0;
      }
      if (initObj.hasOwnProperty('col_length')) {
        this.col_length = initObj.col_length
      }
      else {
        this.col_length = 0.0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type traffic_light_info
    // Serialize message field [type]
    bufferOffset = _serializer.uint8(obj.type, buffer, bufferOffset);
    // Serialize message field [pos_x]
    bufferOffset = _serializer.float32(obj.pos_x, buffer, bufferOffset);
    // Serialize message field [pos_y]
    bufferOffset = _serializer.float32(obj.pos_y, buffer, bufferOffset);
    // Serialize message field [pos_z]
    bufferOffset = _serializer.float32(obj.pos_z, buffer, bufferOffset);
    // Serialize message field [row_length]
    bufferOffset = _serializer.float32(obj.row_length, buffer, bufferOffset);
    // Serialize message field [col_length]
    bufferOffset = _serializer.float32(obj.col_length, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type traffic_light_info
    let len;
    let data = new traffic_light_info(null);
    // Deserialize message field [type]
    data.type = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [pos_x]
    data.pos_x = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [pos_y]
    data.pos_y = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [pos_z]
    data.pos_z = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [row_length]
    data.row_length = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [col_length]
    data.col_length = _deserializer.float32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 21;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/traffic_light_info';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'b4755f326c42173c8ecd115dcd1f1001';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    uint8    type           # 交通灯显示类型：1(读秒类型) 2(频闪类型) 3(高频亮灭型)
    float32  pos_x          # 交通灯中在地图上的坐标x
    float32  pos_y          # 交通灯中在地图上的坐标y
    float32  pos_z          # 交通灯中在地图上的坐标z
    float32  row_length     # 交通灯横向长度
    float32  col_length     # 交通灯纵向长度
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new traffic_light_info(null);
    if (msg.type !== undefined) {
      resolved.type = msg.type;
    }
    else {
      resolved.type = 0
    }

    if (msg.pos_x !== undefined) {
      resolved.pos_x = msg.pos_x;
    }
    else {
      resolved.pos_x = 0.0
    }

    if (msg.pos_y !== undefined) {
      resolved.pos_y = msg.pos_y;
    }
    else {
      resolved.pos_y = 0.0
    }

    if (msg.pos_z !== undefined) {
      resolved.pos_z = msg.pos_z;
    }
    else {
      resolved.pos_z = 0.0
    }

    if (msg.row_length !== undefined) {
      resolved.row_length = msg.row_length;
    }
    else {
      resolved.row_length = 0.0
    }

    if (msg.col_length !== undefined) {
      resolved.col_length = msg.col_length;
    }
    else {
      resolved.col_length = 0.0
    }

    return resolved;
    }
};

module.exports = traffic_light_info;
