;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::move_info)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'move_info (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::MOVE_INFO")
  (make-package "COMM_MSG::MOVE_INFO"))

(in-package "ROS")
;;//! \htmlinclude move_info.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass comm_msg::move_info
  :super ros::object
  :slots (_header _steeringAngle _moveLinearVel _yawAngle _yawAngleVel _accX _accY _accZ ))

(defmethod comm_msg::move_info
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:steeringAngle __steeringAngle) 0.0)
    ((:moveLinearVel __moveLinearVel) 0.0)
    ((:yawAngle __yawAngle) 0.0)
    ((:yawAngleVel __yawAngleVel) 0.0)
    ((:accX __accX) 0.0)
    ((:accY __accY) 0.0)
    ((:accZ __accZ) 0.0)
    )
   (send-super :init)
   (setq _header __header)
   (setq _steeringAngle (float __steeringAngle))
   (setq _moveLinearVel (float __moveLinearVel))
   (setq _yawAngle (float __yawAngle))
   (setq _yawAngleVel (float __yawAngleVel))
   (setq _accX (float __accX))
   (setq _accY (float __accY))
   (setq _accZ (float __accZ))
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:steeringAngle
   (&optional __steeringAngle)
   (if __steeringAngle (setq _steeringAngle __steeringAngle)) _steeringAngle)
  (:moveLinearVel
   (&optional __moveLinearVel)
   (if __moveLinearVel (setq _moveLinearVel __moveLinearVel)) _moveLinearVel)
  (:yawAngle
   (&optional __yawAngle)
   (if __yawAngle (setq _yawAngle __yawAngle)) _yawAngle)
  (:yawAngleVel
   (&optional __yawAngleVel)
   (if __yawAngleVel (setq _yawAngleVel __yawAngleVel)) _yawAngleVel)
  (:accX
   (&optional __accX)
   (if __accX (setq _accX __accX)) _accX)
  (:accY
   (&optional __accY)
   (if __accY (setq _accY __accY)) _accY)
  (:accZ
   (&optional __accZ)
   (if __accZ (setq _accZ __accZ)) _accZ)
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; float32 _steeringAngle
    4
    ;; float32 _moveLinearVel
    4
    ;; float32 _yawAngle
    4
    ;; float32 _yawAngleVel
    4
    ;; float32 _accX
    4
    ;; float32 _accY
    4
    ;; float32 _accZ
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; float32 _steeringAngle
       (sys::poke _steeringAngle (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _moveLinearVel
       (sys::poke _moveLinearVel (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _yawAngle
       (sys::poke _yawAngle (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _yawAngleVel
       (sys::poke _yawAngleVel (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _accX
       (sys::poke _accX (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _accY
       (sys::poke _accY (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _accZ
       (sys::poke _accZ (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; float32 _steeringAngle
     (setq _steeringAngle (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _moveLinearVel
     (setq _moveLinearVel (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _yawAngle
     (setq _yawAngle (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _yawAngleVel
     (setq _yawAngleVel (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _accX
     (setq _accX (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _accY
     (setq _accY (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _accZ
     (setq _accZ (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get comm_msg::move_info :md5sum-) "5651dd8f0d48de47c0d8061b96a98beb")
(setf (get comm_msg::move_info :datatype-) "comm_msg/move_info")
(setf (get comm_msg::move_info :definition-)
      "Header  header

float32 steeringAngle
float32 moveLinearVel
float32 yawAngle
float32 yawAngleVel
float32 accX
float32 accY
float32 accZ
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



(provide :comm_msg/move_info "5651dd8f0d48de47c0d8061b96a98beb")


