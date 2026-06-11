;; Auto-generated. Do not edit!


(when (boundp 'comm_srvs::lidarOffsetCalibration)
  (if (not (find-package "COMM_SRVS"))
    (make-package "COMM_SRVS"))
  (shadow 'lidarOffsetCalibration (find-package "COMM_SRVS")))
(unless (find-package "COMM_SRVS::LIDAROFFSETCALIBRATION")
  (make-package "COMM_SRVS::LIDAROFFSETCALIBRATION"))
(unless (find-package "COMM_SRVS::LIDAROFFSETCALIBRATIONREQUEST")
  (make-package "COMM_SRVS::LIDAROFFSETCALIBRATIONREQUEST"))
(unless (find-package "COMM_SRVS::LIDAROFFSETCALIBRATIONRESPONSE")
  (make-package "COMM_SRVS::LIDAROFFSETCALIBRATIONRESPONSE"))

(in-package "ROS")





(defclass comm_srvs::lidarOffsetCalibrationRequest
  :super ros::object
  :slots (_lidarOffsetParam ))

(defmethod comm_srvs::lidarOffsetCalibrationRequest
  (:init
   (&key
    ((:lidarOffsetParam __lidarOffsetParam) (instance comm_srvs::sensorInstallParam :init))
    )
   (send-super :init)
   (setq _lidarOffsetParam __lidarOffsetParam)
   self)
  (:lidarOffsetParam
   (&rest __lidarOffsetParam)
   (if (keywordp (car __lidarOffsetParam))
       (send* _lidarOffsetParam __lidarOffsetParam)
     (progn
       (if __lidarOffsetParam (setq _lidarOffsetParam (car __lidarOffsetParam)))
       _lidarOffsetParam)))
  (:serialization-length
   ()
   (+
    ;; comm_srvs/sensorInstallParam _lidarOffsetParam
    (send _lidarOffsetParam :serialization-length)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; comm_srvs/sensorInstallParam _lidarOffsetParam
       (send _lidarOffsetParam :serialize s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; comm_srvs/sensorInstallParam _lidarOffsetParam
     (send _lidarOffsetParam :deserialize buf ptr-) (incf ptr- (send _lidarOffsetParam :serialization-length))
   ;;
   self)
  )

(defclass comm_srvs::lidarOffsetCalibrationResponse
  :super ros::object
  :slots (_success ))

(defmethod comm_srvs::lidarOffsetCalibrationResponse
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

(defclass comm_srvs::lidarOffsetCalibration
  :super ros::object
  :slots ())

(setf (get comm_srvs::lidarOffsetCalibration :md5sum-) "c4589f9ccc2d3ea9fba7e7bffbeb08d8")
(setf (get comm_srvs::lidarOffsetCalibration :datatype-) "comm_srvs/lidarOffsetCalibration")
(setf (get comm_srvs::lidarOffsetCalibration :request) comm_srvs::lidarOffsetCalibrationRequest)
(setf (get comm_srvs::lidarOffsetCalibration :response) comm_srvs::lidarOffsetCalibrationResponse)

(defmethod comm_srvs::lidarOffsetCalibrationRequest
  (:response () (instance comm_srvs::lidarOffsetCalibrationResponse :init)))

(setf (get comm_srvs::lidarOffsetCalibrationRequest :md5sum-) "c4589f9ccc2d3ea9fba7e7bffbeb08d8")
(setf (get comm_srvs::lidarOffsetCalibrationRequest :datatype-) "comm_srvs/lidarOffsetCalibrationRequest")
(setf (get comm_srvs::lidarOffsetCalibrationRequest :definition-)
      "sensorInstallParam lidarOffsetParam

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

(setf (get comm_srvs::lidarOffsetCalibrationResponse :md5sum-) "c4589f9ccc2d3ea9fba7e7bffbeb08d8")
(setf (get comm_srvs::lidarOffsetCalibrationResponse :datatype-) "comm_srvs/lidarOffsetCalibrationResponse")
(setf (get comm_srvs::lidarOffsetCalibrationResponse :definition-)
      "sensorInstallParam lidarOffsetParam

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



(provide :comm_srvs/lidarOffsetCalibration "c4589f9ccc2d3ea9fba7e7bffbeb08d8")


