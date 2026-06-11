// Auto-generated. Do not edit!

// (in-package comm_msg.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let label = require('./label.js');

//-----------------------------------------------------------

class visionRect {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.label = null;
      this.score = null;
      this.xmin = null;
      this.ymin = null;
      this.xmax = null;
      this.ymax = null;
      this.feature = null;
    }
    else {
      if (initObj.hasOwnProperty('label')) {
        this.label = initObj.label
      }
      else {
        this.label = new label();
      }
      if (initObj.hasOwnProperty('score')) {
        this.score = initObj.score
      }
      else {
        this.score = 0.0;
      }
      if (initObj.hasOwnProperty('xmin')) {
        this.xmin = initObj.xmin
      }
      else {
        this.xmin = 0;
      }
      if (initObj.hasOwnProperty('ymin')) {
        this.ymin = initObj.ymin
      }
      else {
        this.ymin = 0;
      }
      if (initObj.hasOwnProperty('xmax')) {
        this.xmax = initObj.xmax
      }
      else {
        this.xmax = 0;
      }
      if (initObj.hasOwnProperty('ymax')) {
        this.ymax = initObj.ymax
      }
      else {
        this.ymax = 0;
      }
      if (initObj.hasOwnProperty('feature')) {
        this.feature = initObj.feature
      }
      else {
        this.feature = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type visionRect
    // Serialize message field [label]
    bufferOffset = label.serialize(obj.label, buffer, bufferOffset);
    // Serialize message field [score]
    bufferOffset = _serializer.float32(obj.score, buffer, bufferOffset);
    // Serialize message field [xmin]
    bufferOffset = _serializer.uint32(obj.xmin, buffer, bufferOffset);
    // Serialize message field [ymin]
    bufferOffset = _serializer.uint32(obj.ymin, buffer, bufferOffset);
    // Serialize message field [xmax]
    bufferOffset = _serializer.uint32(obj.xmax, buffer, bufferOffset);
    // Serialize message field [ymax]
    bufferOffset = _serializer.uint32(obj.ymax, buffer, bufferOffset);
    // Serialize message field [feature]
    bufferOffset = _arraySerializer.float32(obj.feature, buffer, bufferOffset, null);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type visionRect
    let len;
    let data = new visionRect(null);
    // Deserialize message field [label]
    data.label = label.deserialize(buffer, bufferOffset);
    // Deserialize message field [score]
    data.score = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [xmin]
    data.xmin = _deserializer.uint32(buffer, bufferOffset);
    // Deserialize message field [ymin]
    data.ymin = _deserializer.uint32(buffer, bufferOffset);
    // Deserialize message field [xmax]
    data.xmax = _deserializer.uint32(buffer, bufferOffset);
    // Deserialize message field [ymax]
    data.ymax = _deserializer.uint32(buffer, bufferOffset);
    // Deserialize message field [feature]
    data.feature = _arrayDeserializer.float32(buffer, bufferOffset, null)
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += 4 * object.feature.length;
    return length + 25;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/visionRect';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'b747118cb5066a7266881512a4bb9821';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    label       label
    float32     score
    uint32      xmin
    uint32      ymin
    uint32      xmax
    uint32      ymax
    float32[]   feature
    ================================================================================
    MSG: comm_msg/label
    #Note: 枚举类别、枚举值、枚举顺序均不可随意改动，不然md5不匹配，消息将无法正常解析
    
    uint8 UNKNOWN                   = 0     #未知（统一类别）
    uint8 PEDESTRIAN                = 1     #行人
    uint8 CYCLIST                   = 2     #骑自行车的人
    uint8 BICYCLE                   = 3     #自行车（两轮车）
    uint8 CAR                       = 4     #汽车
    uint8 TRUCK                     = 5     #卡车
    uint8 TRAM                      = 6     #有轨电车
    uint8 TRICYCLE                  = 7     #三轮车
    uint8 BUS                       = 8     #公交车
    
    uint8 LOWOBST                   = 10    #低矮障碍
    uint8 ROADEDGE                  = 11    #道路边缘
    uint8 PIPELINE                  = 12    #水管管线
    uint8 CORDON                    = 13    #警戒线
    uint8 TREELAWN                  = 14    #街道绿化带
    uint8 THICKPIPE                 = 15    #粗水管
    uint8 CORD                      = 16    #电线、缆绳
    uint8 SLOPE_PAD                 = 17    #斜坡垫
    uint8 SPEED_BUMP                = 18    #减速带
    
    uint8 COVER_SOLID               = 20    #实心井盖
    uint8 COVER_HOLLOW              = 21    #栅格井盖
    uint8 TRAFFIC_CONE              = 22    #雪糕筒（交通锥）
    uint8 METAL_BARRIER             = 23    #金属围栏（铁马）
    uint8 MEDIAN_BARRIER            = 24    #栅栏（道路中央隔离带）
    uint8 CEREMONIAL_POLE           = 25    #礼宾杆
    uint8 BOLLARD                   = 26    #路桩
    
    uint8 GARBAGE                   = 40    #垃圾（统称）
    uint8 GARBAGE_BOTTLE            = 41    #瓶子
    uint8 GARBAGE_PAPER             = 42    #纸巾
    uint8 GARBAGE_BRANCH            = 43    #树枝
    uint8 GARBAGE_LEAVES            = 44    #树叶
    uint8 GARBAGE_BOUGH             = 45    #粗树枝、长树枝
    uint8 GARBAGE_CUP               = 46    #奶茶杯
    uint8 GARBAGE_CAN               = 47    #易拉罐
    uint8 GARBAGE_CARTON            = 48    #纸盒
    uint8 GARBAGE_PACKAGE           = 49    #烟盒
    uint8 GARBAGE_BRICK             = 70    #砖块（小砖块）
    uint8 GARBAGE_CIGARETTEEND      = 71    #烟头
    
    
    uint8 TRAFFIC_LIGHT             = 50    #交通灯（统称）
    uint8 TRAFFIC_LIGHT_OFF         = 51    #交通灯-关闭
    uint8 TRAFFIC_LIGHT_RED         = 52    #交通灯-红灯
    uint8 TRAFFIC_LIGHT_GREEN       = 53    #交通灯-绿灯
    uint8 TRAFFIC_LIGHT_YELLOW      = 54    #交通灯-黄灯
    
    uint8 TRAFFIC_LIGHT_GROUP       = 55    #交通灯-灯组
    uint8 TRAFFIC_LIGHT_OFF_LITE    = 56    #交通灯-关闭（小灯）
    uint8 TRAFFIC_LIGHT_RED_LITE    = 57    #交通灯-红灯（小灯）
    uint8 TRAFFIC_LIGHT_GREEN_LITE  = 58    #交通灯-绿灯（小灯）
    uint8 TRAFFIC_LIGHT_YELLOW_LITE = 59    #交通灯-黄灯（小灯）
    
    uint8 CHARGING_GUN              = 60    #充电枪
    
    uint8 PLACEHOLDER               = 101    #占位符(MAX)
    
    uint8 value
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new visionRect(null);
    if (msg.label !== undefined) {
      resolved.label = label.Resolve(msg.label)
    }
    else {
      resolved.label = new label()
    }

    if (msg.score !== undefined) {
      resolved.score = msg.score;
    }
    else {
      resolved.score = 0.0
    }

    if (msg.xmin !== undefined) {
      resolved.xmin = msg.xmin;
    }
    else {
      resolved.xmin = 0
    }

    if (msg.ymin !== undefined) {
      resolved.ymin = msg.ymin;
    }
    else {
      resolved.ymin = 0
    }

    if (msg.xmax !== undefined) {
      resolved.xmax = msg.xmax;
    }
    else {
      resolved.xmax = 0
    }

    if (msg.ymax !== undefined) {
      resolved.ymax = msg.ymax;
    }
    else {
      resolved.ymax = 0
    }

    if (msg.feature !== undefined) {
      resolved.feature = msg.feature;
    }
    else {
      resolved.feature = []
    }

    return resolved;
    }
};

module.exports = visionRect;
