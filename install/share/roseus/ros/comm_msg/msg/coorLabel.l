;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::coorLabel)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'coorLabel (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::COORLABEL")
  (make-package "COMM_MSG::COORLABEL"))

(in-package "ROS")
;;//! \htmlinclude coorLabel.msg.html


(intern "*UNKNOWN*" (find-package "COMM_MSG::COORLABEL"))
(shadow '*UNKNOWN* (find-package "COMM_MSG::COORLABEL"))
(defconstant comm_msg::coorLabel::*UNKNOWN* 0)
(intern "*STATIC*" (find-package "COMM_MSG::COORLABEL"))
(shadow '*STATIC* (find-package "COMM_MSG::COORLABEL"))
(defconstant comm_msg::coorLabel::*STATIC* 1)
(intern "*DYNAMIC*" (find-package "COMM_MSG::COORLABEL"))
(shadow '*DYNAMIC* (find-package "COMM_MSG::COORLABEL"))
(defconstant comm_msg::coorLabel::*DYNAMIC* 2)
(intern "*HOLLOW_OBST*" (find-package "COMM_MSG::COORLABEL"))
(shadow '*HOLLOW_OBST* (find-package "COMM_MSG::COORLABEL"))
(defconstant comm_msg::coorLabel::*HOLLOW_OBST* 3)
(intern "*SHORT_HOLD*" (find-package "COMM_MSG::COORLABEL"))
(shadow '*SHORT_HOLD* (find-package "COMM_MSG::COORLABEL"))
(defconstant comm_msg::coorLabel::*SHORT_HOLD* 4)
(intern "*COMMON_HOLD*" (find-package "COMM_MSG::COORLABEL"))
(shadow '*COMMON_HOLD* (find-package "COMM_MSG::COORLABEL"))
(defconstant comm_msg::coorLabel::*COMMON_HOLD* 5)
(intern "*BLIND_SPOT*" (find-package "COMM_MSG::COORLABEL"))
(shadow '*BLIND_SPOT* (find-package "COMM_MSG::COORLABEL"))
(defconstant comm_msg::coorLabel::*BLIND_SPOT* 6)
(intern "*SLOPE_PAD*" (find-package "COMM_MSG::COORLABEL"))
(shadow '*SLOPE_PAD* (find-package "COMM_MSG::COORLABEL"))
(defconstant comm_msg::coorLabel::*SLOPE_PAD* 7)
(intern "*CONTACT_EDGE*" (find-package "COMM_MSG::COORLABEL"))
(shadow '*CONTACT_EDGE* (find-package "COMM_MSG::COORLABEL"))
(defconstant comm_msg::coorLabel::*CONTACT_EDGE* 8)
(intern "*TREELAWN*" (find-package "COMM_MSG::COORLABEL"))
(shadow '*TREELAWN* (find-package "COMM_MSG::COORLABEL"))
(defconstant comm_msg::coorLabel::*TREELAWN* 9)
(intern "*ULTRASONIC*" (find-package "COMM_MSG::COORLABEL"))
(shadow '*ULTRASONIC* (find-package "COMM_MSG::COORLABEL"))
(defconstant comm_msg::coorLabel::*ULTRASONIC* 10)

(defun comm_msg::coorLabel-to-symbol (const)
  (cond
        ((= const 0) 'comm_msg::coorLabel::*UNKNOWN*)
        ((= const 1) 'comm_msg::coorLabel::*STATIC*)
        ((= const 2) 'comm_msg::coorLabel::*DYNAMIC*)
        ((= const 3) 'comm_msg::coorLabel::*HOLLOW_OBST*)
        ((= const 4) 'comm_msg::coorLabel::*SHORT_HOLD*)
        ((= const 5) 'comm_msg::coorLabel::*COMMON_HOLD*)
        ((= const 6) 'comm_msg::coorLabel::*BLIND_SPOT*)
        ((= const 7) 'comm_msg::coorLabel::*SLOPE_PAD*)
        ((= const 8) 'comm_msg::coorLabel::*CONTACT_EDGE*)
        ((= const 9) 'comm_msg::coorLabel::*TREELAWN*)
        ((= const 10) 'comm_msg::coorLabel::*ULTRASONIC*)
        (t nil)))

(defclass comm_msg::coorLabel
  :super ros::object
  :slots (_value ))

(defmethod comm_msg::coorLabel
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

(setf (get comm_msg::coorLabel :md5sum-) "c0283db75cf753d6e1cb791ae85ce57c")
(setf (get comm_msg::coorLabel :datatype-) "comm_msg/coorLabel")
(setf (get comm_msg::coorLabel :definition-)
      "uint8   UNKNOWN      = 0  #未知
uint8   STATIC       = 1  #静态
uint8   DYNAMIC      = 2  #动态
uint8   HOLLOW_OBST  = 3  #空洞障碍
uint8   SHORT_HOLD   = 4  #低矮维持
uint8   COMMON_HOLD  = 5  #常规维持
uint8   BLIND_SPOT   = 6  #盲区障碍
uint8   SLOPE_PAD    = 7  #斜坡垫
uint8   CONTACT_EDGE = 8  #触边障碍
uint8   TREELAWN     = 9  #绿植障碍
uint8   ULTRASONIC   = 10 #超声

uint8 value
")



(provide :comm_msg/coorLabel "c0283db75cf753d6e1cb791ae85ce57c")


