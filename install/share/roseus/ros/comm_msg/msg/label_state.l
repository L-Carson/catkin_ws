;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::label_state)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'label_state (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::LABEL_STATE")
  (make-package "COMM_MSG::LABEL_STATE"))

(in-package "ROS")
;;//! \htmlinclude label_state.msg.html


(intern "*NO*" (find-package "COMM_MSG::LABEL_STATE"))
(shadow '*NO* (find-package "COMM_MSG::LABEL_STATE"))
(defconstant comm_msg::label_state::*NO* 0)
(intern "*YES*" (find-package "COMM_MSG::LABEL_STATE"))
(shadow '*YES* (find-package "COMM_MSG::LABEL_STATE"))
(defconstant comm_msg::label_state::*YES* 1)
(intern "*MAY*" (find-package "COMM_MSG::LABEL_STATE"))
(shadow '*MAY* (find-package "COMM_MSG::LABEL_STATE"))
(defconstant comm_msg::label_state::*MAY* 2)

(defun comm_msg::label_state-to-symbol (const)
  (cond
        ((= const 0) 'comm_msg::label_state::*NO*)
        ((= const 1) 'comm_msg::label_state::*YES*)
        ((= const 2) 'comm_msg::label_state::*MAY*)
        (t nil)))

(defclass comm_msg::label_state
  :super ros::object
  :slots (_value ))

(defmethod comm_msg::label_state
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

(setf (get comm_msg::label_state :md5sum-) "888716ada627b9e5d6021e226a28f209")
(setf (get comm_msg::label_state :datatype-) "comm_msg/label_state")
(setf (get comm_msg::label_state :definition-)
      "#Note: 标签类别存在状态

uint8 NO  = 0
uint8 YES = 1
uint8 MAY = 2

uint8 value
")



(provide :comm_msg/label_state "888716ada627b9e5d6021e226a28f209")


