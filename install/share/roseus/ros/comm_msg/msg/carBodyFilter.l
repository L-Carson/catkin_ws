;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::carBodyFilter)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'carBodyFilter (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::CARBODYFILTER")
  (make-package "COMM_MSG::CARBODYFILTER"))

(in-package "ROS")
;;//! \htmlinclude carBodyFilter.msg.html


(defclass comm_msg::carBodyFilter
  :super ros::object
  :slots (_filter ))

(defmethod comm_msg::carBodyFilter
  (:init
   (&key
    ((:filter __filter) 0)
    )
   (send-super :init)
   (setq _filter (round __filter))
   self)
  (:filter
   (&optional __filter)
   (if __filter (setq _filter __filter)) _filter)
  (:serialization-length
   ()
   (+
    ;; uint8 _filter
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint8 _filter
       (write-byte _filter s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint8 _filter
     (setq _filter (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;;
   self)
  )

(setf (get comm_msg::carBodyFilter :md5sum-) "ca8270e9d6440d7014080687771919c9")
(setf (get comm_msg::carBodyFilter :datatype-) "comm_msg/carBodyFilter")
(setf (get comm_msg::carBodyFilter :definition-)
      "# 0 全量构建
# 1 增量构建
# 2 保存并启用
# 3 取消构建

uint8 filter
")



(provide :comm_msg/carBodyFilter "ca8270e9d6440d7014080687771919c9")


