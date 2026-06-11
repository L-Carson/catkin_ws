;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::pose2D)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'pose2D (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::POSE2D")
  (make-package "COMM_MSG::POSE2D"))

(in-package "ROS")
;;//! \htmlinclude pose2D.msg.html


(defclass comm_msg::pose2D
  :super ros::object
  :slots (_status _posX _posY _yaw ))

(defmethod comm_msg::pose2D
  (:init
   (&key
    ((:status __status) 0)
    ((:posX __posX) 0.0)
    ((:posY __posY) 0.0)
    ((:yaw __yaw) 0.0)
    )
   (send-super :init)
   (setq _status (round __status))
   (setq _posX (float __posX))
   (setq _posY (float __posY))
   (setq _yaw (float __yaw))
   self)
  (:status
   (&optional __status)
   (if __status (setq _status __status)) _status)
  (:posX
   (&optional __posX)
   (if __posX (setq _posX __posX)) _posX)
  (:posY
   (&optional __posY)
   (if __posY (setq _posY __posY)) _posY)
  (:yaw
   (&optional __yaw)
   (if __yaw (setq _yaw __yaw)) _yaw)
  (:serialization-length
   ()
   (+
    ;; uint8 _status
    1
    ;; float32 _posX
    4
    ;; float32 _posY
    4
    ;; float32 _yaw
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint8 _status
       (write-byte _status s)
     ;; float32 _posX
       (sys::poke _posX (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _posY
       (sys::poke _posY (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _yaw
       (sys::poke _yaw (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint8 _status
     (setq _status (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; float32 _posX
     (setq _posX (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _posY
     (setq _posY (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _yaw
     (setq _yaw (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get comm_msg::pose2D :md5sum-) "b5e8e1e1f83dbc0b558de78ae9d42c33")
(setf (get comm_msg::pose2D :datatype-) "comm_msg/pose2D")
(setf (get comm_msg::pose2D :definition-)
      "uint8   status
float32 posX
float32 posY
float32 yaw
")



(provide :comm_msg/pose2D "b5e8e1e1f83dbc0b558de78ae9d42c33")


