;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::dustbinState)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'dustbinState (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::DUSTBINSTATE")
  (make-package "COMM_MSG::DUSTBINSTATE"))

(in-package "ROS")
;;//! \htmlinclude dustbinState.msg.html


(defclass comm_msg::dustbinState
  :super ros::object
  :slots (_isFull ))

(defmethod comm_msg::dustbinState
  (:init
   (&key
    ((:isFull __isFull) nil)
    )
   (send-super :init)
   (setq _isFull __isFull)
   self)
  (:isFull
   (&optional (__isFull :null))
   (if (not (eq __isFull :null)) (setq _isFull __isFull)) _isFull)
  (:serialization-length
   ()
   (+
    ;; bool _isFull
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; bool _isFull
       (if _isFull (write-byte -1 s) (write-byte 0 s))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; bool _isFull
     (setq _isFull (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;;
   self)
  )

(setf (get comm_msg::dustbinState :md5sum-) "3815cb2adedcd1d91ff6caca01a84001")
(setf (get comm_msg::dustbinState :datatype-) "comm_msg/dustbinState")
(setf (get comm_msg::dustbinState :definition-)
      "bool  isFull    #站点垃圾箱是否已满
")



(provide :comm_msg/dustbinState "3815cb2adedcd1d91ff6caca01a84001")


