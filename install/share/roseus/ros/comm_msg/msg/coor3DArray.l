;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::coor3DArray)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'coor3DArray (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::COOR3DARRAY")
  (make-package "COMM_MSG::COOR3DARRAY"))

(in-package "ROS")
;;//! \htmlinclude coor3DArray.msg.html


(defclass comm_msg::coor3DArray
  :super ros::object
  :slots (_coor3DArray ))

(defmethod comm_msg::coor3DArray
  (:init
   (&key
    ((:coor3DArray __coor3DArray) ())
    )
   (send-super :init)
   (setq _coor3DArray __coor3DArray)
   self)
  (:coor3DArray
   (&rest __coor3DArray)
   (if (keywordp (car __coor3DArray))
       (send* _coor3DArray __coor3DArray)
     (progn
       (if __coor3DArray (setq _coor3DArray (car __coor3DArray)))
       _coor3DArray)))
  (:serialization-length
   ()
   (+
    ;; comm_msg/coor3D[] _coor3DArray
    (apply #'+ (send-all _coor3DArray :serialization-length)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; comm_msg/coor3D[] _coor3DArray
     (write-long (length _coor3DArray) s)
     (dolist (elem _coor3DArray)
       (send elem :serialize s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; comm_msg/coor3D[] _coor3DArray
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _coor3DArray (let (r) (dotimes (i n) (push (instance comm_msg::coor3D :init) r)) r))
     (dolist (elem- _coor3DArray)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;;
   self)
  )

(setf (get comm_msg::coor3DArray :md5sum-) "f1d540e658b8fab024c8f2c1ea50f3fd")
(setf (get comm_msg::coor3DArray :datatype-) "comm_msg/coor3DArray")
(setf (get comm_msg::coor3DArray :definition-)
      "coor3D[]  coor3DArray
================================================================================
MSG: comm_msg/coor3D
float32 pos_x
float32 pos_y
float32 pos_z
")



(provide :comm_msg/coor3DArray "f1d540e658b8fab024c8f2c1ea50f3fd")


