;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::module_abnormal_info)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'module_abnormal_info (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::MODULE_ABNORMAL_INFO")
  (make-package "COMM_MSG::MODULE_ABNORMAL_INFO"))

(in-package "ROS")
;;//! \htmlinclude module_abnormal_info.msg.html


(defclass comm_msg::module_abnormal_info
  :super ros::object
  :slots (_module _errCode ))

(defmethod comm_msg::module_abnormal_info
  (:init
   (&key
    ((:module __module) 0)
    ((:errCode __errCode) 0)
    )
   (send-super :init)
   (setq _module (round __module))
   (setq _errCode (round __errCode))
   self)
  (:module
   (&optional __module)
   (if __module (setq _module __module)) _module)
  (:errCode
   (&optional __errCode)
   (if __errCode (setq _errCode __errCode)) _errCode)
  (:serialization-length
   ()
   (+
    ;; uint32 _module
    4
    ;; int32 _errCode
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint32 _module
       (write-long _module s)
     ;; int32 _errCode
       (write-long _errCode s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint32 _module
     (setq _module (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; int32 _errCode
     (setq _errCode (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get comm_msg::module_abnormal_info :md5sum-) "84ac86691c35a13d550d35161d31a61a")
(setf (get comm_msg::module_abnormal_info :datatype-) "comm_msg/module_abnormal_info")
(setf (get comm_msg::module_abnormal_info :definition-)
      "uint32 module
int32  errCode          # 0 表示正常, 其它都表示异常，后续扩展
")



(provide :comm_msg/module_abnormal_info "84ac86691c35a13d550d35161d31a61a")


