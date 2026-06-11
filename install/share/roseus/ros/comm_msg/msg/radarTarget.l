;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::radarTarget)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'radarTarget (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::RADARTARGET")
  (make-package "COMM_MSG::RADARTARGET"))

(in-package "ROS")
;;//! \htmlinclude radarTarget.msg.html


(defclass comm_msg::radarTarget
  :super ros::object
  :slots (_targetDistx _targetDisty _targetVelx _targetVely ))

(defmethod comm_msg::radarTarget
  (:init
   (&key
    ((:targetDistx __targetDistx) 0.0)
    ((:targetDisty __targetDisty) 0.0)
    ((:targetVelx __targetVelx) 0.0)
    ((:targetVely __targetVely) 0.0)
    )
   (send-super :init)
   (setq _targetDistx (float __targetDistx))
   (setq _targetDisty (float __targetDisty))
   (setq _targetVelx (float __targetVelx))
   (setq _targetVely (float __targetVely))
   self)
  (:targetDistx
   (&optional __targetDistx)
   (if __targetDistx (setq _targetDistx __targetDistx)) _targetDistx)
  (:targetDisty
   (&optional __targetDisty)
   (if __targetDisty (setq _targetDisty __targetDisty)) _targetDisty)
  (:targetVelx
   (&optional __targetVelx)
   (if __targetVelx (setq _targetVelx __targetVelx)) _targetVelx)
  (:targetVely
   (&optional __targetVely)
   (if __targetVely (setq _targetVely __targetVely)) _targetVely)
  (:serialization-length
   ()
   (+
    ;; float32 _targetDistx
    4
    ;; float32 _targetDisty
    4
    ;; float32 _targetVelx
    4
    ;; float32 _targetVely
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float32 _targetDistx
       (sys::poke _targetDistx (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _targetDisty
       (sys::poke _targetDisty (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _targetVelx
       (sys::poke _targetVelx (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _targetVely
       (sys::poke _targetVely (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float32 _targetDistx
     (setq _targetDistx (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _targetDisty
     (setq _targetDisty (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _targetVelx
     (setq _targetVelx (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _targetVely
     (setq _targetVely (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get comm_msg::radarTarget :md5sum-) "2f0ce27dc024af9302b335afb3187795")
(setf (get comm_msg::radarTarget :datatype-) "comm_msg/radarTarget")
(setf (get comm_msg::radarTarget :definition-)
      "float32   targetDistx         # 目标相对于清扫车参考坐标系X轴坐标
float32   targetDisty         # 目标相对于清扫车参考坐标系Y轴坐标
float32   targetVelx          # 目标相对于清扫车参考坐标系X轴速度
float32   targetVely          # 目标相对于清扫车参考坐标系Y轴速度
")



(provide :comm_msg/radarTarget "2f0ce27dc024af9302b335afb3187795")


