;; Auto-generated. Do not edit!


(when (boundp 'comm_srvs::lidarCalibration)
  (if (not (find-package "COMM_SRVS"))
    (make-package "COMM_SRVS"))
  (shadow 'lidarCalibration (find-package "COMM_SRVS")))
(unless (find-package "COMM_SRVS::LIDARCALIBRATION")
  (make-package "COMM_SRVS::LIDARCALIBRATION"))
(unless (find-package "COMM_SRVS::LIDARCALIBRATIONREQUEST")
  (make-package "COMM_SRVS::LIDARCALIBRATIONREQUEST"))
(unless (find-package "COMM_SRVS::LIDARCALIBRATIONRESPONSE")
  (make-package "COMM_SRVS::LIDARCALIBRATIONRESPONSE"))

(in-package "ROS")





(defclass comm_srvs::lidarCalibrationRequest
  :super ros::object
  :slots (_lidarsClbtParam ))

(defmethod comm_srvs::lidarCalibrationRequest
  (:init
   (&key
    ((:lidarsClbtParam __lidarsClbtParam) ())
    )
   (send-super :init)
   (setq _lidarsClbtParam __lidarsClbtParam)
   self)
  (:lidarsClbtParam
   (&rest __lidarsClbtParam)
   (if (keywordp (car __lidarsClbtParam))
       (send* _lidarsClbtParam __lidarsClbtParam)
     (progn
       (if __lidarsClbtParam (setq _lidarsClbtParam (car __lidarsClbtParam)))
       _lidarsClbtParam)))
  (:serialization-length
   ()
   (+
    ;; comm_srvs/LidarInstallParam[] _lidarsClbtParam
    (apply #'+ (send-all _lidarsClbtParam :serialization-length)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; comm_srvs/LidarInstallParam[] _lidarsClbtParam
     (write-long (length _lidarsClbtParam) s)
     (dolist (elem _lidarsClbtParam)
       (send elem :serialize s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; comm_srvs/LidarInstallParam[] _lidarsClbtParam
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _lidarsClbtParam (let (r) (dotimes (i n) (push (instance comm_srvs::LidarInstallParam :init) r)) r))
     (dolist (elem- _lidarsClbtParam)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;;
   self)
  )

(defclass comm_srvs::lidarCalibrationResponse
  :super ros::object
  :slots (_success ))

(defmethod comm_srvs::lidarCalibrationResponse
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

(defclass comm_srvs::lidarCalibration
  :super ros::object
  :slots ())

(setf (get comm_srvs::lidarCalibration :md5sum-) "a672746d2ead307649fd58de19b13665")
(setf (get comm_srvs::lidarCalibration :datatype-) "comm_srvs/lidarCalibration")
(setf (get comm_srvs::lidarCalibration :request) comm_srvs::lidarCalibrationRequest)
(setf (get comm_srvs::lidarCalibration :response) comm_srvs::lidarCalibrationResponse)

(defmethod comm_srvs::lidarCalibrationRequest
  (:response () (instance comm_srvs::lidarCalibrationResponse :init)))

(setf (get comm_srvs::lidarCalibrationRequest :md5sum-) "a672746d2ead307649fd58de19b13665")
(setf (get comm_srvs::lidarCalibrationRequest :datatype-) "comm_srvs/lidarCalibrationRequest")
(setf (get comm_srvs::lidarCalibrationRequest :definition-)
      "LidarInstallParam[] lidarsClbtParam

================================================================================
MSG: comm_srvs/LidarInstallParam
string  name    #MainLidar BlindLidar BlindLidar2
string  serialNum
float32 x
float32 y
float32 z
float32 roll
float32 pitch
---
bool        success

")

(setf (get comm_srvs::lidarCalibrationResponse :md5sum-) "a672746d2ead307649fd58de19b13665")
(setf (get comm_srvs::lidarCalibrationResponse :datatype-) "comm_srvs/lidarCalibrationResponse")
(setf (get comm_srvs::lidarCalibrationResponse :definition-)
      "LidarInstallParam[] lidarsClbtParam

================================================================================
MSG: comm_srvs/LidarInstallParam
string  name    #MainLidar BlindLidar BlindLidar2
string  serialNum
float32 x
float32 y
float32 z
float32 roll
float32 pitch
---
bool        success

")



(provide :comm_srvs/lidarCalibration "a672746d2ead307649fd58de19b13665")


