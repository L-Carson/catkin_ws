;; Auto-generated. Do not edit!


(when (boundp 'comm_srvs::PcdNet)
  (if (not (find-package "COMM_SRVS"))
    (make-package "COMM_SRVS"))
  (shadow 'PcdNet (find-package "COMM_SRVS")))
(unless (find-package "COMM_SRVS::PCDNET")
  (make-package "COMM_SRVS::PCDNET"))
(unless (find-package "COMM_SRVS::PCDNETREQUEST")
  (make-package "COMM_SRVS::PCDNETREQUEST"))
(unless (find-package "COMM_SRVS::PCDNETRESPONSE")
  (make-package "COMM_SRVS::PCDNETRESPONSE"))

(in-package "ROS")





(defclass comm_srvs::PcdNetRequest
  :super ros::object
  :slots ())

(defmethod comm_srvs::PcdNetRequest
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

(defclass comm_srvs::PcdNetResponse
  :super ros::object
  :slots (_success _isOpen _lidarInstallParamArray ))

(defmethod comm_srvs::PcdNetResponse
  (:init
   (&key
    ((:success __success) nil)
    ((:isOpen __isOpen) nil)
    ((:lidarInstallParamArray __lidarInstallParamArray) ())
    )
   (send-super :init)
   (setq _success __success)
   (setq _isOpen __isOpen)
   (setq _lidarInstallParamArray __lidarInstallParamArray)
   self)
  (:success
   (&optional (__success :null))
   (if (not (eq __success :null)) (setq _success __success)) _success)
  (:isOpen
   (&optional (__isOpen :null))
   (if (not (eq __isOpen :null)) (setq _isOpen __isOpen)) _isOpen)
  (:lidarInstallParamArray
   (&rest __lidarInstallParamArray)
   (if (keywordp (car __lidarInstallParamArray))
       (send* _lidarInstallParamArray __lidarInstallParamArray)
     (progn
       (if __lidarInstallParamArray (setq _lidarInstallParamArray (car __lidarInstallParamArray)))
       _lidarInstallParamArray)))
  (:serialization-length
   ()
   (+
    ;; bool _success
    1
    ;; bool _isOpen
    1
    ;; comm_srvs/LidarInstallParam[] _lidarInstallParamArray
    (apply #'+ (send-all _lidarInstallParamArray :serialization-length)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; bool _success
       (if _success (write-byte -1 s) (write-byte 0 s))
     ;; bool _isOpen
       (if _isOpen (write-byte -1 s) (write-byte 0 s))
     ;; comm_srvs/LidarInstallParam[] _lidarInstallParamArray
     (write-long (length _lidarInstallParamArray) s)
     (dolist (elem _lidarInstallParamArray)
       (send elem :serialize s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; bool _success
     (setq _success (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;; bool _isOpen
     (setq _isOpen (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;; comm_srvs/LidarInstallParam[] _lidarInstallParamArray
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _lidarInstallParamArray (let (r) (dotimes (i n) (push (instance comm_srvs::LidarInstallParam :init) r)) r))
     (dolist (elem- _lidarInstallParamArray)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;;
   self)
  )

(defclass comm_srvs::PcdNet
  :super ros::object
  :slots ())

(setf (get comm_srvs::PcdNet :md5sum-) "d49762f82989f849a33296dad5e777da")
(setf (get comm_srvs::PcdNet :datatype-) "comm_srvs/PcdNet")
(setf (get comm_srvs::PcdNet :request) comm_srvs::PcdNetRequest)
(setf (get comm_srvs::PcdNet :response) comm_srvs::PcdNetResponse)

(defmethod comm_srvs::PcdNetRequest
  (:response () (instance comm_srvs::PcdNetResponse :init)))

(setf (get comm_srvs::PcdNetRequest :md5sum-) "d49762f82989f849a33296dad5e777da")
(setf (get comm_srvs::PcdNetRequest :datatype-) "comm_srvs/PcdNetRequest")
(setf (get comm_srvs::PcdNetRequest :definition-)
      "---
bool    success
bool    isOpen
LidarInstallParam[] lidarInstallParamArray

================================================================================
MSG: comm_srvs/LidarInstallParam
string  name    #MainLidar BlindLidar BlindLidar2
string  serialNum
float32 x
float32 y
float32 z
float32 roll
float32 pitch
")

(setf (get comm_srvs::PcdNetResponse :md5sum-) "d49762f82989f849a33296dad5e777da")
(setf (get comm_srvs::PcdNetResponse :datatype-) "comm_srvs/PcdNetResponse")
(setf (get comm_srvs::PcdNetResponse :definition-)
      "---
bool    success
bool    isOpen
LidarInstallParam[] lidarInstallParamArray

================================================================================
MSG: comm_srvs/LidarInstallParam
string  name    #MainLidar BlindLidar BlindLidar2
string  serialNum
float32 x
float32 y
float32 z
float32 roll
float32 pitch
")



(provide :comm_srvs/PcdNet "d49762f82989f849a33296dad5e777da")


