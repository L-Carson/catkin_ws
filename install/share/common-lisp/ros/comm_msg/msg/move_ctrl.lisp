; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude move_ctrl.msg.html

(cl:defclass <move_ctrl> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (isExistTask
    :reader isExistTask
    :initarg :isExistTask
    :type cl:fixnum
    :initform 0)
   (isTeleCtrl
    :reader isTeleCtrl
    :initarg :isTeleCtrl
    :type cl:fixnum
    :initform 0)
   (isCleaning
    :reader isCleaning
    :initarg :isCleaning
    :type cl:fixnum
    :initform 0)
   (steeringAngle
    :reader steeringAngle
    :initarg :steeringAngle
    :type cl:float
    :initform 0.0)
   (moveLinearVel
    :reader moveLinearVel
    :initarg :moveLinearVel
    :type cl:float
    :initform 0.0)
   (isMoveSmooth
    :reader isMoveSmooth
    :initarg :isMoveSmooth
    :type cl:fixnum
    :initform 0)
   (isStopSmooth
    :reader isStopSmooth
    :initarg :isStopSmooth
    :type cl:fixnum
    :initform 0)
   (isUseSecurityRedundancy
    :reader isUseSecurityRedundancy
    :initarg :isUseSecurityRedundancy
    :type cl:fixnum
    :initform 0)
   (isIgnoreLaserData
    :reader isIgnoreLaserData
    :initarg :isIgnoreLaserData
    :type cl:fixnum
    :initform 0)
   (isIgnoreUltrasonicData
    :reader isIgnoreUltrasonicData
    :initarg :isIgnoreUltrasonicData
    :type cl:fixnum
    :initform 0)
   (planningModel
    :reader planningModel
    :initarg :planningModel
    :type cl:fixnum
    :initform 0)
   (brushModel
    :reader brushModel
    :initarg :brushModel
    :type cl:fixnum
    :initform 0))
)

