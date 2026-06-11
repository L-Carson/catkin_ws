;; Auto-generated. Do not edit!


(when (boundp 'comm_srvs::LidarInstallParam)
  (if (not (find-package "COMM_SRVS"))
    (make-package "COMM_SRVS"))
  (shadow 'LidarInstallParam (find-package "COMM_SRVS")))
(unless (find-package "COMM_SRVS::LIDARINSTALLPARAM")
  (make-package "COMM_SRVS::LIDARINSTALLPARAM"))

(in-package "ROS")
;;//! \htmlinclude LidarInstallParam.msg.html


(defclass comm_srvs::LidarInstallParam
  :super ros::object
  :slots (_name _serialNum _x _y _z _roll _pitch _yaw ))

(defmethod comm_srvs::LidarInstallParam
  (:init
   (&key
    ((:name __name) "")
    ((:serialNum __serialNum) "")
    ((:x __x) 0.0)
    ((:y __y) 0.0)
    ((:z __z) 0.0)
    ((:roll __roll) 0.0)
    ((:pitch __pitch) 0.0)
    ((:yaw __yaw) 0.0)
    )
   (send-super :init)
   (setq _name (string __name))
   (setq _serialNum (string __serialNum))
   (setq _x (float __x))
   (setq _y (float __y))
   (setq _z (float __z))
   (setq _roll (float __roll))
   (setq _pitch (float __pitch))
   (setq _yaw (float __yaw))
   self)
  (:name
   (&optional __name)
   (if __name (setq _name __name)) _name)
  (:serialNum
   (&optional __serialNum)
   (if __serialNum (setq _serialNum __serialNum)) _serialNum)
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
    ;; string _name
    4 (length _name)
    ;; string _serialNum
    4 (length _serialNum)
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
     ;; string _name
       (write-long (length _name) s) (princ _name s)
     ;; string _serialNum
       (write-long (length _serialNum) s) (princ _serialNum s)
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
   ;; string _name
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _name (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; string _serialNum
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _serialNum (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
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

(setf (get comm_srvs::LidarInstallParam :md5sum-) "20c26dd3c3b4024c1be1a0626d71ff63")
(setf (get comm_srvs::LidarInstallParam :datatype-) "comm_srvs/LidarInstallParam")
(setf (get comm_srvs::LidarInstallParam :definition-)
      "string  name    #MainLidar BlindLidar BlindLidar2
string  serialNum
float32 x
float32 y
float32 z
float32 roll
float32 pitch
float32 yaw
")



(provide :comm_srvs/LidarInstallParam "20c26dd3c3b4024c1be1a0626d71ff63")


