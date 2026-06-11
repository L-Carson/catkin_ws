;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::ultrasonicsData)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'ultrasonicsData (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::ULTRASONICSDATA")
  (make-package "COMM_MSG::ULTRASONICSDATA"))

(in-package "ROS")
;;//! \htmlinclude ultrasonicsData.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass comm_msg::ultrasonicsData
  :super ros::object
  :slots (_header _usData ))

(defmethod comm_msg::ultrasonicsData
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:usData __usData) ())
    )
   (send-super :init)
   (setq _header __header)
   (setq _usData __usData)
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:usData
   (&rest __usData)
   (if (keywordp (car __usData))
       (send* _usData __usData)
     (progn
       (if __usData (setq _usData (car __usData)))
       _usData)))
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; comm_msg/ultrasonicData[] _usData
    (apply #'+ (send-all _usData :serialization-length)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; comm_msg/ultrasonicData[] _usData
     (write-long (length _usData) s)
     (dolist (elem _usData)
       (send elem :serialize s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; comm_msg/ultrasonicData[] _usData
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _usData (let (r) (dotimes (i n) (push (instance comm_msg::ultrasonicData :init) r)) r))
     (dolist (elem- _usData)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;;
   self)
  )

(setf (get comm_msg::ultrasonicsData :md5sum-) "107cce378ac5b6b859f4402e5340a53f")
(setf (get comm_msg::ultrasonicsData :datatype-) "comm_msg/ultrasonicsData")
(setf (get comm_msg::ultrasonicsData :definition-)
      "Header           header
ultrasonicData[] usData
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
MSG: comm_msg/ultrasonicData
string   usName
coor2D[] data
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
")



(provide :comm_msg/ultrasonicsData "107cce378ac5b6b859f4402e5340a53f")


