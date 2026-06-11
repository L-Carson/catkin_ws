;; Auto-generated. Do not edit!


(when (boundp 'comm_srvs::imuOffsetCalibration)
  (if (not (find-package "COMM_SRVS"))
    (make-package "COMM_SRVS"))
  (shadow 'imuOffsetCalibration (find-package "COMM_SRVS")))
(unless (find-package "COMM_SRVS::IMUOFFSETCALIBRATION")
  (make-package "COMM_SRVS::IMUOFFSETCALIBRATION"))
(unless (find-package "COMM_SRVS::IMUOFFSETCALIBRATIONREQUEST")
  (make-package "COMM_SRVS::IMUOFFSETCALIBRATIONREQUEST"))
(unless (find-package "COMM_SRVS::IMUOFFSETCALIBRATIONRESPONSE")
  (make-package "COMM_SRVS::IMUOFFSETCALIBRATIONRESPONSE"))

(in-package "ROS")





(defclass comm_srvs::imuOffsetCalibrationRequest
  :super ros::object
  :slots (_imuOffsetParam ))

(defmethod comm_srvs::imuOffsetCalibrationRequest
  (:init
   (&key
    ((:imuOffsetParam __imuOffsetParam) (instance comm_srvs::sensorInstallParam :init))
    )
   (send-super :init)
   (setq _imuOffsetParam __imuOffsetParam)
   self)
  (:imuOffsetParam
   (&rest __imuOffsetParam)
   (if (keywordp (car __imuOffsetParam))
       (send* _imuOffsetParam __imuOffsetParam)
     (progn
       (if __imuOffsetParam (setq _imuOffsetParam (car __imuOffsetParam)))
       _imuOffsetParam)))
  (:serialization-length
   ()
   (+
    ;; comm_srvs/sensorInstallParam _imuOffsetParam
    (send _imuOffsetParam :serialization-length)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; comm_srvs/sensorInstallParam _imuOffsetParam
       (send _imuOffsetParam :serialize s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; comm_srvs/sensorInstallParam _imuOffsetParam
     (send _imuOffsetParam :deserialize buf ptr-) (incf ptr- (send _imuOffsetParam :serialization-length))
   ;;
   self)
  )

(defclass comm_srvs::imuOffsetCalibrationResponse
  :super ros::object
  :slots (_success ))

(defmethod comm_srvs::imuOffsetCalibrationResponse
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

(defclass comm_srvs::imuOffsetCalibration
  :super ros::object
  :slots ())

(setf (get comm_srvs::imuOffsetCalibration :md5sum-) "c4b920e716f4c5969027af9eec7a747b")
(setf (get comm_srvs::imuOffsetCalibration :datatype-) "comm_srvs/imuOffsetCalibration")
(setf (get comm_srvs::imuOffsetCalibration :request) comm_srvs::imuOffsetCalibrationRequest)
(setf (get comm_srvs::imuOffsetCalibration :response) comm_srvs::imuOffsetCalibrationResponse)

(defmethod comm_srvs::imuOffsetCalibrationRequest
  (:response () (instance comm_srvs::imuOffsetCalibrationResponse :init)))

(setf (get comm_srvs::imuOffsetCalibrationRequest :md5sum-) "c4b920e716f4c5969027af9eec7a747b")
(setf (get comm_srvs::imuOffsetCalibrationRequest :datatype-) "comm_srvs/imuOffsetCalibrationRequest")
(setf (get comm_srvs::imuOffsetCalibrationRequest :definition-)
      "sensorInstallParam imuOffsetParam

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

(setf (get comm_srvs::imuOffsetCalibrationResponse :md5sum-) "c4b920e716f4c5969027af9eec7a747b")
(setf (get comm_srvs::imuOffsetCalibrationResponse :datatype-) "comm_srvs/imuOffsetCalibrationResponse")
(setf (get comm_srvs::imuOffsetCalibrationResponse :definition-)
      "sensorInstallParam imuOffsetParam

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



(provide :comm_srvs/imuOffsetCalibration "c4b920e716f4c5969027af9eec7a747b")


