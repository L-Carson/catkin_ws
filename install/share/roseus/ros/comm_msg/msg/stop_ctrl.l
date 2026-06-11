;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::stop_ctrl)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'stop_ctrl (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::STOP_CTRL")
  (make-package "COMM_MSG::STOP_CTRL"))

(in-package "ROS")
;;//! \htmlinclude stop_ctrl.msg.html


(defclass comm_msg::stop_ctrl
  :super ros::object
  :slots (_isAct ))

(defmethod comm_msg::stop_ctrl
  (:init
   (&key
    ((:isAct __isAct) 0)
    )
   (send-super :init)
   (setq _isAct (round __isAct))
   self)
  (:isAct
   (&optional __isAct)
   (if __isAct (setq _isAct __isAct)) _isAct)
  (:serialization-length
   ()
   (+
    ;; uint8 _isAct
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint8 _isAct
       (write-byte _isAct s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint8 _isAct
     (setq _isAct (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;;
   self)
  )

(setf (get comm_msg::stop_ctrl :md5sum-) "6214bd4b533a199bfb2bc3ad84feb300")
(setf (get comm_msg::stop_ctrl :datatype-) "comm_msg/stop_ctrl")
(setf (get comm_msg::stop_ctrl :definition-)
      "uint8  isAct
")



(provide :comm_msg/stop_ctrl "6214bd4b533a199bfb2bc3ad84feb300")


