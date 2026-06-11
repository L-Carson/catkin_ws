;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::simulation_obsts)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'simulation_obsts (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::SIMULATION_OBSTS")
  (make-package "COMM_MSG::SIMULATION_OBSTS"))

(in-package "ROS")
;;//! \htmlinclude simulation_obsts.msg.html


(defclass comm_msg::simulation_obsts
  :super ros::object
  :slots (_circularObsts ))

(defmethod comm_msg::simulation_obsts
  (:init
   (&key
    ((:circularObsts __circularObsts) ())
    )
   (send-super :init)
   (setq _circularObsts __circularObsts)
   self)
  (:circularObsts
   (&rest __circularObsts)
   (if (keywordp (car __circularObsts))
       (send* _circularObsts __circularObsts)
     (progn
       (if __circularObsts (setq _circularObsts (car __circularObsts)))
       _circularObsts)))
  (:serialization-length
   ()
   (+
    ;; comm_msg/simulation_circular_obst[] _circularObsts
    (apply #'+ (send-all _circularObsts :serialization-length)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; comm_msg/simulation_circular_obst[] _circularObsts
     (write-long (length _circularObsts) s)
     (dolist (elem _circularObsts)
       (send elem :serialize s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; comm_msg/simulation_circular_obst[] _circularObsts
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _circularObsts (let (r) (dotimes (i n) (push (instance comm_msg::simulation_circular_obst :init) r)) r))
     (dolist (elem- _circularObsts)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;;
   self)
  )

(setf (get comm_msg::simulation_obsts :md5sum-) "addd4aa7ed3126b9ca217aeb1d69aa70")
(setf (get comm_msg::simulation_obsts :datatype-) "comm_msg/simulation_obsts")
(setf (get comm_msg::simulation_obsts :definition-)
      "simulation_circular_obst[] circularObsts
================================================================================
MSG: comm_msg/simulation_circular_obst
float32 x
float32 y
float32 radius
")



(provide :comm_msg/simulation_obsts "addd4aa7ed3126b9ca217aeb1d69aa70")


