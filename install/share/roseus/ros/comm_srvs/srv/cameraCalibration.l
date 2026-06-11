;; Auto-generated. Do not edit!


(when (boundp 'comm_srvs::cameraCalibration)
  (if (not (find-package "COMM_SRVS"))
    (make-package "COMM_SRVS"))
  (shadow 'cameraCalibration (find-package "COMM_SRVS")))
(unless (find-package "COMM_SRVS::CAMERACALIBRATION")
  (make-package "COMM_SRVS::CAMERACALIBRATION"))
(unless (find-package "COMM_SRVS::CAMERACALIBRATIONREQUEST")
  (make-package "COMM_SRVS::CAMERACALIBRATIONREQUEST"))
(unless (find-package "COMM_SRVS::CAMERACALIBRATIONRESPONSE")
  (make-package "COMM_SRVS::CAMERACALIBRATIONRESPONSE"))

(in-package "ROS")





(defclass comm_srvs::cameraCalibrationRequest
  :super ros::object
  :slots (_camera_name _camera_matrix _distortion_coefficients _tx _ty _tz _roll _pitch _yaw ))

(defmethod comm_srvs::cameraCalibrationRequest
  (:init
   (&key
    ((:camera_name __camera_name) "")
    ((:camera_matrix __camera_matrix) "")
    ((:distortion_coefficients __distortion_coefficients) "")
    ((:tx __tx) 0.0)
    ((:ty __ty) 0.0)
    ((:tz __tz) 0.0)
    ((:roll __roll) 0.0)
    ((:pitch __pitch) 0.0)
    ((:yaw __yaw) 0.0)
    )
   (send-super :init)
   (setq _camera_name (string __camera_name))
   (setq _camera_matrix (string __camera_matrix))
   (setq _distortion_coefficients (string __distortion_coefficients))
   (setq _tx (float __tx))
   (setq _ty (float __ty))
   (setq _tz (float __tz))
   (setq _roll (float __roll))
   (setq _pitch (float __pitch))
   (setq _yaw (float __yaw))
   self)
  (:camera_name
   (&optional __camera_name)
   (if __camera_name (setq _camera_name __camera_name)) _camera_name)
  (:camera_matrix
   (&optional __camera_matrix)
   (if __camera_matrix (setq _camera_matrix __camera_matrix)) _camera_matrix)
  (:distortion_coefficients
   (&optional __distortion_coefficients)
   (if __distortion_coefficients (setq _distortion_coefficients __distortion_coefficients)) _distortion_coefficients)
  (:tx
   (&optional __tx)
   (if __tx (setq _tx __tx)) _tx)
  (:ty
   (&optional __ty)
   (if __ty (setq _ty __ty)) _ty)
  (:tz
   (&optional __tz)
   (if __tz (setq _tz __tz)) _tz)
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
    ;; string _camera_name
    4 (length _camera_name)
    ;; string _camera_matrix
    4 (length _camera_matrix)
    ;; string _distortion_coefficients
    4 (length _distortion_coefficients)
    ;; float32 _tx
    4
    ;; float32 _ty
    4
    ;; float32 _tz
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
     ;; string _camera_name
       (write-long (length _camera_name) s) (princ _camera_name s)
     ;; string _camera_matrix
       (write-long (length _camera_matrix) s) (princ _camera_matrix s)
     ;; string _distortion_coefficients
       (write-long (length _distortion_coefficients) s) (princ _distortion_coefficients s)
     ;; float32 _tx
       (sys::poke _tx (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _ty
       (sys::poke _ty (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _tz
       (sys::poke _tz (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
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
   ;; string _camera_name
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _camera_name (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; string _camera_matrix
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _camera_matrix (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; string _distortion_coefficients
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _distortion_coefficients (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; float32 _tx
     (setq _tx (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _ty
     (setq _ty (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _tz
     (setq _tz (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _roll
     (setq _roll (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _pitch
     (setq _pitch (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _yaw
     (setq _yaw (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(defclass comm_srvs::cameraCalibrationResponse
  :super ros::object
  :slots (_success ))

(defmethod comm_srvs::cameraCalibrationResponse
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

(defclass comm_srvs::cameraCalibration
  :super ros::object
  :slots ())

(setf (get comm_srvs::cameraCalibration :md5sum-) "79a62af09061e3caf062e9248160d26b")
(setf (get comm_srvs::cameraCalibration :datatype-) "comm_srvs/cameraCalibration")
(setf (get comm_srvs::cameraCalibration :request) comm_srvs::cameraCalibrationRequest)
(setf (get comm_srvs::cameraCalibration :response) comm_srvs::cameraCalibrationResponse)

(defmethod comm_srvs::cameraCalibrationRequest
  (:response () (instance comm_srvs::cameraCalibrationResponse :init)))

(setf (get comm_srvs::cameraCalibrationRequest :md5sum-) "79a62af09061e3caf062e9248160d26b")
(setf (get comm_srvs::cameraCalibrationRequest :datatype-) "comm_srvs/cameraCalibrationRequest")
(setf (get comm_srvs::cameraCalibrationRequest :definition-)
      "string  camera_name             #相机名称
string  camera_matrix           #相机内参
string  distortion_coefficients #畸变系数
float32 tx
float32 ty
float32 tz
float32 roll
float32 pitch
float32 yaw
---
bool    success

")

(setf (get comm_srvs::cameraCalibrationResponse :md5sum-) "79a62af09061e3caf062e9248160d26b")
(setf (get comm_srvs::cameraCalibrationResponse :datatype-) "comm_srvs/cameraCalibrationResponse")
(setf (get comm_srvs::cameraCalibrationResponse :definition-)
      "string  camera_name             #相机名称
string  camera_matrix           #相机内参
string  distortion_coefficients #畸变系数
float32 tx
float32 ty
float32 tz
float32 roll
float32 pitch
float32 yaw
---
bool    success

")



(provide :comm_srvs/cameraCalibration "79a62af09061e3caf062e9248160d26b")


