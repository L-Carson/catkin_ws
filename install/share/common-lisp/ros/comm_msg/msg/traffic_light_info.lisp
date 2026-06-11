; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude traffic_light_info.msg.html

(cl:defclass <traffic_light_info> (roslisp-msg-protocol:ros-message)
  ((type
    :reader type
    :initarg :type
    :type cl:fixnum
    :initform 0)
   (pos_x
    :reader pos_x
    :initarg :pos_x
    :type cl:float
    :initform 0.0)
   (pos_y
    :reader pos_y
    :initarg :pos_y
    :type cl:float
    :initform 0.0)
   (pos_z
    :reader pos_z
    :initarg :pos_z
    :type cl:float
    :initform 0.0)
   (row_length
    :reader row_length
    :initarg :row_length
    :type cl:float
    :initform 0.0)
   (col_length
    :reader col_length
    :initarg :col_length
    :type cl:float
    :initform 0.0))
)

(cl:defclass traffic_light_info (<traffic_light_info>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <traffic_light_info>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'traffic_light_info)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<traffic_light_info> is deprecated: use comm_msg-msg:traffic_light_info instead.")))

(cl:ensure-generic-function 'type-val :lambda-list '(m))
(cl:defmethod type-val ((m <traffic_light_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:type-val is deprecated.  Use comm_msg-msg:type instead.")
  (type m))

(cl:ensure-generic-function 'pos_x-val :lambda-list '(m))
(cl:defmethod pos_x-val ((m <traffic_light_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:pos_x-val is deprecated.  Use comm_msg-msg:pos_x instead.")
  (pos_x m))

(cl:ensure-generic-function 'pos_y-val :lambda-list '(m))
(cl:defmethod pos_y-val ((m <traffic_light_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:pos_y-val is deprecated.  Use comm_msg-msg:pos_y instead.")
  (pos_y m))

(cl:ensure-generic-function 'pos_z-val :lambda-list '(m))
(cl:defmethod pos_z-val ((m <traffic_light_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:pos_z-val is deprecated.  Use comm_msg-msg:pos_z instead.")
  (pos_z m))

(cl:ensure-generic-function 'row_length-val :lambda-list '(m))
(cl:defmethod row_length-val ((m <traffic_light_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:row_length-val is deprecated.  Use comm_msg-msg:row_length instead.")
  (row_length m))

(cl:ensure-generic-function 'col_length-val :lambda-list '(m))
(cl:defmethod col_length-val ((m <traffic_light_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:col_length-val is deprecated.  Use comm_msg-msg:col_length instead.")
  (col_length m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <traffic_light_info>) ostream)
  "Serializes a message object of type '<traffic_light_info>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'type)) ostream)
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'pos_x))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'pos_y))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'pos_z))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'row_length))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'col_length))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <traffic_light_info>) istream)
  "Deserializes a message object of type '<traffic_light_info>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'type)) (cl:read-byte istream))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'pos_x) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'pos_y) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'pos_z) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'row_length) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'col_length) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<traffic_light_info>)))
  "Returns string type for a message object of type '<traffic_light_info>"
  "comm_msg/traffic_light_info")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'traffic_light_info)))
  "Returns string type for a message object of type 'traffic_light_info"
  "comm_msg/traffic_light_info")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<traffic_light_info>)))
  "Returns md5sum for a message object of type '<traffic_light_info>"
  "b4755f326c42173c8ecd115dcd1f1001")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'traffic_light_info)))
  "Returns md5sum for a message object of type 'traffic_light_info"
  "b4755f326c42173c8ecd115dcd1f1001")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<traffic_light_info>)))
  "Returns full string definition for message of type '<traffic_light_info>"
  (cl:format cl:nil "uint8    type           # 交通灯显示类型：1(读秒类型) 2(频闪类型) 3(高频亮灭型)~%float32  pos_x          # 交通灯中在地图上的坐标x~%float32  pos_y          # 交通灯中在地图上的坐标y~%float32  pos_z          # 交通灯中在地图上的坐标z~%float32  row_length     # 交通灯横向长度~%float32  col_length     # 交通灯纵向长度~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'traffic_light_info)))
  "Returns full string definition for message of type 'traffic_light_info"
  (cl:format cl:nil "uint8    type           # 交通灯显示类型：1(读秒类型) 2(频闪类型) 3(高频亮灭型)~%float32  pos_x          # 交通灯中在地图上的坐标x~%float32  pos_y          # 交通灯中在地图上的坐标y~%float32  pos_z          # 交通灯中在地图上的坐标z~%float32  row_length     # 交通灯横向长度~%float32  col_length     # 交通灯纵向长度~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <traffic_light_info>))
  (cl:+ 0
     1
     4
     4
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <traffic_light_info>))
  "Converts a ROS message object to a list"
  (cl:list 'traffic_light_info
    (cl:cons ':type (type msg))
    (cl:cons ':pos_x (pos_x msg))
    (cl:cons ':pos_y (pos_y msg))
    (cl:cons ':pos_z (pos_z msg))
    (cl:cons ':row_length (row_length msg))
    (cl:cons ':col_length (col_length msg))
))
