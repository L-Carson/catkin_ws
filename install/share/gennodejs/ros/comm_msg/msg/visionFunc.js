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

class visionFunc {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.funcId = null;
      this.ipu = null;
      this.hz = null;
      this.cameras = null;
      this.usingLabel = null;
    }
    else {
      if (initObj.hasOwnProperty('funcId')) {
        this.funcId = initObj.funcId
      }
      else {
        this.funcId = 0;
      }
      if (initObj.hasOwnProperty('ipu')) {
        this.ipu = initObj.ipu
      }
      else {
        this.ipu = false;
      }
      if (initObj.hasOwnProperty('hz')) {
        this.hz = initObj.hz
      }
      else {
        this.hz = 0;
      }
      if (initObj.hasOwnProperty('cameras')) {
        this.cameras = initObj.cameras
      }
      else {
        this.cameras = [];
      }
      if (initObj.hasOwnProperty('usingLabel')) {
        this.usingLabel = initObj.usingLabel
      }
      else {
        this.usingLabel = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type visionFunc
    // Serialize message field [funcId]
    bufferOffset = _serializer.uint8(obj.funcId, buffer, bufferOffset);
    // Serialize message field [ipu]
    bufferOffset = _serializer.bool(obj.ipu, buffer, bufferOffset);
    // Serialize message field [hz]
    bufferOffset = _serializer.uint8(obj.hz, buffer, bufferOffset);
    // Serialize message field [cameras]
    bufferOffset = _arraySerializer.string(obj.cameras, buffer, bufferOffset, null);
    // Serialize message field [usingLabel]
    // Serialize the length for message field [usingLabel]
    bufferOffset = _serializer.uint32(obj.usingLabel.length, buffer, bufferOffset);
    obj.usingLabel.forEach((val) => {
      bufferOffset = label.serialize(val, buffer, bufferOffset);
    });
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type visionFunc
    let len;
    let data = new visionFunc(null);
    // Deserialize message field [funcId]
    data.funcId = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [ipu]
    data.ipu = _deserializer.bool(buffer, bufferOffset);
    // Deserialize message field [hz]
    data.hz = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [cameras]
    data.cameras = _arrayDeserializer.string(buffer, bufferOffset, null)
    // Deserialize message field [usingLabel]
    // Deserialize array length for message field [usingLabel]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.usingLabel = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.usingLabel[i] = label.deserialize(buffer, bufferOffset)
    }
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    object.cameras.forEach((val) => {
      length += 4 + _getByteLength(val);
    });
    length += object.usingLabel.length;
    return length + 11;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/visionFunc';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '79b818443e89e9f0f5bd6b426111152a';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    # 功能ID
    uint8 NULL_FUNC   = 0   # NULL
    uint8 QR_CODE_DET = 1   # 二维码检测
    uint8 NET_DET     = 2   # 网络检测
    uint8 NET_SEG     = 3   # 网络分割
    
    # msg
    uint8       funcId      # 功能ID
    bool        ipu         # 是否使用图像处理单元
    uint8       hz          # 执行频率
    string[]    cameras     # 相机ID：front_left(前左), front_mid(前中), front_right(前右), rear_mid(后中) etc.
    label[]     usingLabel  # 当前正在使用的类别标签
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
    const resolved = new visionFunc(null);
    if (msg.funcId !== undefined) {
      resolved.funcId = msg.funcId;
    }
    else {
      resolved.funcId = 0
    }

    if (msg.ipu !== undefined) {
      resolved.ipu = msg.ipu;
    }
    else {
      resolved.ipu = false
    }

    if (msg.hz !== undefined) {
      resolved.hz = msg.hz;
    }
    else {
      resolved.hz = 0
    }

    if (msg.cameras !== undefined) {
      resolved.cameras = msg.cameras;
    }
    else {
      resolved.cameras = []
    }

    if (msg.usingLabel !== undefined) {
      resolved.usingLabel = new Array(msg.usingLabel.length);
      for (let i = 0; i < resolved.usingLabel.length; ++i) {
        resolved.usingLabel[i] = label.Resolve(msg.usingLabel[i]);
      }
    }
    else {
      resolved.usingLabel = []
    }

    return resolved;
    }
};

// Constants for message
visionFunc.Constants = {
  NULL_FUNC: 0,
  QR_CODE_DET: 1,
  NET_DET: 2,
  NET_SEG: 3,
}

module.exports = visionFunc;
