;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::locationInfo3D)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'locationInfo3D (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::LOCATIONINFO3D")
  (make-package "COMM_MSG::LOCATIONINFO3D"))

(in-package "ROS")
;;//! \htmlinclude locationInfo3D.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass comm_msg::locationInfo3D
  :super ros::object
  :slots (_header _status _subMapNo _posX _posY _posZ _roll _pitch _yaw ))

(defmethod comm_msg::locationInfo3D
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:status __status) 0)
    ((:subMapNo __subMapNo) 0)
    ((:posX __posX) 0.0)
    ((:posY __posY) 0.0)
    ((:posZ __posZ) 0.0)
    ((:roll __roll) 0.0)
    ((:pitch __pitch) 0.0)
    ((:yaw __yaw) 0.0)
    )
   (send-super :init)
   (setq _header __header)
   (setq _status (round __status))
   (setq _subMapNo (round __subMapNo))
   (setq _posX (float __posX))
   (setq _posY (float __posY))
   (setq _posZ (float __posZ))
   (setq _roll (float __roll))
   (setq _pitch (float __pitch))
   (setq _yaw (float __yaw))
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:status
   (&optional __status)
   (if __status (setq _status __status)) _status)
  (:subMapNo
   (&optional __subMapNo)
   (if __subMapNo (setq _subMapNo __subMapNo)) _subMapNo)
  (:posX
   (&optional __posX)
   (if __posX (setq _posX __posX)) _posX)
  (:posY
   (&optional __posY)
   (if __posY (setq _posY __posY)) _posY)
  (:posZ
   (&optional __posZ)
   (if __posZ (setq _posZ __posZ)) _posZ)
  (:roll
   (&optional __roll)
   (if __roll (setq _roll __roll)) _roll)
  (:pitch
   (&optional __pitch)
   (if __pitch (setq _pitch __pitch)) _pitch)
  (:yaw
   (&optional __yaw)
   (if __yaw (setq _yaw __yaw)) _yaw)
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; int32 _status
    4
    ;; int32 _subMapNo
    4
    ;; float32 _posX
    4
    ;; float32 _posY
    4
    ;; float32 _posZ
    4
    ;; float32 _roll
    4
    ;; float32 _pitch
    4
    ;; float32 _yaw
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; int32 _status
       (write-long _status s)
     ;; int32 _subMapNo
       (write-long _subMapNo s)
     ;; float32 _posX
       (sys::poke _posX (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _posY
       (sys::poke _posY (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _posZ
       (sys::poke _posZ (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _roll
       (sys::poke _roll (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _pitch
       (sys::poke _pitch (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _yaw
       (sys::poke _yaw (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; int32 _status
     (setq _status (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; int32 _subMapNo
     (setq _subMapNo (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; float32 _posX
     (setq _posX (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _posY
     (setq _posY (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _posZ
     (setq _posZ (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _roll
     (setq _roll (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _pitch
     (setq _pitch (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _yaw
     (setq _yaw (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get comm_msg::locationInfo3D :md5sum-) "d9467975d89bc17ef59573b6ebcb142d")
(setf (get comm_msg::locationInfo3D :datatype-) "comm_msg/locationInfo3D")
(setf (get comm_msg::locationInfo3D :definition-)
      "# location status, refer to MAP_NODE_STATE_ENUM
Header header
int32 status

# sub map no, this is valid when >= 0, if -ing status, this is the new sub map no
int32 subMapNo

# Comb Map pose, which is valid When status is > 0.
float32 posX
float32 posY
float32 posZ
float32 roll
float32 pitch
float32 yaw
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



(provide :comm_msg/locationInfo3D "d9467975d89bc17ef59573b6ebcb142d")


