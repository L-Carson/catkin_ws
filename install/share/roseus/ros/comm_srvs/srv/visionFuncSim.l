;; Auto-generated. Do not edit!


(when (boundp 'comm_srvs::visionFuncSim)
  (if (not (find-package "COMM_SRVS"))
    (make-package "COMM_SRVS"))
  (shadow 'visionFuncSim (find-package "COMM_SRVS")))
(unless (find-package "COMM_SRVS::VISIONFUNCSIM")
  (make-package "COMM_SRVS::VISIONFUNCSIM"))
(unless (find-package "COMM_SRVS::VISIONFUNCSIMREQUEST")
  (make-package "COMM_SRVS::VISIONFUNCSIMREQUEST"))
(unless (find-package "COMM_SRVS::VISIONFUNCSIMRESPONSE")
  (make-package "COMM_SRVS::VISIONFUNCSIMRESPONSE"))

(in-package "ROS")





(defclass comm_srvs::visionFuncSimRequest
  :super ros::object
  :slots (_qrCodeId _funcId _isUseIpu _hz _camera ))

(defmethod comm_srvs::visionFuncSimRequest
  (:init
   (&key
    ((:qrCodeId __qrCodeId) 0)
    ((:funcId __funcId) 0)
    ((:isUseIpu __isUseIpu) nil)
    ((:hz __hz) 0)
    ((:camera __camera) "")
    )
   (send-super :init)
   (setq _qrCodeId (round __qrCodeId))
   (setq _funcId (round __funcId))
   (setq _isUseIpu __isUseIpu)
   (setq _hz (round __hz))
   (setq _camera (string __camera))
   self)
  (:qrCodeId
   (&optional __qrCodeId)
   (if __qrCodeId (setq _qrCodeId __qrCodeId)) _qrCodeId)
  (:funcId
   (&optional __funcId)
   (if __funcId (setq _funcId __funcId)) _funcId)
  (:isUseIpu
   (&optional (__isUseIpu :null))
   (if (not (eq __isUseIpu :null)) (setq _isUseIpu __isUseIpu)) _isUseIpu)
  (:hz
   (&optional __hz)
   (if __hz (setq _hz __hz)) _hz)
  (:camera
   (&optional __camera)
   (if __camera (setq _camera __camera)) _camera)
  (:serialization-length
   ()
   (+
    ;; uint8 _qrCodeId
    1
    ;; uint8 _funcId
    1
    ;; bool _isUseIpu
    1
    ;; uint8 _hz
    1
    ;; string _camera
    4 (length _camera)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint8 _qrCodeId
       (write-byte _qrCodeId s)
     ;; uint8 _funcId
       (write-byte _funcId s)
     ;; bool _isUseIpu
       (if _isUseIpu (write-byte -1 s) (write-byte 0 s))
     ;; uint8 _hz
       (write-byte _hz s)
     ;; string _camera
       (write-long (length _camera) s) (princ _camera s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint8 _qrCodeId
     (setq _qrCodeId (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _funcId
     (setq _funcId (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; bool _isUseIpu
     (setq _isUseIpu (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;; uint8 _hz
     (setq _hz (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; string _camera
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _camera (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;;
   self)
  )

(defclass comm_srvs::visionFuncSimResponse
  :super ros::object
  :slots (_success ))

(defmethod comm_srvs::visionFuncSimResponse
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

(defclass comm_srvs::visionFuncSim
  :super ros::object
  :slots ())

(setf (get comm_srvs::visionFuncSim :md5sum-) "0a3c827bca110ade616a89594700a208")
(setf (get comm_srvs::visionFuncSim :datatype-) "comm_srvs/visionFuncSim")
(setf (get comm_srvs::visionFuncSim :request) comm_srvs::visionFuncSimRequest)
(setf (get comm_srvs::visionFuncSim :response) comm_srvs::visionFuncSimResponse)

(defmethod comm_srvs::visionFuncSimRequest
  (:response () (instance comm_srvs::visionFuncSimResponse :init)))

(setf (get comm_srvs::visionFuncSimRequest :md5sum-) "0a3c827bca110ade616a89594700a208")
(setf (get comm_srvs::visionFuncSimRequest :datatype-) "comm_srvs/visionFuncSimRequest")
(setf (get comm_srvs::visionFuncSimRequest :definition-)
      "uint8       qrCodeId
uint8       funcId
bool        isUseIpu
uint8       hz
string      camera
---
bool        success
")

(setf (get comm_srvs::visionFuncSimResponse :md5sum-) "0a3c827bca110ade616a89594700a208")
(setf (get comm_srvs::visionFuncSimResponse :datatype-) "comm_srvs/visionFuncSimResponse")
(setf (get comm_srvs::visionFuncSimResponse :definition-)
      "uint8       qrCodeId
uint8       funcId
bool        isUseIpu
uint8       hz
string      camera
---
bool        success
")



(provide :comm_srvs/visionFuncSim "0a3c827bca110ade616a89594700a208")


