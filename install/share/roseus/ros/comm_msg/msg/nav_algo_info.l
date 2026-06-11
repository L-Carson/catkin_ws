;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::nav_algo_info)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'nav_algo_info (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::NAV_ALGO_INFO")
  (make-package "COMM_MSG::NAV_ALGO_INFO"))

(in-package "ROS")
;;//! \htmlinclude nav_algo_info.msg.html


(defclass comm_msg::nav_algo_info
  :super ros::object
  :slots (_algoType ))

(defmethod comm_msg::nav_algo_info
  (:init
   (&key
    ((:algoType __algoType) 0)
    )
   (send-super :init)
   (setq _algoType (round __algoType))
   self)
  (:algoType
   (&optional __algoType)
   (if __algoType (setq _algoType __algoType)) _algoType)
  (:serialization-length
   ()
   (+
    ;; uint8 _algoType
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint8 _algoType
       (write-byte _algoType s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint8 _algoType
     (setq _algoType (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;;
   self)
  )

(setf (get comm_msg::nav_algo_info :md5sum-) "e5aec186900a8a01b0a68e06489a4999")
(setf (get comm_msg::nav_algo_info :datatype-) "comm_msg/nav_algo_info")
(setf (get comm_msg::nav_algo_info :definition-)
      "uint8 algoType

")



(provide :comm_msg/nav_algo_info "e5aec186900a8a01b0a68e06489a4999")


