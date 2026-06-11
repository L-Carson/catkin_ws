;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::nav_state)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'nav_state (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::NAV_STATE")
  (make-package "COMM_MSG::NAV_STATE"))

(in-package "ROS")
;;//! \htmlinclude nav_state.msg.html


(defclass comm_msg::nav_state
  :super ros::object
  :slots (_planMoveTrend _navCtrl ))

(defmethod comm_msg::nav_state
  (:init
   (&key
    ((:planMoveTrend __planMoveTrend) 0)
    ((:navCtrl __navCtrl) ())
    )
   (send-super :init)
   (setq _planMoveTrend (round __planMoveTrend))
   (setq _navCtrl __navCtrl)
   self)
  (:planMoveTrend
   (&optional __planMoveTrend)
   (if __planMoveTrend (setq _planMoveTrend __planMoveTrend)) _planMoveTrend)
  (:navCtrl
   (&rest __navCtrl)
   (if (keywordp (car __navCtrl))
       (send* _navCtrl __navCtrl)
     (progn
       (if __navCtrl (setq _navCtrl (car __navCtrl)))
       _navCtrl)))
  (:serialization-length
   ()
   (+
    ;; uint8 _planMoveTrend
    1
    ;; comm_msg/nav_ctrl[] _navCtrl
    (apply #'+ (send-all _navCtrl :serialization-length)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint8 _planMoveTrend
       (write-byte _planMoveTrend s)
     ;; comm_msg/nav_ctrl[] _navCtrl
     (write-long (length _navCtrl) s)
     (dolist (elem _navCtrl)
       (send elem :serialize s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint8 _planMoveTrend
     (setq _planMoveTrend (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; comm_msg/nav_ctrl[] _navCtrl
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _navCtrl (let (r) (dotimes (i n) (push (instance comm_msg::nav_ctrl :init) r)) r))
     (dolist (elem- _navCtrl)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;;
   self)
  )

(setf (get comm_msg::nav_state :md5sum-) "2b0151b05a659a933eedcf8f25c3b286")
(setf (get comm_msg::nav_state :datatype-) "comm_msg/nav_state")
(setf (get comm_msg::nav_state :definition-)
      "uint8       planMoveTrend        # 0:停止前进 1:向前运动 2:向后运动 3:自由运动
nav_ctrl[]  navCtrl
================================================================================
MSG: comm_msg/nav_ctrl
uint8 GARBAGE_DETECT = 1    # 垃圾寻扫
uint8 CONTACT_EDGE   = 2    # 触边生成障碍物

uint8 value
")



(provide :comm_msg/nav_state "2b0151b05a659a933eedcf8f25c3b286")


