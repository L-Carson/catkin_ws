// Auto-generated. Do not edit!

// (in-package comm_msg.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let pose = require('./pose.js');
let coor3D = require('./coor3D.js');
let geometry_msgs = _finder('geometry_msgs');

//-----------------------------------------------------------

class equipment_info {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.status = null;
      this.equipmentType = null;
      this.QRCodeId = null;
      this.QRCodePose = null;
      this.QRCodeOffset = null;
      this.reflectiveStripPosLeft = null;
      this.reflectiveStripPosRight = null;
    }
    else {
      if (initObj.hasOwnProperty('status')) {
        this.status = initObj.status
      }
      else {
        this.status = 0;
      }
      if (initObj.hasOwnProperty('equipmentType')) {
        this.equipmentType = initObj.equipmentType
      }
      else {
        this.equipmentType = 0;
      }
      if (initObj.hasOwnProperty('QRCodeId')) {
        this.QRCodeId = initObj.QRCodeId
      }
      else {
        this.QRCodeId = 0;
      }
      if (initObj.hasOwnProperty('QRCodePose')) {
        this.QRCodePose = initObj.QRCodePose
      }
      else {
        this.QRCodePose = new pose();
      }
      if (initObj.hasOwnProperty('QRCodeOffset')) {
        this.QRCodeOffset = initObj.QRCodeOffset
      }
      else {
        this.QRCodeOffset = new coor3D();
      }
      if (initObj.hasOwnProperty('reflectiveStripPosLeft')) {
        this.reflectiveStripPosLeft = initObj.reflectiveStripPosLeft
      }
      else {
        this.reflectiveStripPosLeft = [];
      }
      if (initObj.hasOwnProperty('reflectiveStripPosRight')) {
        this.reflectiveStripPosRight = initObj.reflectiveStripPosRight
      }
      else {
        this.reflectiveStripPosRight = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type equipment_info
    // Serialize message field [status]
    bufferOffset = _serializer.uint8(obj.status, buffer, bufferOffset);
    // Serialize message field [equipmentType]
    bufferOffset = _serializer.uint8(obj.equipmentType, buffer, bufferOffset);
    // Serialize message field [QRCodeId]
    bufferOffset = _serializer.uint32(obj.QRCodeId, buffer, bufferOffset);
    // Serialize message field [QRCodePose]
    bufferOffset = pose.serialize(obj.QRCodePose, buffer, bufferOffset);
    // Serialize message field [QRCodeOffset]
    bufferOffset = coor3D.serialize(obj.QRCodeOffset, buffer, bufferOffset);
    // Serialize message field [reflectiveStripPosLeft]
    // Serialize the length for message field [reflectiveStripPosLeft]
    bufferOffset = _serializer.uint32(obj.reflectiveStripPosLeft.length, buffer, bufferOffset);
    obj.reflectiveStripPosLeft.forEach((val) => {
      bufferOffset = geometry_msgs.msg.Point32.serialize(val, buffer, bufferOffset);
    });
    // Serialize message field [reflectiveStripPosRight]
    // Serialize the length for message field [reflectiveStripPosRight]
    bufferOffset = _serializer.uint32(obj.reflectiveStripPosRight.length, buffer, bufferOffset);
    obj.reflectiveStripPosRight.forEach((val) => {
      bufferOffset = geometry_msgs.msg.Point32.serialize(val, buffer, bufferOffset);
    });
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type equipment_info
    let len;
    let data = new equipment_info(null);
    // Deserialize message field [status]
    data.status = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [equipmentType]
    data.equipmentType = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [QRCodeId]
    data.QRCodeId = _deserializer.uint32(buffer, bufferOffset);
    // Deserialize message field [QRCodePose]
    data.QRCodePose = pose.deserialize(buffer, bufferOffset);
    // Deserialize message field [QRCodeOffset]
    data.QRCodeOffset = coor3D.deserialize(buffer, bufferOffset);
    // Deserialize message field [reflectiveStripPosLeft]
    // Deserialize array length for message field [reflectiveStripPosLeft]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.reflectiveStripPosLeft = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.reflectiveStripPosLeft[i] = geometry_msgs.msg.Point32.deserialize(buffer, bufferOffset)
    }
    // Deserialize message field [reflectiveStripPosRight]
    // Deserialize array length for message field [reflectiveStripPosRight]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.reflectiveStripPosRight = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.reflectiveStripPosRight[i] = geometry_msgs.msg.Point32.deserialize(buffer, bufferOffset)
    }
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += 12 * object.reflectiveStripPosLeft.length;
    length += 12 * object.reflectiveStripPosRight.length;
    return length + 50;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/equipment_info';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'b8804889ce9395f076bb976f79f9f21b';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    uint8                   status
    
    # 0是二维码，1是二维码加反光条
    uint8                   equipmentType
    
    uint32                  QRCodeId
    pose                    QRCodePose
    coor3D                  QRCodeOffset
    geometry_msgs/Point32[] reflectiveStripPosLeft
    geometry_msgs/Point32[] reflectiveStripPosRight
    ================================================================================
    MSG: comm_msg/pose
    float32 px
    float32 py
    float32 pz
    float32 roll
    float32 pitch
    float32 yaw
    ================================================================================
    MSG: comm_msg/coor3D
    float32 pos_x
    float32 pos_y
    float32 pos_z
    ================================================================================
    MSG: geometry_msgs/Point32
    # This contains the position of a point in free space(with 32 bits of precision).
    # It is recommeded to use Point wherever possible instead of Point32.  
    # 
    # This recommendation is to promote interoperability.  
    #
    # This message is designed to take up less space when sending
    # lots of points at once, as in the case of a PointCloud.  
    
    float32 x
    float32 y
    float32 z
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new equipment_info(null);
    if (msg.status !== undefined) {
      resolved.status = msg.status;
    }
    else {
      resolved.status = 0
    }

    if (msg.equipmentType !== undefined) {
      resolved.equipmentType = msg.equipmentType;
    }
    else {
      resolved.equipmentType = 0
    }

    if (msg.QRCodeId !== undefined) {
      resolved.QRCodeId = msg.QRCodeId;
    }
    else {
      resolved.QRCodeId = 0
    }

    if (msg.QRCodePose !== undefined) {
      resolved.QRCodePose = pose.Resolve(msg.QRCodePose)
    }
    else {
      resolved.QRCodePose = new pose()
    }

    if (msg.QRCodeOffset !== undefined) {
      resolved.QRCodeOffset = coor3D.Resolve(msg.QRCodeOffset)
    }
    else {
      resolved.QRCodeOffset = new coor3D()
    }

    if (msg.reflectiveStripPosLeft !== undefined) {
      resolved.reflectiveStripPosLeft = new Array(msg.reflectiveStripPosLeft.length);
      for (let i = 0; i < resolved.reflectiveStripPosLeft.length; ++i) {
        resolved.reflectiveStripPosLeft[i] = geometry_msgs.msg.Point32.Resolve(msg.reflectiveStripPosLeft[i]);
      }
    }
    else {
      resolved.reflectiveStripPosLeft = []
    }

    if (msg.reflectiveStripPosRight !== undefined) {
      resolved.reflectiveStripPosRight = new Array(msg.reflectiveStripPosRight.length);
      for (let i = 0; i < resolved.reflectiveStripPosRight.length; ++i) {
        resolved.reflectiveStripPosRight[i] = geometry_msgs.msg.Point32.Resolve(msg.reflectiveStripPosRight[i]);
      }
    }
    else {
      resolved.reflectiveStripPosRight = []
    }

    return resolved;
    }
};

module.exports = equipment_info;
