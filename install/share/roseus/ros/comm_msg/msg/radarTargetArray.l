;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::radarTargetArray)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'radarTargetArray (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::RADARTARGETARRAY")
  (make-package "COMM_MSG::RADARTARGETARRAY"))

(in-package "ROS")
;;//! \htmlinclude radarTargetArray.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass comm_msg::radarTargetArray
  :super ros::object
  :slots (_header _radarID _allTargets ))

(defmethod comm_msg::radarTargetArray
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:radarID __radarID) 0)
    ((:allTargets __allTargets) ())
    )
   (send-super :init)
   (setq _header __header)
   (setq _radarID (round __radarID))
   (setq _allTargets __allTargets)
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:radarID
   (&optional __radarID)
   (if __radarID (setq _radarID __radarID)) _radarID)
  (:allTargets
   (&rest __allTargets)
   (if (keywordp (car __allTargets))
       (send* _allTargets __allTargets)
     (progn
       (if __allTargets (setq _allTargets (car __allTargets)))
       _allTargets)))
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; int16 _radarID
    2
    ;; comm_msg/radarTarget[] _allTargets
    (apply #'+ (send-all _allTargets :serialization-length)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; int16 _radarID
       (write-word _radarID s)
     ;; comm_msg/radarTarget[] _allTargets
     (write-long (length _allTargets) s)
     (dolist (elem _allTargets)
       (send elem :serialize s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; int16 _radarID
     (setq _radarID (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; comm_msg/radarTarget[] _allTargets
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _allTargets (let (r) (dotimes (i n) (push (instance comm_msg::radarTarget :init) r)) r))
     (dolist (elem- _allTargets)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;;
   self)
  )

(setf (get comm_msg::radarTargetArray :md5sum-) "6dbbce6501fbb2b1edc228d20004f209")
(setf (get comm_msg::radarTargetArray :datatype-) "comm_msg/radarTargetArray")
(setf (get comm_msg::radarTargetArray :definition-)
      "Header          header
int16           radarID
radarTarget[]   allTargets

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
MSG: comm_msg/radarTarget
float32   targetDistx         # 目标相对于清扫车参考坐标系X轴坐标
float32   targetDisty         # 目标相对于清扫车参考坐标系Y轴坐标
float32   targetVelx          # 目标相对于清扫车参考坐标系X轴速度
float32   targetVely          # 目标相对于清扫车参考坐标系Y轴速度
")



(provide :comm_msg/radarTargetArray "6dbbce6501fbb2b1edc228d20004f209")


