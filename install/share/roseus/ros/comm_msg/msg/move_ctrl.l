;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::move_ctrl)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'move_ctrl (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::MOVE_CTRL")
  (make-package "COMM_MSG::MOVE_CTRL"))

(in-package "ROS")
;;//! \htmlinclude move_ctrl.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass comm_msg::move_ctrl
  :super ros::object
  :slots (_header _isExistTask _isTeleCtrl _isCleaning _steeringAngle _moveLinearVel _isMoveSmooth _isStopSmooth _isUseSecurityRedundancy _isIgnoreLaserData _isIgnoreUltrasonicData _planningModel _brushModel ))

(defmethod comm_msg::move_ctrl
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:isExistTask __isExistTask) 0)
    ((:isTeleCtrl __isTeleCtrl) 0)
    ((:isCleaning __isCleaning) 0)
    ((:steeringAngle __steeringAngle) 0.0)
    ((:moveLinearVel __moveLinearVel) 0.0)
    ((:isMoveSmooth __isMoveSmooth) 0)
    ((:isStopSmooth __isStopSmooth) 0)
    ((:isUseSecurityRedundancy __isUseSecurityRedundancy) 0)
    ((:isIgnoreLaserData __isIgnoreLaserData) 0)
    ((:isIgnoreUltrasonicData __isIgnoreUltrasonicData) 0)
    ((:planningModel __planningModel) 0)
    ((:brushModel __brushModel) 0)
    )
   (send-super :init)
   (setq _header __header)
   (setq _isExistTask (round __isExistTask))
   (setq _isTeleCtrl (round __isTeleCtrl))
   (setq _isCleaning (round __isCleaning))
   (setq _steeringAngle (float __steeringAngle))
   (setq _moveLinearVel (float __moveLinearVel))
   (setq _isMoveSmooth (round __isMoveSmooth))
   (setq _isStopSmooth (round __isStopSmooth))
   (setq _isUseSecurityRedundancy (round __isUseSecurityRedundancy))
   (setq _isIgnoreLaserData (round __isIgnoreLaserData))
   (setq _isIgnoreUltrasonicData (round __isIgnoreUltrasonicData))
   (setq _planningModel (round __planningModel))
   (setq _brushModel (round __brushModel))
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:isExistTask
   (&optional __isExistTask)
   (if __isExistTask (setq _isExistTask __isExistTask)) _isExistTask)
  (:isTeleCtrl
   (&optional __isTeleCtrl)
   (if __isTeleCtrl (setq _isTeleCtrl __isTeleCtrl)) _isTeleCtrl)
  (:isCleaning
   (&optional __isCleaning)
   (if __isCleaning (setq _isCleaning __isCleaning)) _isCleaning)
  (:steeringAngle
   (&optional __steeringAngle)
   (if __steeringAngle (setq _steeringAngle __steeringAngle)) _steeringAngle)
  (:moveLinearVel
   (&optional __moveLinearVel)
   (if __moveLinearVel (setq _moveLinearVel __moveLinearVel)) _moveLinearVel)
  (:isMoveSmooth
   (&optional __isMoveSmooth)
   (if __isMoveSmooth (setq _isMoveSmooth __isMoveSmooth)) _isMoveSmooth)
  (:isStopSmooth
   (&optional __isStopSmooth)
   (if __isStopSmooth (setq _isStopSmooth __isStopSmooth)) _isStopSmooth)
  (:isUseSecurityRedundancy
   (&optional __isUseSecurityRedundancy)
   (if __isUseSecurityRedundancy (setq _isUseSecurityRedundancy __isUseSecurityRedundancy)) _isUseSecurityRedundancy)
  (:isIgnoreLaserData
   (&optional __isIgnoreLaserData)
   (if __isIgnoreLaserData (setq _isIgnoreLaserData __isIgnoreLaserData)) _isIgnoreLaserData)
  (:isIgnoreUltrasonicData
   (&optional __isIgnoreUltrasonicData)
   (if __isIgnoreUltrasonicData (setq _isIgnoreUltrasonicData __isIgnoreUltrasonicData)) _isIgnoreUltrasonicData)
  (:planningModel
   (&optional __planningModel)
   (if __planningModel (setq _planningModel __planningModel)) _planningModel)
  (:brushModel
   (&optional __brushModel)
   (if __brushModel (setq _brushModel __brushModel)) _brushModel)
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; uint8 _isExistTask
    1
    ;; uint8 _isTeleCtrl
    1
    ;; uint8 _isCleaning
    1
    ;; float32 _steeringAngle
    4
    ;; float32 _moveLinearVel
    4
    ;; uint8 _isMoveSmooth
    1
    ;; uint8 _isStopSmooth
    1
    ;; uint8 _isUseSecurityRedundancy
    1
    ;; uint8 _isIgnoreLaserData
    1
    ;; uint8 _isIgnoreUltrasonicData
    1
    ;; uint8 _planningModel
    1
    ;; uint8 _brushModel
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; uint8 _isExistTask
       (write-byte _isExistTask s)
     ;; uint8 _isTeleCtrl
       (write-byte _isTeleCtrl s)
     ;; uint8 _isCleaning
       (write-byte _isCleaning s)
     ;; float32 _steeringAngle
       (sys::poke _steeringAngle (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _moveLinearVel
       (sys::poke _moveLinearVel (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; uint8 _isMoveSmooth
       (write-byte _isMoveSmooth s)
     ;; uint8 _isStopSmooth
       (write-byte _isStopSmooth s)
     ;; uint8 _isUseSecurityRedundancy
       (write-byte _isUseSecurityRedundancy s)
     ;; uint8 _isIgnoreLaserData
       (write-byte _isIgnoreLaserData s)
     ;; uint8 _isIgnoreUltrasonicData
       (write-byte _isIgnoreUltrasonicData s)
     ;; uint8 _planningModel
       (write-byte _planningModel s)
     ;; uint8 _brushModel
       (write-byte _brushModel s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; uint8 _isExistTask
     (setq _isExistTask (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _isTeleCtrl
     (setq _isTeleCtrl (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _isCleaning
     (setq _isCleaning (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; float32 _steeringAngle
     (setq _steeringAngle (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _moveLinearVel
     (setq _moveLinearVel (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; uint8 _isMoveSmooth
     (setq _isMoveSmooth (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _isStopSmooth
     (setq _isStopSmooth (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _isUseSecurityRedundancy
     (setq _isUseSecurityRedundancy (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _isIgnoreLaserData
     (setq _isIgnoreLaserData (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _isIgnoreUltrasonicData
     (setq _isIgnoreUltrasonicData (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _planningModel
     (setq _planningModel (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _brushModel
     (setq _brushModel (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;;
   self)
  )

(setf (get comm_msg::move_ctrl :md5sum-) "50e3072a553418d33ffdbff20878da88")
(setf (get comm_msg::move_ctrl :datatype-) "comm_msg/move_ctrl")
(setf (get comm_msg::move_ctrl :definition-)
      "Header  header

uint8   isExistTask
uint8   isTeleCtrl
uint8   isCleaning
float32 steeringAngle
float32 moveLinearVel
uint8   isMoveSmooth
uint8   isStopSmooth

# 发给安全冗余模块新增的消息
uint8   isUseSecurityRedundancy
uint8   isIgnoreLaserData
uint8   isIgnoreUltrasonicData
uint8   planningModel          # 0无效模式, 1贴边模式, 2跟线模式, 3发点远程遥控模式, 4方向远程遥控模式, 5调试工具遥控车辆
uint8   brushModel             # 0关刷模式，1开刷模式
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

")



(provide :comm_msg/move_ctrl "50e3072a553418d33ffdbff20878da88")


