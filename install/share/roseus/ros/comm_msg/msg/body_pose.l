;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::body_pose)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'body_pose (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::BODY_POSE")
  (make-package "COMM_MSG::BODY_POSE"))

(in-package "ROS")
;;//! \htmlinclude body_pose.msg.html


(defclass comm_msg::body_pose
  :super ros::object
  :slots (_body_yaw _backWheel_x _backWheel_y _frontWheel_x _frontWheel_y _alongEdgeRefPoint_x _alongEdgeRefPoint_y ))

(defmethod comm_msg::body_pose
  (:init
   (&key
    ((:body_yaw __body_yaw) 0.0)
    ((:backWheel_x __backWheel_x) 0.0)
    ((:backWheel_y __backWheel_y) 0.0)
    ((:frontWheel_x __frontWheel_x) 0.0)
    ((:frontWheel_y __frontWheel_y) 0.0)
    ((:alongEdgeRefPoint_x __alongEdgeRefPoint_x) 0.0)
    ((:alongEdgeRefPoint_y __alongEdgeRefPoint_y) 0.0)
    )
   (send-super :init)
   (setq _body_yaw (float __body_yaw))
   (setq _backWheel_x (float __backWheel_x))
   (setq _backWheel_y (float __backWheel_y))
   (setq _frontWheel_x (float __frontWheel_x))
   (setq _frontWheel_y (float __frontWheel_y))
   (setq _alongEdgeRefPoint_x (float __alongEdgeRefPoint_x))
   (setq _alongEdgeRefPoint_y (float __alongEdgeRefPoint_y))
   self)
  (:body_yaw
   (&optional __body_yaw)
   (if __body_yaw (setq _body_yaw __body_yaw)) _body_yaw)
  (:backWheel_x
   (&optional __backWheel_x)
   (if __backWheel_x (setq _backWheel_x __backWheel_x)) _backWheel_x)
  (:backWheel_y
   (&optional __backWheel_y)
   (if __backWheel_y (setq _backWheel_y __backWheel_y)) _backWheel_y)
  (:frontWheel_x
   (&optional __frontWheel_x)
   (if __frontWheel_x (setq _frontWheel_x __frontWheel_x)) _frontWheel_x)
  (:frontWheel_y
   (&optional __frontWheel_y)
   (if __frontWheel_y (setq _frontWheel_y __frontWheel_y)) _frontWheel_y)
  (:alongEdgeRefPoint_x
   (&optional __alongEdgeRefPoint_x)
   (if __alongEdgeRefPoint_x (setq _alongEdgeRefPoint_x __alongEdgeRefPoint_x)) _alongEdgeRefPoint_x)
  (:alongEdgeRefPoint_y
   (&optional __alongEdgeRefPoint_y)
   (if __alongEdgeRefPoint_y (setq _alongEdgeRefPoint_y __alongEdgeRefPoint_y)) _alongEdgeRefPoint_y)
  (:serialization-length
   ()
   (+
    ;; float32 _body_yaw
    4
    ;; float32 _backWheel_x
    4
    ;; float32 _backWheel_y
    4
    ;; float32 _frontWheel_x
    4
    ;; float32 _frontWheel_y
    4
    ;; float32 _alongEdgeRefPoint_x
    4
    ;; float32 _alongEdgeRefPoint_y
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float32 _body_yaw
       (sys::poke _body_yaw (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _backWheel_x
       (sys::poke _backWheel_x (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _backWheel_y
       (sys::poke _backWheel_y (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _frontWheel_x
       (sys::poke _frontWheel_x (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _frontWheel_y
       (sys::poke _frontWheel_y (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _alongEdgeRefPoint_x
       (sys::poke _alongEdgeRefPoint_x (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _alongEdgeRefPoint_y
       (sys::poke _alongEdgeRefPoint_y (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float32 _body_yaw
     (setq _body_yaw (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _backWheel_x
     (setq _backWheel_x (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _backWheel_y
     (setq _backWheel_y (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _frontWheel_x
     (setq _frontWheel_x (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _frontWheel_y
     (setq _frontWheel_y (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _alongEdgeRefPoint_x
     (setq _alongEdgeRefPoint_x (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _alongEdgeRefPoint_y
     (setq _alongEdgeRefPoint_y (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get comm_msg::body_pose :md5sum-) "f09536d45d4aed5f09a8a01687e32257")
(setf (get comm_msg::body_pose :datatype-) "comm_msg/body_pose")
(setf (get comm_msg::body_pose :definition-)
      "float32 body_yaw
float32 backWheel_x
float32 backWheel_y
float32 frontWheel_x
float32 frontWheel_y
float32 alongEdgeRefPoint_x
float32 alongEdgeRefPoint_y
")



(provide :comm_msg/body_pose "f09536d45d4aed5f09a8a01687e32257")


