;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::mapping_ctrl)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'mapping_ctrl (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::MAPPING_CTRL")
  (make-package "COMM_MSG::MAPPING_CTRL"))

(in-package "ROS")
;;//! \htmlinclude mapping_ctrl.msg.html


(defclass comm_msg::mapping_ctrl
  :super ros::object
  :slots (_cmd ))

(defmethod comm_msg::mapping_ctrl
  (:init
   (&key
    ((:cmd __cmd) 0)
    )
   (send-super :init)
   (setq _cmd (round __cmd))
   self)
  (:cmd
   (&optional __cmd)
   (if __cmd (setq _cmd __cmd)) _cmd)
  (:serialization-length
   ()
   (+
    ;; int32 _cmd
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int32 _cmd
       (write-long _cmd s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int32 _cmd
     (setq _cmd (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get comm_msg::mapping_ctrl :md5sum-) "66990e73c7aab0c47ddcdc70f7fa5bd0")
(setf (get comm_msg::mapping_ctrl :datatype-) "comm_msg/mapping_ctrl")
(setf (get comm_msg::mapping_ctrl :definition-)
      "int32 cmd #1:save map command 2:enable gnss 3:disable gnss

")



(provide :comm_msg/mapping_ctrl "66990e73c7aab0c47ddcdc70f7fa5bd0")


