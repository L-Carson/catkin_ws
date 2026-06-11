;; Auto-generated. Do not edit!


(when (boundp 'comm_srvs::InitPose)
  (if (not (find-package "COMM_SRVS"))
    (make-package "COMM_SRVS"))
  (shadow 'InitPose (find-package "COMM_SRVS")))
(unless (find-package "COMM_SRVS::INITPOSE")
  (make-package "COMM_SRVS::INITPOSE"))
(unless (find-package "COMM_SRVS::INITPOSEREQUEST")
  (make-package "COMM_SRVS::INITPOSEREQUEST"))
(unless (find-package "COMM_SRVS::INITPOSERESPONSE")
  (make-package "COMM_SRVS::INITPOSERESPONSE"))

(in-package "ROS")





(defclass comm_srvs::InitPoseRequest
  :super ros::object
  :slots (_x _y _yaw ))

(defmethod comm_srvs::InitPoseRequest
  (:init
   (&key
    ((:x __x) 0.0)
    ((:y __y) 0.0)
    ((:yaw __yaw) 0.0)
    )
   (send-super :init)
   (setq _x (float __x))
   (setq _y (float __y))
   (setq _yaw (float __yaw))
   self)
  (:x
   (&optional __x)
   (if __x (setq _x __x)) _x)
  (:y
   (&optional __y)
   (if __y (setq _y __y)) _y)
  (:yaw
   (&optional __yaw)
   (if __yaw (setq _yaw __yaw)) _yaw)
  (:serialization-length
   ()
   (+
    ;; float32 _x
    4
    ;; float32 _y
    4
    ;; float32 _yaw
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
     ;; float32 _yaw
       (sys::poke _yaw (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float32 _x
     (setq _x (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _y
     (setq _y (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _yaw
     (setq _yaw (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(defclass comm_srvs::InitPoseResponse
  :super ros::object
  :slots (_success ))

(defmethod comm_srvs::InitPoseResponse
  (:init
   (&key
    ((:success __success) nil)
    )
   (send-super :init)
   (setq _success __success)
   self)
  (:success
   (&optional (__success :null))
   (if (not (eq __success :null)) (setq _success __success)) _success)
  (:serialization-length
   ()
   (+
    ;; bool _success
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; bool _success
       (if _success (write-byte -1 s) (write-byte 0 s))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; bool _success
     (setq _success (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;;
   self)
  )

(defclass comm_srvs::InitPose
  :super ros::object
  :slots ())

(setf (get comm_srvs::InitPose :md5sum-) "4ea516c50b8e2b735013ac031a10cc9d")
(setf (get comm_srvs::InitPose :datatype-) "comm_srvs/InitPose")
(setf (get comm_srvs::InitPose :request) comm_srvs::InitPoseRequest)
(setf (get comm_srvs::InitPose :response) comm_srvs::InitPoseResponse)

(defmethod comm_srvs::InitPoseRequest
  (:response () (instance comm_srvs::InitPoseResponse :init)))

(setf (get comm_srvs::InitPoseRequest :md5sum-) "4ea516c50b8e2b735013ac031a10cc9d")
(setf (get comm_srvs::InitPoseRequest :datatype-) "comm_srvs/InitPoseRequest")
(setf (get comm_srvs::InitPoseRequest :definition-)
      "float32 x
float32 y
float32 yaw
---
bool    success
")

(setf (get comm_srvs::InitPoseResponse :md5sum-) "4ea516c50b8e2b735013ac031a10cc9d")
(setf (get comm_srvs::InitPoseResponse :datatype-) "comm_srvs/InitPoseResponse")
(setf (get comm_srvs::InitPoseResponse :definition-)
      "float32 x
float32 y
float32 yaw
---
bool    success
")



(provide :comm_srvs/InitPose "4ea516c50b8e2b735013ac031a10cc9d")


