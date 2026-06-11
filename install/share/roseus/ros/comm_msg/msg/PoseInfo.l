;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::PoseInfo)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'PoseInfo (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::POSEINFO")
  (make-package "COMM_MSG::POSEINFO"))

(in-package "ROS")
;;//! \htmlinclude PoseInfo.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass comm_msg::PoseInfo
  :super ros::object
  :slots (_header _map_name _localization_score _status _x _y _z _roll _pitch _yaw ))

(defmethod comm_msg::PoseInfo
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:map_name __map_name) "")
    ((:localization_score __localization_score) 0)
    ((:status __status) 0)
    ((:x __x) 0.0)
    ((:y __y) 0.0)
    ((:z __z) 0.0)
    ((:roll __roll) 0.0)
    ((:pitch __pitch) 0.0)
    ((:yaw __yaw) 0.0)
    )
   (send-super :init)
   (setq _header __header)
   (setq _map_name (string __map_name))
   (setq _localization_score (round __localization_score))
   (setq _status (round __status))
   (setq _x (float __x))
   (setq _y (float __y))
   (setq _z (float __z))
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
  (:map_name
   (&optional __map_name)
   (if __map_name (setq _map_name __map_name)) _map_name)
  (:localization_score
   (&optional __localization_score)
   (if __localization_score (setq _localization_score __localization_score)) _localization_score)
  (:status
   (&optional __status)
   (if __status (setq _status __status)) _status)
  (:x
   (&optional __x)
   (if __x (setq _x __x)) _x)
  (:y
   (&optional __y)
   (if __y (setq _y __y)) _y)
  (:z
   (&optional __z)
   (if __z (setq _z __z)) _z)
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
    ;; string _map_name
    4 (length _map_name)
    ;; int32 _localization_score
    4
    ;; int32 _status
    4
    ;; float32 _x
    4
    ;; float32 _y
    4
    ;; float32 _z
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
     ;; string _map_name
       (write-long (length _map_name) s) (princ _map_name s)
     ;; int32 _localization_score
       (write-long _localization_score s)
     ;; int32 _status
       (write-long _status s)
     ;; float32 _x
       (sys::poke _x (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _y
       (sys::poke _y (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _z
       (sys::poke _z (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
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
   ;; string _map_name
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _map_name (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; int32 _localization_score
     (setq _localization_score (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; int32 _status
     (setq _status (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; float32 _x
     (setq _x (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _y
     (setq _y (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _z
     (setq _z (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _roll
     (setq _roll (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _pitch
     (setq _pitch (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _yaw
     (setq _yaw (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get comm_msg::PoseInfo :md5sum-) "7912a75f0912ef1c02553a53e7d7a9ec")
(setf (get comm_msg::PoseInfo :datatype-) "comm_msg/PoseInfo")
(setf (get comm_msg::PoseInfo :definition-)
      "# location info
Header header
string map_name
#0~100
int32 localization_score

int32 status

float32 x
float32 y
float32 z
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



(provide :comm_msg/PoseInfo "7912a75f0912ef1c02553a53e7d7a9ec")


