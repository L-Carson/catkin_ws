;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::Mode)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'Mode (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::MODE")
  (make-package "COMM_MSG::MODE"))

(in-package "ROS")
;;//! \htmlinclude Mode.msg.html


(defclass comm_msg::Mode
  :super ros::object
  :slots (_mode ))

(defmethod comm_msg::Mode
  (:init
   (&key
    ((:mode __mode) 0)
    )
   (send-super :init)
   (setq _mode (round __mode))
   self)
  (:mode
   (&optional __mode)
   (if __mode (setq _mode __mode)) _mode)
  (:serialization-length
   ()
   (+
    ;; int32 _mode
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int32 _mode
       (write-long _mode s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int32 _mode
     (setq _mode (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get comm_msg::Mode :md5sum-) "ff63f6ea3c3e9b7504b2cb3ee0a09d92")
(setf (get comm_msg::Mode :datatype-) "comm_msg/Mode")
(setf (get comm_msg::Mode :definition-)
      "int32 mode    #1:navigation mode 2:gmapping mode

")



(provide :comm_msg/Mode "ff63f6ea3c3e9b7504b2cb3ee0a09d92")


