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
let coor2D = require('./coor2D.js');
let std_msgs = _finder('std_msgs');

//-----------------------------------------------------------

class coor2DArrayL {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.header = null;
      this.label = null;
      this.id = null;
      this.coor2DArray = null;
    }
    else {
      if (initObj.hasOwnProperty('header')) {
        this.header = initObj.header
      }
      else {
        this.header = new std_msgs.msg.Header();
      }
      if (initObj.hasOwnProperty('label')) {
        this.label = initObj.label
      }
      else {
        this.label = new label();
      }
      if (initObj.hasOwnProperty('id')) {
        this.id = initObj.id
      }
      else {
        this.id = 0;
      }
      if (initObj.hasOwnProperty('coor2DArray')) {
        this.coor2DArray = initObj.coor2DArray
      }
      else {
        this.coor2DArray = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type coor2DArrayL
    // Serialize message field [header]
    bufferOffset = std_msgs.msg.Header.serialize(obj.header, buffer, bufferOffset);
    // Serialize message field [label]
    bufferOffset = label.serialize(obj.label, buffer, bufferOffset);
    // Serialize message field [id]
    bufferOffset = _serializer.uint8(obj.id, buffer, bufferOffset);
    // Serialize message field [coor2DArray]
    // Serialize the length for message field [coor2DArray]
    bufferOffset = _serializer.uint32(obj.coor2DArray.length, buffer, bufferOffset);
    obj.coor2DArray.forEach((val) => {
      bufferOffset = coor2D.serialize(val, buffer, bufferOffset);
    });
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type coor2DArrayL
    let len;
    let data = new coor2DArrayL(null);
    // Deserialize message field [header]
    data.header = std_msgs.msg.Header.deserialize(buffer, bufferOffset);
    // Deserialize message field [label]
    data.label = label.deserialize(buffer, bufferOffset);
    // Deserialize message field [id]
    data.id = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [coor2DArray]
    // Deserialize array length for message field [coor2DArray]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.coor2DArray = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.coor2DArray[i] = coor2D.deserialize(buffer, bufferOffset)
    }
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += std_msgs.msg.Header.getMessageSize(object.header);
    length += 9 * object.coor2DArray.length;
    return length + 6;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/coor2DArrayL';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '729ee7f33a4f91facef2ac82db7169b5';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    Header      header
    label       label
    uint8       id
    coor2D[]    coor2DArray
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
    ================================================================================
    MSG: comm_msg/coor2D
    coorLabel   label       #坐标类别
    float32     pos_x
    float32     pos_y
    ================================================================================
    MSG: comm_msg/coorLabel
    uint8   UNKNOWN      = 0  #未知
    uint8   STATIC       = 1  #静态
    uint8   DYNAMIC      = 2  #动态
    uint8   HOLLOW_OBST  = 3  #空洞障碍
    uint8   SHORT_HOLD   = 4  #低矮维持
    uint8   COMMON_HOLD  = 5  #常规维持
    uint8   BLIND_SPOT   = 6  #盲区障碍
    uint8   SLOPE_PAD    = 7  #斜坡垫
    uint8   CONTACT_EDGE = 8  #触边障碍
    uint8   TREELAWN     = 9  #绿植障碍
    uint8   ULTRASONIC   = 10 #超声
    
    uint8 value
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new coor2DArrayL(null);
    if (msg.header !== undefined) {
      resolved.header = std_msgs.msg.Header.Resolve(msg.header)
    }
    else {
      resolved.header = new std_msgs.msg.Header()
    }

    if (msg.label !== undefined) {
      resolved.label = label.Resolve(msg.label)
    }
    else {
      resolved.label = new label()
    }

    if (msg.id !== undefined) {
      resolved.id = msg.id;
    }
    else {
      resolved.id = 0
    }

    if (msg.coor2DArray !== undefined) {
      resolved.coor2DArray = new Array(msg.coor2DArray.length);
      for (let i = 0; i < resolved.coor2DArray.length; ++i) {
        resolved.coor2DArray[i] = coor2D.Resolve(msg.coor2DArray[i]);
      }
    }
    else {
      resolved.coor2DArray = []
    }

    return resolved;
    }
};

module.exports = coor2DArrayL;
