;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::nav_ctrl)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'nav_ctrl (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::NAV_CTRL")
  (make-package "COMM_MSG::NAV_CTRL"))

(in-package "ROS")
;;//! \htmlinclude nav_ctrl.msg.html


(intern "*GARBAGE_DETECT*" (find-package "COMM_MSG::NAV_CTRL"))
(shadow '*GARBAGE_DETECT* (find-package "COMM_MSG::NAV_CTRL"))
(defconstant comm_msg::nav_ctrl::*GARBAGE_DETECT* 1)
(intern "*CONTACT_EDGE*" (find-package "COMM_MSG::NAV_CTRL"))
(shadow '*CONTACT_EDGE* (find-package "COMM_MSG::NAV_CTRL"))
(defconstant comm_msg::nav_ctrl::*CONTACT_EDGE* 2)

(defun comm_msg::nav_ctrl-to-symbol (const)
  (cond
        ((= const 1) 'comm_msg::nav_ctrl::*GARBAGE_DETECT*)
        ((= const 2) 'comm_msg::nav_ctrl::*CONTACT_EDGE*)
        (t nil)))

(defclass comm_msg::nav_ctrl
  :super ros::object
  :slots (_value ))

(defmethod comm_msg::nav_ctrl
  (:init
   (&key
    ((:value __value) 0)
    )
   (send-super :init)
   (setq _value (round __value))
   self)
  (:value
   (&optional __value)
   (if __value (setq _value __value)) _value)
  (:serialization-length
   ()
   (+
    ;; uint8 _value
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint8 _value
       (write-byte _value s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint8 _value
     (setq _value (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;;
   self)
  )

(setf (get comm_msg::nav_ctrl :md5sum-) "c6fe31622265b6df86e8a79eeb6d20fe")
(setf (get comm_msg::nav_ctrl :datatype-) "comm_msg/nav_ctrl")
(setf (get comm_msg::nav_ctrl :definition-)
      "uint8 GARBAGE_DETECT = 1    # 垃圾寻扫
uint8 CONTACT_EDGE   = 2    # 触边生成障碍物

uint8 value
")



(provide :comm_msg/nav_ctrl "c6fe31622265b6df86e8a79eeb6d20fe")


