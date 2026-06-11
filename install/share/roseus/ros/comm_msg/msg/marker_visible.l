;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::marker_visible)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'marker_visible (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::MARKER_VISIBLE")
  (make-package "COMM_MSG::MARKER_VISIBLE"))

(in-package "ROS")
;;//! \htmlinclude marker_visible.msg.html
(if (not (find-package "GEOMETRY_MSGS"))
  (ros::roseus-add-msgs "geometry_msgs"))


(defclass comm_msg::marker_visible
  :super ros::object
  :slots (_point_P _point_V _velocity ))

(defmethod comm_msg::marker_visible
  (:init
   (&key
    ((:point_P __point_P) ())
    ((:point_V __point_V) ())
    ((:velocity __velocity) (make-array 0 :initial-element 0.0 :element-type :float))
    )
   (send-super :init)
   (setq _point_P __point_P)
   (setq _point_V __point_V)
   (setq _velocity __velocity)
   self)
  (:point_P
   (&rest __point_P)
   (if (keywordp (car __point_P))
       (send* _point_P __point_P)
     (progn
       (if __point_P (setq _point_P (car __point_P)))
       _point_P)))
  (:point_V
   (&rest __point_V)
   (if (keywordp (car __point_V))
       (send* _point_V __point_V)
     (progn
       (if __point_V (setq _point_V (car __point_V)))
       _point_V)))
  (:velocity
   (&optional __velocity)
   (if __velocity (setq _velocity __velocity)) _velocity)
  (:serialization-length
   ()
   (+
    ;; geometry_msgs/Point[] _point_P
    (apply #'+ (send-all _point_P :serialization-length)) 4
    ;; geometry_msgs/Point[] _point_V
    (apply #'+ (send-all _point_V :serialization-length)) 4
    ;; float32[] _velocity
    (* 4    (length _velocity)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; geometry_msgs/Point[] _point_P
     (write-long (length _point_P) s)
     (dolist (elem _point_P)
       (send elem :serialize s)
       )
     ;; geometry_msgs/Point[] _point_V
     (write-long (length _point_V) s)
     (dolist (elem _point_V)
       (send elem :serialize s)
       )
     ;; float32[] _velocity
     (write-long (length _velocity) s)
     (dotimes (i (length _velocity))
       (sys::poke (elt _velocity i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; geometry_msgs/Point[] _point_P
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _point_P (let (r) (dotimes (i n) (push (instance geometry_msgs::Point :init) r)) r))
     (dolist (elem- _point_P)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;; geometry_msgs/Point[] _point_V
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _point_V (let (r) (dotimes (i n) (push (instance geometry_msgs::Point :init) r)) r))
     (dolist (elem- _point_V)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;; float32[] _velocity
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _velocity (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _velocity i) (sys::peek buf ptr- :float)) (incf ptr- 4)
     ))
   ;;
   self)
  )

(setf (get comm_msg::marker_visible :md5sum-) "8c091b688cfba7fce69c6312f128ed30")
(setf (get comm_msg::marker_visible :datatype-) "comm_msg/marker_visible")
(setf (get comm_msg::marker_visible :definition-)
      "geometry_msgs/Point[] 	point_P
geometry_msgs/Point[] 	point_V

float32[]               velocity

================================================================================
MSG: geometry_msgs/Point
# This contains the position of a point in free space
float64 x
float64 y
float64 z

")



(provide :comm_msg/marker_visible "8c091b688cfba7fce69c6312f128ed30")


