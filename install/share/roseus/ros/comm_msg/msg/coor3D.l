;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::coor3D)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'coor3D (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::COOR3D")
  (make-package "COMM_MSG::COOR3D"))

(in-package "ROS")
;;//! \htmlinclude coor3D.msg.html


(defclass comm_msg::coor3D
  :super ros::object
  :slots (_pos_x _pos_y _pos_z ))

(defmethod comm_msg::coor3D
  (:init
   (&key
    ((:pos_x __pos_x) 0.0)
    ((:pos_y __pos_y) 0.0)
    ((:pos_z __pos_z) 0.0)
    )
   (send-super :init)
   (setq _pos_x (float __pos_x))
   (setq _pos_y (float __pos_y))
   (setq _pos_z (float __pos_z))
   self)
  (:pos_x
   (&optional __pos_x)
   (if __pos_x (setq _pos_x __pos_x)) _pos_x)
  (:pos_y
   (&optional __pos_y)
   (if __pos_y (setq _pos_y __pos_y)) _pos_y)
  (:pos_z
   (&optional __pos_z)
   (if __pos_z (setq _pos_z __pos_z)) _pos_z)
  (:serialization-length
   ()
   (+
    ;; float32 _pos_x
    4
    ;; float32 _pos_y
    4
    ;; float32 _pos_z
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float32 _pos_x
       (sys::poke _pos_x (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _pos_y
       (sys::poke _pos_y (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _pos_z
       (sys::poke _pos_z (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float32 _pos_x
     (setq _pos_x (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _pos_y
     (setq _pos_y (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _pos_z
     (setq _pos_z (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get comm_msg::coor3D :md5sum-) "03225cff21da80c8dcb47075eac84a6b")
(setf (get comm_msg::coor3D :datatype-) "comm_msg/coor3D")
(setf (get comm_msg::coor3D :definition-)
      "float32 pos_x
float32 pos_y
float32 pos_z
")



(provide :comm_msg/coor3D "03225cff21da80c8dcb47075eac84a6b")


