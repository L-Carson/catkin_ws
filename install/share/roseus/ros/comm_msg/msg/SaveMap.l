;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::SaveMap)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'SaveMap (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::SAVEMAP")
  (make-package "COMM_MSG::SAVEMAP"))

(in-package "ROS")
;;//! \htmlinclude SaveMap.msg.html


(defclass comm_msg::SaveMap
  :super ros::object
  :slots (_flag ))

(defmethod comm_msg::SaveMap
  (:init
   (&key
    ((:flag __flag) 0)
    )
   (send-super :init)
   (setq _flag (round __flag))
   self)
  (:flag
   (&optional __flag)
   (if __flag (setq _flag __flag)) _flag)
  (:serialization-length
   ()
   (+
    ;; int32 _flag
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int32 _flag
       (write-long _flag s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int32 _flag
     (setq _flag (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get comm_msg::SaveMap :md5sum-) "aa0dbc5596ec12974ea3a17a045b36e0")
(setf (get comm_msg::SaveMap :datatype-) "comm_msg/SaveMap")
(setf (get comm_msg::SaveMap :definition-)
      "int32 flag    #1:save map command

")



(provide :comm_msg/SaveMap "aa0dbc5596ec12974ea3a17a045b36e0")


