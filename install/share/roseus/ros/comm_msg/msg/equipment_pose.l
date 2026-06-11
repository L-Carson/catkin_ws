;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::equipment_pose)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'equipment_pose (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::EQUIPMENT_POSE")
  (make-package "COMM_MSG::EQUIPMENT_POSE"))

(in-package "ROS")
;;//! \htmlinclude equipment_pose.msg.html


(defclass comm_msg::equipment_pose
  :super ros::object
  :slots (_status _posX _posY _dirAngle ))

(defmethod comm_msg::equipment_pose
  (:init
   (&key
    ((:status __status) 0)
    ((:posX __posX) 0.0)
    ((:posY __posY) 0.0)
    ((:dirAngle __dirAngle) 0.0)
    )
   (send-super :init)
   (setq _status (round __status))
   (setq _posX (float __posX))
   (setq _posY (float __posY))
   (setq _dirAngle (float __dirAngle))
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
  (:dirAngle
   (&optional __dirAngle)
   (if __dirAngle (setq _dirAngle __dirAngle)) _dirAngle)
  (:serialization-length
   ()
   (+
    ;; uint8 _status
    1
    ;; float32 _posX
    4
    ;; float32 _posY
    4
    ;; float32 _dirAngle
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
     ;; float32 _dirAngle
       (sys::poke _dirAngle (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
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
   ;; float32 _dirAngle
     (setq _dirAngle (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get comm_msg::equipment_pose :md5sum-) "6f56144f592d31a738a420cd57e47f50")
(setf (get comm_msg::equipment_pose :datatype-) "comm_msg/equipment_pose")
(setf (get comm_msg::equipment_pose :definition-)
      "# location status, 1 normal, 0 abnormal
uint8   status
float32 posX
float32 posY
float32 dirAngle
")



(provide :comm_msg/equipment_pose "6f56144f592d31a738a420cd57e47f50")


