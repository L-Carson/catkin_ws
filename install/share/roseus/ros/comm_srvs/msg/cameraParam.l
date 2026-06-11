;; Auto-generated. Do not edit!


(when (boundp 'comm_srvs::cameraParam)
  (if (not (find-package "COMM_SRVS"))
    (make-package "COMM_SRVS"))
  (shadow 'cameraParam (find-package "COMM_SRVS")))
(unless (find-package "COMM_SRVS::CAMERAPARAM")
  (make-package "COMM_SRVS::CAMERAPARAM"))

(in-package "ROS")
;;//! \htmlinclude cameraParam.msg.html


(defclass comm_srvs::cameraParam
  :super ros::object
  :slots (_name_id _port_name _frame_hz _height _width _tx _ty _tz _roll _pitch _yaw _camera_matrix _distortion_coefficients ))

(defmethod comm_srvs::cameraParam
  (:init
   (&key
    ((:name_id __name_id) "")
    ((:port_name __port_name) "")
    ((:frame_hz __frame_hz) 0)
    ((:height __height) 0)
    ((:width __width) 0)
    ((:tx __tx) 0.0)
    ((:ty __ty) 0.0)
    ((:tz __tz) 0.0)
    ((:roll __roll) 0.0)
    ((:pitch __pitch) 0.0)
    ((:yaw __yaw) 0.0)
    ((:camera_matrix __camera_matrix) (make-array 0 :initial-element 0.0 :element-type :float))
    ((:distortion_coefficients __distortion_coefficients) (make-array 0 :initial-element 0.0 :element-type :float))
    )
   (send-super :init)
   (setq _name_id (string __name_id))
   (setq _port_name (string __port_name))
   (setq _frame_hz (round __frame_hz))
   (setq _height (round __height))
   (setq _width (round __width))
   (setq _tx (float __tx))
   (setq _ty (float __ty))
   (setq _tz (float __tz))
   (setq _roll (float __roll))
   (setq _pitch (float __pitch))
   (setq _yaw (float __yaw))
   (setq _camera_matrix __camera_matrix)
   (setq _distortion_coefficients __distortion_coefficients)
   self)
  (:name_id
   (&optional __name_id)
   (if __name_id (setq _name_id __name_id)) _name_id)
  (:port_name
   (&optional __port_name)
   (if __port_name (setq _port_name __port_name)) _port_name)
  (:frame_hz
   (&optional __frame_hz)
   (if __frame_hz (setq _frame_hz __frame_hz)) _frame_hz)
  (:height
   (&optional __height)
   (if __height (setq _height __height)) _height)
  (:width
   (&optional __width)
   (if __width (setq _width __width)) _width)
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
  (:camera_matrix
   (&optional __camera_matrix)
   (if __camera_matrix (setq _camera_matrix __camera_matrix)) _camera_matrix)
  (:distortion_coefficients
   (&optional __distortion_coefficients)
   (if __distortion_coefficients (setq _distortion_coefficients __distortion_coefficients)) _distortion_coefficients)
  (:serialization-length
   ()
   (+
    ;; string _name_id
    4 (length _name_id)
    ;; string _port_name
    4 (length _port_name)
    ;; uint32 _frame_hz
    4
    ;; uint32 _height
    4
    ;; uint32 _width
    4
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
    ;; float64[] _camera_matrix
    (* 8    (length _camera_matrix)) 4
    ;; float64[] _distortion_coefficients
    (* 8    (length _distortion_coefficients)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; string _name_id
       (write-long (length _name_id) s) (princ _name_id s)
     ;; string _port_name
       (write-long (length _port_name) s) (princ _port_name s)
     ;; uint32 _frame_hz
       (write-long _frame_hz s)
     ;; uint32 _height
       (write-long _height s)
     ;; uint32 _width
       (write-long _width s)
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
     ;; float64[] _camera_matrix
     (write-long (length _camera_matrix) s)
     (dotimes (i (length _camera_matrix))
       (sys::poke (elt _camera_matrix i) (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
       )
     ;; float64[] _distortion_coefficients
     (write-long (length _distortion_coefficients) s)
     (dotimes (i (length _distortion_coefficients))
       (sys::poke (elt _distortion_coefficients i) (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; string _name_id
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _name_id (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; string _port_name
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _port_name (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; uint32 _frame_hz
     (setq _frame_hz (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; uint32 _height
     (setq _height (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; uint32 _width
     (setq _width (sys::peek buf ptr- :integer)) (incf ptr- 4)
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
   ;; float64[] _camera_matrix
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _camera_matrix (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _camera_matrix i) (sys::peek buf ptr- :double)) (incf ptr- 8)
     ))
   ;; float64[] _distortion_coefficients
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _distortion_coefficients (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _distortion_coefficients i) (sys::peek buf ptr- :double)) (incf ptr- 8)
     ))
   ;;
   self)
  )

(setf (get comm_srvs::cameraParam :md5sum-) "4f9f9ccd79905f101e4555a21c54abfd")
(setf (get comm_srvs::cameraParam :datatype-) "comm_srvs/cameraParam")
(setf (get comm_srvs::cameraParam :definition-)
      "string      name_id
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



(provide :comm_srvs/cameraParam "4f9f9ccd79905f101e4555a21c54abfd")


