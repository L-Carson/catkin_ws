;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::pose3D)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'pose3D (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::POSE3D")
  (make-package "COMM_MSG::POSE3D"))

(in-package "ROS")
;;//! \htmlinclude pose3D.msg.html


(defclass comm_msg::pose3D
  :super ros::object
  :slots (_posX _posY _posZ _roll _pitch _yaw ))

(defmethod comm_msg::pose3D
  (:init
   (&key
    ((:posX __posX) 0.0)
    ((:posY __posY) 0.0)
    ((:posZ __posZ) 0.0)
    ((:roll __roll) 0.0)
    ((:pitch __pitch) 0.0)
    ((:yaw __yaw) 0.0)
    )
   (send-super :init)
   (setq _posX (float __posX))
   (setq _posY (float __posY))
   (setq _posZ (float __posZ))
   (setq _roll (float __roll))
   (setq _pitch (float __pitch))
   (setq _yaw (float __yaw))
   self)
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

(setf (get comm_msg::pose3D :md5sum-) "b0f6a63f05aca4ae94f3fa6431f6fc26")
(setf (get comm_msg::pose3D :datatype-) "comm_msg/pose3D")
(setf (get comm_msg::pose3D :definition-)
      "float32 posX
float32 posY
float32 posZ
float32 roll
float32 pitch
float32 yaw
")



(provide :comm_msg/pose3D "b0f6a63f05aca4ae94f3fa6431f6fc26")


