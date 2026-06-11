;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::wheelSpeed)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'wheelSpeed (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::WHEELSPEED")
  (make-package "COMM_MSG::WHEELSPEED"))

(in-package "ROS")
;;//! \htmlinclude wheelSpeed.msg.html


(defclass comm_msg::wheelSpeed
  :super ros::object
  :slots (_leftwheel_speed _rightwheel_speed ))

(defmethod comm_msg::wheelSpeed
  (:init
   (&key
    ((:leftwheel_speed __leftwheel_speed) 0.0)
    ((:rightwheel_speed __rightwheel_speed) 0.0)
    )
   (send-super :init)
   (setq _leftwheel_speed (float __leftwheel_speed))
   (setq _rightwheel_speed (float __rightwheel_speed))
   self)
  (:leftwheel_speed
   (&optional __leftwheel_speed)
   (if __leftwheel_speed (setq _leftwheel_speed __leftwheel_speed)) _leftwheel_speed)
  (:rightwheel_speed
   (&optional __rightwheel_speed)
   (if __rightwheel_speed (setq _rightwheel_speed __rightwheel_speed)) _rightwheel_speed)
  (:serialization-length
   ()
   (+
    ;; float32 _leftwheel_speed
    4
    ;; float32 _rightwheel_speed
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float32 _leftwheel_speed
       (sys::poke _leftwheel_speed (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _rightwheel_speed
       (sys::poke _rightwheel_speed (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float32 _leftwheel_speed
     (setq _leftwheel_speed (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _rightwheel_speed
     (setq _rightwheel_speed (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get comm_msg::wheelSpeed :md5sum-) "a9d95faf9204c76e50e85451b2bd65f4")
(setf (get comm_msg::wheelSpeed :datatype-) "comm_msg/wheelSpeed")
(setf (get comm_msg::wheelSpeed :definition-)
      "float32 leftwheel_speed
float32 rightwheel_speed

")



(provide :comm_msg/wheelSpeed "a9d95faf9204c76e50e85451b2bd65f4")


