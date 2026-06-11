;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::simulation_circular_obst)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'simulation_circular_obst (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::SIMULATION_CIRCULAR_OBST")
  (make-package "COMM_MSG::SIMULATION_CIRCULAR_OBST"))

(in-package "ROS")
;;//! \htmlinclude simulation_circular_obst.msg.html


(defclass comm_msg::simulation_circular_obst
  :super ros::object
  :slots (_x _y _radius ))

(defmethod comm_msg::simulation_circular_obst
  (:init
   (&key
    ((:x __x) 0.0)
    ((:y __y) 0.0)
    ((:radius __radius) 0.0)
    )
   (send-super :init)
   (setq _x (float __x))
   (setq _y (float __y))
   (setq _radius (float __radius))
   self)
  (:x
   (&optional __x)
   (if __x (setq _x __x)) _x)
  (:y
   (&optional __y)
   (if __y (setq _y __y)) _y)
  (:radius
   (&optional __radius)
   (if __radius (setq _radius __radius)) _radius)
  (:serialization-length
   ()
   (+
    ;; float32 _x
    4
    ;; float32 _y
    4
    ;; float32 _radius
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float32 _x
       (sys::poke _x (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _y
       (sys::poke _y (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _radius
       (sys::poke _radius (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float32 _x
     (setq _x (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _y
     (setq _y (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _radius
     (setq _radius (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get comm_msg::simulation_circular_obst :md5sum-) "816ab4505c541cc7c042e75523ad2290")
(setf (get comm_msg::simulation_circular_obst :datatype-) "comm_msg/simulation_circular_obst")
(setf (get comm_msg::simulation_circular_obst :definition-)
      "float32 x
float32 y
float32 radius
")



(provide :comm_msg/simulation_circular_obst "816ab4505c541cc7c042e75523ad2290")


