;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::roadmark)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'roadmark (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::ROADMARK")
  (make-package "COMM_MSG::ROADMARK"))

(in-package "ROS")
;;//! \htmlinclude roadmark.msg.html
(if (not (find-package "GEOMETRY_MSGS"))
  (ros::roseus-add-msgs "geometry_msgs"))
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass comm_msg::roadmark
  :super ros::object
  :slots (_header _polygons ))

(defmethod comm_msg::roadmark
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:polygons __polygons) ())
    )
   (send-super :init)
   (setq _header __header)
   (setq _polygons __polygons)
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:polygons
   (&rest __polygons)
   (if (keywordp (car __polygons))
       (send* _polygons __polygons)
     (progn
       (if __polygons (setq _polygons (car __polygons)))
       _polygons)))
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; geometry_msgs/Polygon[] _polygons
    (apply #'+ (send-all _polygons :serialization-length)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; geometry_msgs/Polygon[] _polygons
     (write-long (length _polygons) s)
     (dolist (elem _polygons)
       (send elem :serialize s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; geometry_msgs/Polygon[] _polygons
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _polygons (let (r) (dotimes (i n) (push (instance geometry_msgs::Polygon :init) r)) r))
     (dolist (elem- _polygons)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;;
   self)
  )

(setf (get comm_msg::roadmark :md5sum-) "436cca0b8e50ceff14d30527fc67b7a4")
(setf (get comm_msg::roadmark :datatype-) "comm_msg/roadmark")
(setf (get comm_msg::roadmark :definition-)
      "Header                  header
geometry_msgs/Polygon[] polygons
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
MSG: geometry_msgs/Polygon
#A specification of a polygon where the first and last points are assumed to be connected
Point32[] points

================================================================================
MSG: geometry_msgs/Point32
# This contains the position of a point in free space(with 32 bits of precision).
# It is recommeded to use Point wherever possible instead of Point32.  
# 
# This recommendation is to promote interoperability.  
#
# This message is designed to take up less space when sending
# lots of points at once, as in the case of a PointCloud.  

float32 x
float32 y
float32 z
")



(provide :comm_msg/roadmark "436cca0b8e50ceff14d30527fc67b7a4")