(cl:defclass move_ctrl (<move_ctrl>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <move_ctrl>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'move_ctrl)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<move_ctrl> is deprecated: use comm_msg-msg:move_ctrl instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <move_ctrl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:header-val is deprecated.  Use comm_msg-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'isExistTask-val :lambda-list '(m))
(cl:defmethod isExistTask-val ((m <move_ctrl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isExistTask-val is deprecated.  Use comm_msg-msg:isExistTask instead.")
  (isExistTask m))

(cl:ensure-generic-function 'isTeleCtrl-val :lambda-list '(m))
(cl:defmethod isTeleCtrl-val ((m <move_ctrl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isTeleCtrl-val is deprecated.  Use comm_msg-msg:isTeleCtrl instead.")
  (isTeleCtrl m))

(cl:ensure-generic-function 'isCleaning-val :lambda-list '(m))
(cl:defmethod isCleaning-val ((m <move_ctrl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isCleaning-val is deprecated.  Use comm_msg-msg:isCleaning instead.")
  (isCleaning m))

(cl:ensure-generic-function 'steeringAngle-val :lambda-list '(m))
(cl:defmethod steeringAngle-val ((m <move_ctrl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:steeringAngle-val is deprecated.  Use comm_msg-msg:steeringAngle instead.")
  (steeringAngle m))

(cl:ensure-generic-function 'moveLinearVel-val :lambda-list '(m))
(cl:defmethod moveLinearVel-val ((m <move_ctrl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:moveLinearVel-val is deprecated.  Use comm_msg-msg:moveLinearVel instead.")
  (moveLinearVel m))

(cl:ensure-generic-function 'isMoveSmooth-val :lambda-list '(m))
(cl:defmethod isMoveSmooth-val ((m <move_ctrl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isMoveSmooth-val is deprecated.  Use comm_msg-msg:isMoveSmooth instead.")
  (isMoveSmooth m))

(cl:ensure-generic-function 'isStopSmooth-val :lambda-list '(m))
(cl:defmethod isStopSmooth-val ((m <move_ctrl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isStopSmooth-val is deprecated.  Use comm_msg-msg:isStopSmooth instead.")
  (isStopSmooth m))

(cl:ensure-generic-function 'isUseSecurityRedundancy-val :lambda-list '(m))
(cl:defmethod isUseSecurityRedundancy-val ((m <move_ctrl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isUseSecurityRedundancy-val is deprecated.  Use comm_msg-msg:isUseSecurityRedundancy instead.")
  (isUseSecurityRedundancy m))

(cl:ensure-generic-function 'isIgnoreLaserData-val :lambda-list '(m))
(cl:defmethod isIgnoreLaserData-val ((m <move_ctrl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isIgnoreLaserData-val is deprecated.  Use comm_msg-msg:isIgnoreLaserData instead.")
  (isIgnoreLaserData m))

(cl:ensure-generic-function 'isIgnoreUltrasonicData-val :lambda-list '(m))
(cl:defmethod isIgnoreUltrasonicData-val ((m <move_ctrl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isIgnoreUltrasonicData-val is deprecated.  Use comm_msg-msg:isIgnoreUltrasonicData instead.")
  (isIgnoreUltrasonicData m))

(cl:ensure-generic-function 'planningModel-val :lambda-list '(m))
(cl:defmethod planningModel-val ((m <move_ctrl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:planningModel-val is deprecated.  Use comm_msg-msg:planningModel instead.")
  (planningModel m))

(cl:ensure-generic-function 'brushModel-val :lambda-list '(m))
(cl:defmethod brushModel-val ((m <move_ctrl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:brushModel-val is deprecated.  Use comm_msg-msg:brushModel instead.")
  (brushModel m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <move_ctrl>) ostream)
  "Serializes a message object of type '<move_ctrl>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isExistTask)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isTeleCtrl)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isCleaning)) ostream)
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'steeringAngle))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'moveLinearVel))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isMoveSmooth)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isStopSmooth)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isUseSecurityRedundancy)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isIgnoreLaserData)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isIgnoreUltrasonicData)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'planningModel)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'brushModel)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <move_ctrl>) istream)
  "Deserializes a message object of type '<move_ctrl>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isExistTask)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isTeleCtrl)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isCleaning)) (cl:read-byte istream))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'steeringAngle) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'moveLinearVel) (roslisp-utils:decode-single-float-bits bits)))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isMoveSmooth)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isStopSmooth)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isUseSecurityRedundancy)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isIgnoreLaserData)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isIgnoreUltrasonicData)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'planningModel)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'brushModel)) (cl:read-byte istream))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<move_ctrl>)))
  "Returns string type for a message object of type '<move_ctrl>"
  "comm_msg/move_ctrl")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'move_ctrl)))
  "Returns string type for a message object of type 'move_ctrl"
  "comm_msg/move_ctrl")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<move_ctrl>)))
  "Returns md5sum for a message object of type '<move_ctrl>"
  "50e3072a553418d33ffdbff20878da88")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'move_ctrl)))
  "Returns md5sum for a message object of type 'move_ctrl"
  "50e3072a553418d33ffdbff20878da88")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<move_ctrl>)))
  "Returns full string definition for message of type '<move_ctrl>"
  (cl:format cl:nil "Header  header~%~%uint8   isExistTask~%uint8   isTeleCtrl~%uint8   isCleaning~%float32 steeringAngle~%float32 moveLinearVel~%uint8   isMoveSmooth~%uint8   isStopSmooth~%~%# 发给安全冗余模块新增的消息~%uint8   isUseSecurityRedundancy~%uint8   isIgnoreLaserData~%uint8   isIgnoreUltrasonicData~%uint8   planningModel          # 0无效模式, 1贴边模式, 2跟线模式, 3发点远程遥控模式, 4方向远程遥控模式, 5调试工具遥控车辆~%uint8   brushModel             # 0关刷模式，1开刷模式~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'move_ctrl)))
  "Returns full string definition for message of type 'move_ctrl"
  (cl:format cl:nil "Header  header~%~%uint8   isExistTask~%uint8   isTeleCtrl~%uint8   isCleaning~%float32 steeringAngle~%float32 moveLinearVel~%uint8   isMoveSmooth~%uint8   isStopSmooth~%~%# 发给安全冗余模块新增的消息~%uint8   isUseSecurityRedundancy~%uint8   isIgnoreLaserData~%uint8   isIgnoreUltrasonicData~%uint8   planningModel          # 0无效模式, 1贴边模式, 2跟线模式, 3发点远程遥控模式, 4方向远程遥控模式, 5调试工具遥控车辆~%uint8   brushModel             # 0关刷模式，1开刷模式~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <move_ctrl>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     1
     1
     1
     4
     4
     1
     1
     1
     1
     1
     1
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <move_ctrl>))
  "Converts a ROS message object to a list"
  (cl:list 'move_ctrl
    (cl:cons ':header (header msg))
    (cl:cons ':isExistTask (isExistTask msg))
    (cl:cons ':isTeleCtrl (isTeleCtrl msg))
    (cl:cons ':isCleaning (isCleaning msg))
    (cl:cons ':steeringAngle (steeringAngle msg))
    (cl:cons ':moveLinearVel (moveLinearVel msg))
    (cl:cons ':isMoveSmooth (isMoveSmooth msg))
    (cl:cons ':isStopSmooth (isStopSmooth msg))
    (cl:cons ':isUseSecurityRedundancy (isUseSecurityRedundancy msg))
    (cl:cons ':isIgnoreLaserData (isIgnoreLaserData msg))
    (cl:cons ':isIgnoreUltrasonicData (isIgnoreUltrasonicData msg))
    (cl:cons ':planningModel (planningModel msg))
    (cl:cons ':brushModel (brushModel msg))
))
