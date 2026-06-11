;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::move_close_loop_ctrl)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'move_close_loop_ctrl (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::MOVE_CLOSE_LOOP_CTRL")
  (make-package "COMM_MSG::MOVE_CLOSE_LOOP_CTRL"))

(in-package "ROS")
;;//! \htmlinclude move_close_loop_ctrl.msg.html


(defclass comm_msg::move_close_loop_ctrl
  :super ros::object
  :slots (_plannedSteeringAngle _plannedMoveLinearVel _controlSteeringAngle _controlMoveLinearVel _measureSteeringAngle _measureMoveLinearVel ))

(defmethod comm_msg::move_close_loop_ctrl
  (:init
   (&key
    ((:plannedSteeringAngle __plannedSteeringAngle) 0.0)
    ((:plannedMoveLinearVel __plannedMoveLinearVel) 0.0)
    ((:controlSteeringAngle __controlSteeringAngle) 0.0)
    ((:controlMoveLinearVel __controlMoveLinearVel) 0.0)
    ((:measureSteeringAngle __measureSteeringAngle) 0.0)
    ((:measureMoveLinearVel __measureMoveLinearVel) 0.0)
    )
   (send-super :init)
   (setq _plannedSteeringAngle (float __plannedSteeringAngle))
   (setq _plannedMoveLinearVel (float __plannedMoveLinearVel))
   (setq _controlSteeringAngle (float __controlSteeringAngle))
   (setq _controlMoveLinearVel (float __controlMoveLinearVel))
   (setq _measureSteeringAngle (float __measureSteeringAngle))
   (setq _measureMoveLinearVel (float __measureMoveLinearVel))
   self)
  (:plannedSteeringAngle
   (&optional __plannedSteeringAngle)
   (if __plannedSteeringAngle (setq _plannedSteeringAngle __plannedSteeringAngle)) _plannedSteeringAngle)
  (:plannedMoveLinearVel
   (&optional __plannedMoveLinearVel)
   (if __plannedMoveLinearVel (setq _plannedMoveLinearVel __plannedMoveLinearVel)) _plannedMoveLinearVel)
  (:controlSteeringAngle
   (&optional __controlSteeringAngle)
   (if __controlSteeringAngle (setq _controlSteeringAngle __controlSteeringAngle)) _controlSteeringAngle)
  (:controlMoveLinearVel
   (&optional __controlMoveLinearVel)
   (if __controlMoveLinearVel (setq _controlMoveLinearVel __controlMoveLinearVel)) _controlMoveLinearVel)
  (:measureSteeringAngle
   (&optional __measureSteeringAngle)
   (if __measureSteeringAngle (setq _measureSteeringAngle __measureSteeringAngle)) _measureSteeringAngle)
  (:measureMoveLinearVel
   (&optional __measureMoveLinearVel)
   (if __measureMoveLinearVel (setq _measureMoveLinearVel __measureMoveLinearVel)) _measureMoveLinearVel)
  (:serialization-length
   ()
   (+
    ;; float32 _plannedSteeringAngle
    4
    ;; float32 _plannedMoveLinearVel
    4
    ;; float32 _controlSteeringAngle
    4
    ;; float32 _controlMoveLinearVel
    4
    ;; float32 _measureSteeringAngle
    4
    ;; float32 _measureMoveLinearVel
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float32 _plannedSteeringAngle
       (sys::poke _plannedSteeringAngle (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _plannedMoveLinearVel
       (sys::poke _plannedMoveLinearVel (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _controlSteeringAngle
       (sys::poke _controlSteeringAngle (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _controlMoveLinearVel
       (sys::poke _controlMoveLinearVel (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _measureSteeringAngle
       (sys::poke _measureSteeringAngle (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _measureMoveLinearVel
       (sys::poke _measureMoveLinearVel (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float32 _plannedSteeringAngle
     (setq _plannedSteeringAngle (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _plannedMoveLinearVel
     (setq _plannedMoveLinearVel (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _controlSteeringAngle
     (setq _controlSteeringAngle (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _controlMoveLinearVel
     (setq _controlMoveLinearVel (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _measureSteeringAngle
     (setq _measureSteeringAngle (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _measureMoveLinearVel
     (setq _measureMoveLinearVel (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get comm_msg::move_close_loop_ctrl :md5sum-) "e51a1651c7c8b7b7a901faef61d12ad6")
(setf (get comm_msg::move_close_loop_ctrl :datatype-) "comm_msg/move_close_loop_ctrl")
(setf (get comm_msg::move_close_loop_ctrl :definition-)
      "float32 plannedSteeringAngle
float32 plannedMoveLinearVel
float32 controlSteeringAngle
float32 controlMoveLinearVel
float32 measureSteeringAngle
float32 measureMoveLinearVel
")



(provide :comm_msg/move_close_loop_ctrl "e51a1651c7c8b7b7a901faef61d12ad6")


