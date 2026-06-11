;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::roborExtriParam)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'roborExtriParam (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::ROBOREXTRIPARAM")
  (make-package "COMM_MSG::ROBOREXTRIPARAM"))

(in-package "ROS")
;;//! \htmlinclude roborExtriParam.msg.html


(defclass comm_msg::roborExtriParam
  :super ros::object
  :slots (_gnss _imu _lidar ))

(defmethod comm_msg::roborExtriParam
  (:init
   (&key
    ((:gnss __gnss) (instance comm_msg::sensorExtriParam :init))
    ((:imu __imu) (instance comm_msg::sensorExtriParam :init))
    ((:lidar __lidar) (instance comm_msg::sensorExtriParam :init))
    )
   (send-super :init)
   (setq _gnss __gnss)
   (setq _imu __imu)
   (setq _lidar __lidar)
   self)
  (:gnss
   (&rest __gnss)
   (if (keywordp (car __gnss))
       (send* _gnss __gnss)
     (progn
       (if __gnss (setq _gnss (car __gnss)))
       _gnss)))
  (:imu
   (&rest __imu)
   (if (keywordp (car __imu))
       (send* _imu __imu)
     (progn
       (if __imu (setq _imu (car __imu)))
       _imu)))
  (:lidar
   (&rest __lidar)
   (if (keywordp (car __lidar))
       (send* _lidar __lidar)
     (progn
       (if __lidar (setq _lidar (car __lidar)))
       _lidar)))
  (:serialization-length
   ()
   (+
    ;; comm_msg/sensorExtriParam _gnss
    (send _gnss :serialization-length)
    ;; comm_msg/sensorExtriParam _imu
    (send _imu :serialization-length)
    ;; comm_msg/sensorExtriParam _lidar
    (send _lidar :serialization-length)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; comm_msg/sensorExtriParam _gnss
       (send _gnss :serialize s)
     ;; comm_msg/sensorExtriParam _imu
       (send _imu :serialize s)
     ;; comm_msg/sensorExtriParam _lidar
       (send _lidar :serialize s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; comm_msg/sensorExtriParam _gnss
     (send _gnss :deserialize buf ptr-) (incf ptr- (send _gnss :serialization-length))
   ;; comm_msg/sensorExtriParam _imu
     (send _imu :deserialize buf ptr-) (incf ptr- (send _imu :serialization-length))
   ;; comm_msg/sensorExtriParam _lidar
     (send _lidar :deserialize buf ptr-) (incf ptr- (send _lidar :serialization-length))
   ;;
   self)
  )

(setf (get comm_msg::roborExtriParam :md5sum-) "ad756381b7e6a7c7a09b59fa09842fbd")
(setf (get comm_msg::roborExtriParam :datatype-) "comm_msg/roborExtriParam")
(setf (get comm_msg::roborExtriParam :definition-)
      "sensorExtriParam gnss
sensorExtriParam imu
sensorExtriParam lidar
================================================================================
MSG: comm_msg/sensorExtriParam
bool isSupport 
float32 x
float32 y
float32 z
float32 roll
float32 pitch
float32 yaw
")



(provide :comm_msg/roborExtriParam "ad756381b7e6a7c7a09b59fa09842fbd")


