;; Auto-generated. Do not edit!


(when (boundp 'comm_srvs::imuCalibration)
  (if (not (find-package "COMM_SRVS"))
    (make-package "COMM_SRVS"))
  (shadow 'imuCalibration (find-package "COMM_SRVS")))
(unless (find-package "COMM_SRVS::IMUCALIBRATION")
  (make-package "COMM_SRVS::IMUCALIBRATION"))
(unless (find-package "COMM_SRVS::IMUCALIBRATIONREQUEST")
  (make-package "COMM_SRVS::IMUCALIBRATIONREQUEST"))
(unless (find-package "COMM_SRVS::IMUCALIBRATIONRESPONSE")
  (make-package "COMM_SRVS::IMUCALIBRATIONRESPONSE"))

(in-package "ROS")





(defclass comm_srvs::imuCalibrationRequest
  :super ros::object
  :slots (_imuClbtParam ))

(defmethod comm_srvs::imuCalibrationRequest
  (:init
   (&key
    ((:imuClbtParam __imuClbtParam) (instance comm_srvs::sensorInstallParam :init))
    )
   (send-super :init)
   (setq _imuClbtParam __imuClbtParam)
   self)
  (:imuClbtParam
   (&rest __imuClbtParam)
   (if (keywordp (car __imuClbtParam))
       (send* _imuClbtParam __imuClbtParam)
     (progn
       (if __imuClbtParam (setq _imuClbtParam (car __imuClbtParam)))
       _imuClbtParam)))
  (:serialization-length
   ()
   (+
    ;; comm_srvs/sensorInstallParam _imuClbtParam
    (send _imuClbtParam :serialization-length)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; comm_srvs/sensorInstallParam _imuClbtParam
       (send _imuClbtParam :serialize s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; comm_srvs/sensorInstallParam _imuClbtParam
     (send _imuClbtParam :deserialize buf ptr-) (incf ptr- (send _imuClbtParam :serialization-length))
   ;;
   self)
  )

(defclass comm_srvs::imuCalibrationResponse
  :super ros::object
  :slots (_success ))

(defmethod comm_srvs::imuCalibrationResponse
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

(defclass comm_srvs::imuCalibration
  :super ros::object
  :slots ())

(setf (get comm_srvs::imuCalibration :md5sum-) "bf5a50f919bcad97898b19cc4dfd4322")
(setf (get comm_srvs::imuCalibration :datatype-) "comm_srvs/imuCalibration")
(setf (get comm_srvs::imuCalibration :request) comm_srvs::imuCalibrationRequest)
(setf (get comm_srvs::imuCalibration :response) comm_srvs::imuCalibrationResponse)

(defmethod comm_srvs::imuCalibrationRequest
  (:response () (instance comm_srvs::imuCalibrationResponse :init)))

(setf (get comm_srvs::imuCalibrationRequest :md5sum-) "bf5a50f919bcad97898b19cc4dfd4322")
(setf (get comm_srvs::imuCalibrationRequest :datatype-) "comm_srvs/imuCalibrationRequest")
(setf (get comm_srvs::imuCalibrationRequest :definition-)
      "sensorInstallParam imuClbtParam

================================================================================
MSG: comm_srvs/sensorInstallParam
string  name 
float32 x
float32 y
float32 z
float32 roll
float32 pitch
---
bool        success


")

(setf (get comm_srvs::imuCalibrationResponse :md5sum-) "bf5a50f919bcad97898b19cc4dfd4322")
(setf (get comm_srvs::imuCalibrationResponse :datatype-) "comm_srvs/imuCalibrationResponse")
(setf (get comm_srvs::imuCalibrationResponse :definition-)
      "sensorInstallParam imuClbtParam

================================================================================
MSG: comm_srvs/sensorInstallParam
string  name 
float32 x
float32 y
float32 z
float32 roll
float32 pitch
---
bool        success


")



(provide :comm_srvs/imuCalibration "bf5a50f919bcad97898b19cc4dfd4322")


