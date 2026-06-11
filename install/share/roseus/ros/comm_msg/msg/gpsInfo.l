;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::gpsInfo)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'gpsInfo (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::GPSINFO")
  (make-package "COMM_MSG::GPSINFO"))

(in-package "ROS")
;;//! \htmlinclude gpsInfo.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass comm_msg::gpsInfo
  :super ros::object
  :slots (_header _satelliteNum _rsltType _longitude _latitude _altitude _yamAngle ))

(defmethod comm_msg::gpsInfo
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:satelliteNum __satelliteNum) 0)
    ((:rsltType __rsltType) 0)
    ((:longitude __longitude) 0.0)
    ((:latitude __latitude) 0.0)
    ((:altitude __altitude) 0.0)
    ((:yamAngle __yamAngle) 0.0)
    )
   (send-super :init)
   (setq _header __header)
   (setq _satelliteNum (round __satelliteNum))
   (setq _rsltType (round __rsltType))
   (setq _longitude (float __longitude))
   (setq _latitude (float __latitude))
   (setq _altitude (float __altitude))
   (setq _yamAngle (float __yamAngle))
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:satelliteNum
   (&optional __satelliteNum)
   (if __satelliteNum (setq _satelliteNum __satelliteNum)) _satelliteNum)
  (:rsltType
   (&optional __rsltType)
   (if __rsltType (setq _rsltType __rsltType)) _rsltType)
  (:longitude
   (&optional __longitude)
   (if __longitude (setq _longitude __longitude)) _longitude)
  (:latitude
   (&optional __latitude)
   (if __latitude (setq _latitude __latitude)) _latitude)
  (:altitude
   (&optional __altitude)
   (if __altitude (setq _altitude __altitude)) _altitude)
  (:yamAngle
   (&optional __yamAngle)
   (if __yamAngle (setq _yamAngle __yamAngle)) _yamAngle)
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; uint8 _satelliteNum
    1
    ;; uint8 _rsltType
    1
    ;; float64 _longitude
    8
    ;; float64 _latitude
    8
    ;; float32 _altitude
    4
    ;; float32 _yamAngle
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; uint8 _satelliteNum
       (write-byte _satelliteNum s)
     ;; uint8 _rsltType
       (write-byte _rsltType s)
     ;; float64 _longitude
       (sys::poke _longitude (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _latitude
       (sys::poke _latitude (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float32 _altitude
       (sys::poke _altitude (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _yamAngle
       (sys::poke _yamAngle (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; uint8 _satelliteNum
     (setq _satelliteNum (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _rsltType
     (setq _rsltType (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; float64 _longitude
     (setq _longitude (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _latitude
     (setq _latitude (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float32 _altitude
     (setq _altitude (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _yamAngle
     (setq _yamAngle (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get comm_msg::gpsInfo :md5sum-) "7af55e227f3750810313d5dec1b64d15")
(setf (get comm_msg::gpsInfo :datatype-) "comm_msg/gpsInfo")
(setf (get comm_msg::gpsInfo :definition-)
      "Header  header

# satellite num
uint8 satelliteNum

# Rslt Type
uint8 rsltType

# 
float64 longitude
float64 latitude
float32 altitude

#
float32 yamAngle


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



(provide :comm_msg/gpsInfo "7af55e227f3750810313d5dec1b64d15")


