;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::detect_result)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'detect_result (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::DETECT_RESULT")
  (make-package "COMM_MSG::DETECT_RESULT"))

(in-package "ROS")
;;//! \htmlinclude detect_result.msg.html


(defclass comm_msg::detect_result
  :super ros::object
  :slots (_result ))

(defmethod comm_msg::detect_result
  (:init
   (&key
    ((:result __result) 0)
    )
   (send-super :init)
   (setq _result (round __result))
   self)
  (:result
   (&optional __result)
   (if __result (setq _result __result)) _result)
  (:serialization-length
   ()
   (+
    ;; int8 _result
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int8 _result
       (write-byte _result s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int8 _result
     (setq _result (sys::peek buf ptr- :char)) (incf ptr- 1)
     (if (> _result 127) (setq _result (- _result 256)))
   ;;
   self)
  )

(setf (get comm_msg::detect_result :md5sum-) "4414c67819626a1b8e0f043a9a0d6c9a")
(setf (get comm_msg::detect_result :datatype-) "comm_msg/detect_result")
(setf (get comm_msg::detect_result :definition-)
      "# 识别门时 0为开 1为关 -1未知
int8  result
")



(provide :comm_msg/detect_result "4414c67819626a1b8e0f043a9a0d6c9a")


