;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::locationInfo)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'locationInfo (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::LOCATIONINFO")
  (make-package "COMM_MSG::LOCATIONINFO"))

(in-package "ROS")
;;//! \htmlinclude locationInfo.msg.html


(defclass comm_msg::locationInfo
  :super ros::object
  :slots (_status _subMapNo _posX _posY _dirAngle ))

(defmethod comm_msg::locationInfo
  (:init
   (&key
    ((:status __status) 0)
    ((:subMapNo __subMapNo) 0)
    ((:posX __posX) 0.0)
    ((:posY __posY) 0.0)
    ((:dirAngle __dirAngle) 0.0)
    )
   (send-super :init)
   (setq _status (round __status))
   (setq _subMapNo (round __subMapNo))
   (setq _posX (float __posX))
   (setq _posY (float __posY))
   (setq _dirAngle (float __dirAngle))
   self)
  (:status
   (&optional __status)
   (if __status (setq _status __status)) _status)
  (:subMapNo
   (&optional __subMapNo)
   (if __subMapNo (setq _subMapNo __subMapNo)) _subMapNo)
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
    ;; int32 _status
    4
    ;; int32 _subMapNo
    4
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
     ;; int32 _status
       (write-long _status s)
     ;; int32 _subMapNo
       (write-long _subMapNo s)
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
   ;; int32 _status
     (setq _status (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; int32 _subMapNo
     (setq _subMapNo (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; float32 _posX
     (setq _posX (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _posY
     (setq _posY (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _dirAngle
     (setq _dirAngle (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get comm_msg::locationInfo :md5sum-) "b3a48261c7b0aa3907b3479915747d21")
(setf (get comm_msg::locationInfo :datatype-) "comm_msg/locationInfo")
(setf (get comm_msg::locationInfo :definition-)
      "# location status, refer to MAP_NODE_STATE_ENUM
int32 status

# sub map no, this is valid when >= 0, if -ing status, this is the new sub map no
int32 subMapNo

# Comb Map pose, which is valid When status is > 0.
float32 posX
float32 posY
float32 dirAngle
")



(provide :comm_msg/locationInfo "b3a48261c7b0aa3907b3479915747d21")


