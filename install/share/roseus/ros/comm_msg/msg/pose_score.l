;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::pose_score)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'pose_score (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::POSE_SCORE")
  (make-package "COMM_MSG::POSE_SCORE"))

(in-package "ROS")
;;//! \htmlinclude pose_score.msg.html


(defclass comm_msg::pose_score
  :super ros::object
  :slots (_score _pose ))

(defmethod comm_msg::pose_score
  (:init
   (&key
    ((:score __score) 0.0)
    ((:pose __pose) (instance comm_msg::pose3D :init))
    )
   (send-super :init)
   (setq _score (float __score))
   (setq _pose __pose)
   self)
  (:score
   (&optional __score)
   (if __score (setq _score __score)) _score)
  (:pose
   (&rest __pose)
   (if (keywordp (car __pose))
       (send* _pose __pose)
     (progn
       (if __pose (setq _pose (car __pose)))
       _pose)))
  (:serialization-length
   ()
   (+
    ;; float32 _score
    4
    ;; comm_msg/pose3D _pose
    (send _pose :serialization-length)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float32 _score
       (sys::poke _score (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; comm_msg/pose3D _pose
       (send _pose :serialize s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float32 _score
     (setq _score (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; comm_msg/pose3D _pose
     (send _pose :deserialize buf ptr-) (incf ptr- (send _pose :serialization-length))
   ;;
   self)
  )

(setf (get comm_msg::pose_score :md5sum-) "1ba278e321050d4e0b42ab7a7a69fe0c")
(setf (get comm_msg::pose_score :datatype-) "comm_msg/pose_score")
(setf (get comm_msg::pose_score :definition-)
      "float32 score
pose3D  pose
================================================================================
MSG: comm_msg/pose3D
float32 posX
float32 posY
float32 posZ
float32 roll
float32 pitch
float32 yaw
")



(provide :comm_msg/pose_score "1ba278e321050d4e0b42ab7a7a69fe0c")


