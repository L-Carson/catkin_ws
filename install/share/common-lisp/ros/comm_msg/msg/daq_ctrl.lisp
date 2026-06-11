; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude daq_ctrl.msg.html

(cl:defclass <daq_ctrl> (roslisp-msg-protocol:ros-message)
  ((daq_fusion
    :reader daq_fusion
    :initarg :daq_fusion
    :type cl:boolean
    :initform cl:nil)
   (daq_hz
    :reader daq_hz
    :initarg :daq_hz
    :type cl:fixnum
    :initform 0)
   (daq_camera
    :reader daq_camera
    :initarg :daq_camera
    :type (cl:vector cl:fixnum)
   :initform (cl:make-array 0 :element-type 'cl:fixnum :initial-element 0))
   (daq_lidar
    :reader daq_lidar
    :initarg :daq_lidar
    :type (cl:vector cl:fixnum)
   :initform (cl:make-array 0 :element-type 'cl:fixnum :initial-element 0)))
)

(cl:defclass daq_ctrl (<daq_ctrl>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <daq_ctrl>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'daq_ctrl)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<daq_ctrl> is deprecated: use comm_msg-msg:daq_ctrl instead.")))

(cl:ensure-generic-function 'daq_fusion-val :lambda-list '(m))
(cl:defmethod daq_fusion-val ((m <daq_ctrl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:daq_fusion-val is deprecated.  Use comm_msg-msg:daq_fusion instead.")
  (daq_fusion m))

(cl:ensure-generic-function 'daq_hz-val :lambda-list '(m))
(cl:defmethod daq_hz-val ((m <daq_ctrl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:daq_hz-val is deprecated.  Use comm_msg-msg:daq_hz instead.")
  (daq_hz m))

(cl:ensure-generic-function 'daq_camera-val :lambda-list '(m))
(cl:defmethod daq_camera-val ((m <daq_ctrl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:daq_camera-val is deprecated.  Use comm_msg-msg:daq_camera instead.")
  (daq_camera m))

(cl:ensure-generic-function 'daq_lidar-val :lambda-list '(m))
(cl:defmethod daq_lidar-val ((m <daq_ctrl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:daq_lidar-val is deprecated.  Use comm_msg-msg:daq_lidar instead.")
  (daq_lidar m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <daq_ctrl>) ostream)
  "Serializes a message object of type '<daq_ctrl>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'daq_fusion) 1 0)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'daq_hz)) ostream)
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'daq_camera))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:write-byte (cl:ldb (cl:byte 8 0) ele) ostream))
   (cl:slot-value msg 'daq_camera))
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'daq_lidar))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:write-byte (cl:ldb (cl:byte 8 0) ele) ostream))
   (cl:slot-value msg 'daq_lidar))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <daq_ctrl>) istream)
  "Deserializes a message object of type '<daq_ctrl>"
    (cl:setf (cl:slot-value msg 'daq_fusion) (cl:not (cl:zerop (cl:read-byte istream))))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'daq_hz)) (cl:read-byte istream))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'daq_camera) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'daq_camera)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:aref vals i)) (cl:read-byte istream)))))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'daq_lidar) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'daq_lidar)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:aref vals i)) (cl:read-byte istream)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<daq_ctrl>)))
  "Returns string type for a message object of type '<daq_ctrl>"
  "comm_msg/daq_ctrl")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'daq_ctrl)))
  "Returns string type for a message object of type 'daq_ctrl"
  "comm_msg/daq_ctrl")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<daq_ctrl>)))
  "Returns md5sum for a message object of type '<daq_ctrl>"
  "b65aad8c511a16f789728648f67ea4d6")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'daq_ctrl)))
  "Returns md5sum for a message object of type 'daq_ctrl"
  "b65aad8c511a16f789728648f67ea4d6")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<daq_ctrl>)))
  "Returns full string definition for message of type '<daq_ctrl>"
  (cl:format cl:nil "# e.g. begin: rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl \"{daq_fusion: 1, daq_hz: 1, daq_camera:[1,2,3,4,5,6,7,8,9,10], daq_lidar:[1,2,3,4,5]}\"     # 相机+雷达：时间戳需对齐~%# e.g. begin: rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl \"{daq_fusion: 0, daq_hz: 1, daq_camera:[1,2,3,4,5,6,7,8,9,10], daq_lidar:[1,2,3,4,5]}\"     # 相机+雷达：时间戳无需对齐~%~%# e.g. begin: rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl \"{daq_fusion: 1, daq_hz: 1, daq_camera:[1,2,3,4,5,6,7,8,9,10], daq_lidar:[]}\"              # 相机：时间戳需对齐~%# e.g. begin: rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl \"{daq_fusion: 0, daq_hz: 1, daq_camera:[1,2,3,4,5,6,7,8,9,10], daq_lidar:[]}\"              # 相机：时间戳无需对齐~%~%# e.g. begin: rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl \"{daq_fusion: 1, daq_hz: 1, daq_camera:[], daq_lidar:[1,2,3,4,5]}\"                         # 雷达：时间戳需对齐~%# e.g. begin: rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl \"{daq_fusion: 0, daq_hz: 1, daq_camera:[], daq_lidar:[1,2,3,4,5]}\"                         # 雷达：时间戳无需对齐~%~%# e.g. end:   rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl \"{daq_fusion: 1, daq_hz: 1, daq_camera:[], daq_lidar:[]}\"                                  # 结束采集~%# e.g. end:   rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl \"{daq_fusion: 0, daq_hz: 1, daq_camera:[], daq_lidar:[]}\"                                  # 结束采集（同上）~%~%bool    daq_fusion      # 采集融合数据（相机+雷达：时间戳对齐）~%uint8   daq_hz          # 采集频率~%uint8[] daq_camera      # 采集相机数据~%uint8[] daq_lidar       # 采集雷达数据~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'daq_ctrl)))
  "Returns full string definition for message of type 'daq_ctrl"
  (cl:format cl:nil "# e.g. begin: rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl \"{daq_fusion: 1, daq_hz: 1, daq_camera:[1,2,3,4,5,6,7,8,9,10], daq_lidar:[1,2,3,4,5]}\"     # 相机+雷达：时间戳需对齐~%# e.g. begin: rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl \"{daq_fusion: 0, daq_hz: 1, daq_camera:[1,2,3,4,5,6,7,8,9,10], daq_lidar:[1,2,3,4,5]}\"     # 相机+雷达：时间戳无需对齐~%~%# e.g. begin: rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl \"{daq_fusion: 1, daq_hz: 1, daq_camera:[1,2,3,4,5,6,7,8,9,10], daq_lidar:[]}\"              # 相机：时间戳需对齐~%# e.g. begin: rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl \"{daq_fusion: 0, daq_hz: 1, daq_camera:[1,2,3,4,5,6,7,8,9,10], daq_lidar:[]}\"              # 相机：时间戳无需对齐~%~%# e.g. begin: rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl \"{daq_fusion: 1, daq_hz: 1, daq_camera:[], daq_lidar:[1,2,3,4,5]}\"                         # 雷达：时间戳需对齐~%# e.g. begin: rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl \"{daq_fusion: 0, daq_hz: 1, daq_camera:[], daq_lidar:[1,2,3,4,5]}\"                         # 雷达：时间戳无需对齐~%~%# e.g. end:   rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl \"{daq_fusion: 1, daq_hz: 1, daq_camera:[], daq_lidar:[]}\"                                  # 结束采集~%# e.g. end:   rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl \"{daq_fusion: 0, daq_hz: 1, daq_camera:[], daq_lidar:[]}\"                                  # 结束采集（同上）~%~%bool    daq_fusion      # 采集融合数据（相机+雷达：时间戳对齐）~%uint8   daq_hz          # 采集频率~%uint8[] daq_camera      # 采集相机数据~%uint8[] daq_lidar       # 采集雷达数据~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <daq_ctrl>))
  (cl:+ 0
     1
     1
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'daq_camera) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 1)))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'daq_lidar) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 1)))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <daq_ctrl>))
  "Converts a ROS message object to a list"
  (cl:list 'daq_ctrl
    (cl:cons ':daq_fusion (daq_fusion msg))
    (cl:cons ':daq_hz (daq_hz msg))
    (cl:cons ':daq_camera (daq_camera msg))
    (cl:cons ':daq_lidar (daq_lidar msg))
))
