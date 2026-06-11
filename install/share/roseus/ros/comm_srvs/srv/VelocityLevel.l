;; Auto-generated. Do not edit!


(when (boundp 'comm_srvs::VelocityLevel)
  (if (not (find-package "COMM_SRVS"))
    (make-package "COMM_SRVS"))
  (shadow 'VelocityLevel (find-package "COMM_SRVS")))
(unless (find-package "COMM_SRVS::VELOCITYLEVEL")
  (make-package "COMM_SRVS::VELOCITYLEVEL"))
(unless (find-package "COMM_SRVS::VELOCITYLEVELREQUEST")
  (make-package "COMM_SRVS::VELOCITYLEVELREQUEST"))
(unless (find-package "COMM_SRVS::VELOCITYLEVELRESPONSE")
  (make-package "COMM_SRVS::VELOCITYLEVELRESPONSE"))

(in-package "ROS")





(defclass comm_srvs::VelocityLevelRequest
  :super ros::object
  :slots (_level ))

(defmethod comm_srvs::VelocityLevelRequest
  (:init
   (&key
    ((:level __level) 0)
    )
   (send-super :init)
   (setq _level (round __level))
   self)
  (:level
   (&optional __level)
   (if __level (setq _level __level)) _level)
  (:serialization-length
   ()
   (+
    ;; int32 _level
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int32 _level
       (write-long _level s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int32 _level
     (setq _level (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;;
   self)
  )

(defclass comm_srvs::VelocityLevelResponse
  :super ros::object
  :slots (_success ))

(defmethod comm_srvs::VelocityLevelResponse
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

(defclass comm_srvs::VelocityLevel
  :super ros::object
  :slots ())

(setf (get comm_srvs::VelocityLevel :md5sum-) "442ff6aec1dc9bb7000abb97372a7b4d")
(setf (get comm_srvs::VelocityLevel :datatype-) "comm_srvs/VelocityLevel")
(setf (get comm_srvs::VelocityLevel :request) comm_srvs::VelocityLevelRequest)
(setf (get comm_srvs::VelocityLevel :response) comm_srvs::VelocityLevelResponse)

(defmethod comm_srvs::VelocityLevelRequest
  (:response () (instance comm_srvs::VelocityLevelResponse :init)))

(setf (get comm_srvs::VelocityLevelRequest :md5sum-) "442ff6aec1dc9bb7000abb97372a7b4d")
(setf (get comm_srvs::VelocityLevelRequest :datatype-) "comm_srvs/VelocityLevelRequest")
(setf (get comm_srvs::VelocityLevelRequest :definition-)
      "int32   level
---
bool    success
")

(setf (get comm_srvs::VelocityLevelResponse :md5sum-) "442ff6aec1dc9bb7000abb97372a7b4d")
(setf (get comm_srvs::VelocityLevelResponse :datatype-) "comm_srvs/VelocityLevelResponse")
(setf (get comm_srvs::VelocityLevelResponse :definition-)
      "int32   level
---
bool    success
")



(provide :comm_srvs/VelocityLevel "442ff6aec1dc9bb7000abb97372a7b4d")


