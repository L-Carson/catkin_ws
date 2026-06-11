; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude traffic_light_target.msg.html

(cl:defclass <traffic_light_target> (roslisp-msg-protocol:ros-message)
  ((is_open
    :reader is_open
    :initarg :is_open
    :type cl:fixnum
    :initform 0)
   (pixel_bias
    :reader pixel_bias
    :initarg :pixel_bias
    :type cl:float
    :initform 0.0)
   (rect_bias
    :reader rect_bias
    :initarg :rect_bias
    :type cl:float
    :initform 0.0)
   (target
    :reader target
    :initarg :target
    :type comm_msg-msg:traffic_light_info
    :initform (cl:make-instance 'comm_msg-msg:traffic_light_info))
   (nears
    :reader nears
    :initarg :nears
    :type (cl:vector comm_msg-msg:traffic_light_info)
   :initform (cl:make-array 0 :element-type 'comm_msg-msg:traffic_light_info :initial-element (cl:make-instance 'comm_msg-msg:traffic_light_info))))
)

(cl:defclass traffic_light_target (<traffic_light_target>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <traffic_light_target>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'traffic_light_target)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<traffic_light_target> is deprecated: use comm_msg-msg:traffic_light_target instead.")))

(cl:ensure-generic-function 'is_open-val :lambda-list '(m))
(cl:defmethod is_open-val ((m <traffic_light_target>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:is_open-val is deprecated.  Use comm_msg-msg:is_open instead.")
  (is_open m))

(cl:ensure-generic-function 'pixel_bias-val :lambda-list '(m))
(cl:defmethod pixel_bias-val ((m <traffic_light_target>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:pixel_bias-val is deprecated.  Use comm_msg-msg:pixel_bias instead.")
  (pixel_bias m))

(cl:ensure-generic-function 'rect_bias-val :lambda-list '(m))
(cl:defmethod rect_bias-val ((m <traffic_light_target>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:rect_bias-val is deprecated.  Use comm_msg-msg:rect_bias instead.")
  (rect_bias m))

(cl:ensure-generic-function 'target-val :lambda-list '(m))
(cl:defmethod target-val ((m <traffic_light_target>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:target-val is deprecated.  Use comm_msg-msg:target instead.")
  (target m))

(cl:ensure-generic-function 'nears-val :lambda-list '(m))
(cl:defmethod nears-val ((m <traffic_light_target>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:nears-val is deprecated.  Use comm_msg-msg:nears instead.")
  (nears m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <traffic_light_target>) ostream)
  "Serializes a message object of type '<traffic_light_target>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'is_open)) ostream)
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'pixel_bias))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'rect_bias))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'target) ostream)
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'nears))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'nears))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <traffic_light_target>) istream)
  "Deserializes a message object of type '<traffic_light_target>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'is_open)) (cl:read-byte istream))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'pixel_bias) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'rect_bias) (roslisp-utils:decode-single-float-bits bits)))
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'target) istream)
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'nears) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'nears)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'comm_msg-msg:traffic_light_info))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<traffic_light_target>)))
  "Returns string type for a message object of type '<traffic_light_target>"
  "comm_msg/traffic_light_target")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'traffic_light_target)))
  "Returns string type for a message object of type 'traffic_light_target"
  "comm_msg/traffic_light_target")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<traffic_light_target>)))
  "Returns md5sum for a message object of type '<traffic_light_target>"
  "5c8d3210265e381480cecfd925687150")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'traffic_light_target)))
  "Returns md5sum for a message object of type 'traffic_light_target"
  "5c8d3210265e381480cecfd925687150")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<traffic_light_target>)))
  "Returns full string definition for message of type '<traffic_light_target>"
  (cl:format cl:nil "uint8                   is_open     # 是否开启交通灯检测：0(关闭) 1(开启)~%float32                 pixel_bias  # 像素偏差~%float32                 rect_bias   # 检测框匹配率偏差~%traffic_light_info      target      # 目标交通灯信息~%traffic_light_info[]    nears       # 周边交通灯信息~%================================================================================~%MSG: comm_msg/traffic_light_info~%uint8    type           # 交通灯显示类型：1(读秒类型) 2(频闪类型) 3(高频亮灭型)~%float32  pos_x          # 交通灯中在地图上的坐标x~%float32  pos_y          # 交通灯中在地图上的坐标y~%float32  pos_z          # 交通灯中在地图上的坐标z~%float32  row_length     # 交通灯横向长度~%float32  col_length     # 交通灯纵向长度~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'traffic_light_target)))
  "Returns full string definition for message of type 'traffic_light_target"
  (cl:format cl:nil "uint8                   is_open     # 是否开启交通灯检测：0(关闭) 1(开启)~%float32                 pixel_bias  # 像素偏差~%float32                 rect_bias   # 检测框匹配率偏差~%traffic_light_info      target      # 目标交通灯信息~%traffic_light_info[]    nears       # 周边交通灯信息~%================================================================================~%MSG: comm_msg/traffic_light_info~%uint8    type           # 交通灯显示类型：1(读秒类型) 2(频闪类型) 3(高频亮灭型)~%float32  pos_x          # 交通灯中在地图上的坐标x~%float32  pos_y          # 交通灯中在地图上的坐标y~%float32  pos_z          # 交通灯中在地图上的坐标z~%float32  row_length     # 交通灯横向长度~%float32  col_length     # 交通灯纵向长度~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <traffic_light_target>))
  (cl:+ 0
     1
     4
     4
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'target))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'nears) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <traffic_light_target>))
  "Converts a ROS message object to a list"
  (cl:list 'traffic_light_target
    (cl:cons ':is_open (is_open msg))
    (cl:cons ':pixel_bias (pixel_bias msg))
    (cl:cons ':rect_bias (rect_bias msg))
    (cl:cons ':target (target msg))
    (cl:cons ':nears (nears msg))
))
