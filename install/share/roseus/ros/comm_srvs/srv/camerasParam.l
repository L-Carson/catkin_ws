;; Auto-generated. Do not edit!


(when (boundp 'comm_srvs::camerasParam)
  (if (not (find-package "COMM_SRVS"))
    (make-package "COMM_SRVS"))
  (shadow 'camerasParam (find-package "COMM_SRVS")))
(unless (find-package "COMM_SRVS::CAMERASPARAM")
  (make-package "COMM_SRVS::CAMERASPARAM"))
(unless (find-package "COMM_SRVS::CAMERASPARAMREQUEST")
  (make-package "COMM_SRVS::CAMERASPARAMREQUEST"))
(unless (find-package "COMM_SRVS::CAMERASPARAMRESPONSE")
  (make-package "COMM_SRVS::CAMERASPARAMRESPONSE"))

(in-package "ROS")





(defclass comm_srvs::camerasParamRequest
  :super ros::object
  :slots ())

(defmethod comm_srvs::camerasParamRequest
  (:init
   (&key
    )
   (send-super :init)
   self)
  (:serialization-length
   ()
   (+
    0
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;;
   self)
  )

(defclass comm_srvs::camerasParamResponse
  :super ros::object
  :slots (_success _cameraParamArray ))

(defmethod comm_srvs::camerasParamResponse
  (:init
   (&key
    ((:success __success) nil)
    ((:cameraParamArray __cameraParamArray) ())
    )
   (send-super :init)
   (setq _success __success)
   (setq _cameraParamArray __cameraParamArray)
   self)
  (:success
   (&optional (__success :null))
   (if (not (eq __success :null)) (setq _success __success)) _success)
  (:cameraParamArray
   (&rest __cameraParamArray)
   (if (keywordp (car __cameraParamArray))
       (send* _cameraParamArray __cameraParamArray)
     (progn
       (if __cameraParamArray (setq _cameraParamArray (car __cameraParamArray)))
       _cameraParamArray)))
  (:serialization-length
   ()
   (+
    ;; bool _success
    1
    ;; comm_srvs/cameraParam[] _cameraParamArray
    (apply #'+ (send-all _cameraParamArray :serialization-length)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; bool _success
       (if _success (write-byte -1 s) (write-byte 0 s))
     ;; comm_srvs/cameraParam[] _cameraParamArray
     (write-long (length _cameraParamArray) s)
     (dolist (elem _cameraParamArray)
       (send elem :serialize s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; bool _success
     (setq _success (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;; comm_srvs/cameraParam[] _cameraParamArray
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _cameraParamArray (let (r) (dotimes (i n) (push (instance comm_srvs::cameraParam :init) r)) r))
     (dolist (elem- _cameraParamArray)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;;
   self)
  )

(defclass comm_srvs::camerasParam
  :super ros::object
  :slots ())

(setf (get comm_srvs::camerasParam :md5sum-) "fd18540e50d2c9de8ab7b244039cf247")
(setf (get comm_srvs::camerasParam :datatype-) "comm_srvs/camerasParam")
(setf (get comm_srvs::camerasParam :request) comm_srvs::camerasParamRequest)
(setf (get comm_srvs::camerasParam :response) comm_srvs::camerasParamResponse)

(defmethod comm_srvs::camerasParamRequest
  (:response () (instance comm_srvs::camerasParamResponse :init)))

(setf (get comm_srvs::camerasParamRequest :md5sum-) "fd18540e50d2c9de8ab7b244039cf247")
(setf (get comm_srvs::camerasParamRequest :datatype-) "comm_srvs/camerasParamRequest")
(setf (get comm_srvs::camerasParamRequest :definition-)
      "---
bool          success
cameraParam[] cameraParamArray

================================================================================
MSG: comm_srvs/cameraParam
string      name_id
string      port_name
uint32      frame_hz

uint32      height
uint32      width

float32     tx
float32     ty
float32     tz
float32     roll
float32     pitch
float32     yaw

float64[]   camera_matrix
float64[]   distortion_coefficients
")

(setf (get comm_srvs::camerasParamResponse :md5sum-) "fd18540e50d2c9de8ab7b244039cf247")
(setf (get comm_srvs::camerasParamResponse :datatype-) "comm_srvs/camerasParamResponse")
(setf (get comm_srvs::camerasParamResponse :definition-)
      "---
bool          success
cameraParam[] cameraParamArray

================================================================================
MSG: comm_srvs/cameraParam
string      name_id
string      port_name
uint32      frame_hz

uint32      height
uint32      width

float32     tx
float32     ty
float32     tz
float32     roll
float32     pitch
float32     yaw

float64[]   camera_matrix
float64[]   distortion_coefficients
")



(provide :comm_srvs/camerasParam "fd18540e50d2c9de8ab7b244039cf247")


