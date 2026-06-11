;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::pose)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'pose (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::POSE")
  (make-package "COMM_MSG::POSE"))

(in-package "ROS")
;;//! \htmlinclude pose.msg.html


(defclass comm_msg::pose
  :super ros::object
  :slots (_px _py _pz _roll _pitch _yaw ))

(defmethod comm_msg::pose
  (:init
   (&key
    ((:px __px) 0.0)
    ((:py __py) 0.0)
    ((:pz __pz) 0.0)
    ((:roll __roll) 0.0)
    ((:pitch __pitch) 0.0)
    ((:yaw __yaw) 0.0)
    )
   (send-super :init)
   (setq _px (float __px))
   (setq _py (float __py))
   (setq _pz (float __pz))
   (setq _roll (float __roll))
   (setq _pitch (float __pitch))
   (setq _yaw (float __yaw))
   self)
  (:px
   (&optional __px)
   (if __px (setq _px __px)) _px)
  (:py
   (&optional __py)
   (if __py (setq _py __py)) _py)
  (:pz
   (&optional __pz)
   (if __pz (setq _pz __pz)) _pz)
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
    ;; float32 _px
    4
    ;; float32 _py
    4
    ;; float32 _pz
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
     ;; float32 _px
       (sys::poke _px (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _py
       (sys::poke _py (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _pz
       (sys::poke _pz (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
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
   ;; float32 _px
     (setq _px (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _py
     (setq _py (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _pz
     (setq _pz (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _roll
     (setq _roll (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _pitch
     (setq _pitch (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _yaw
     (setq _yaw (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get comm_msg::pose :md5sum-) "4974c257f289cfad706f1b0679063a6b")
(setf (get comm_msg::pose :datatype-) "comm_msg/pose")
(setf (get comm_msg::pose :definition-)
      "float32 px
float32 py
float32 pz
float32 roll
float32 pitch
float32 yaw
")



(provide :comm_msg/pose "4974c257f289cfad706f1b0679063a6b")


