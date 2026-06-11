;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::ultrasonicData)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'ultrasonicData (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::ULTRASONICDATA")
  (make-package "COMM_MSG::ULTRASONICDATA"))

(in-package "ROS")
;;//! \htmlinclude ultrasonicData.msg.html


(defclass comm_msg::ultrasonicData
  :super ros::object
  :slots (_usName _data ))

(defmethod comm_msg::ultrasonicData
  (:init
   (&key
    ((:usName __usName) "")
    ((:data __data) ())
    )
   (send-super :init)
   (setq _usName (string __usName))
   (setq _data __data)
   self)
  (:usName
   (&optional __usName)
   (if __usName (setq _usName __usName)) _usName)
  (:data
   (&rest __data)
   (if (keywordp (car __data))
       (send* _data __data)
     (progn
       (if __data (setq _data (car __data)))
       _data)))
  (:serialization-length
   ()
   (+
    ;; string _usName
    4 (length _usName)
    ;; comm_msg/coor2D[] _data
    (apply #'+ (send-all _data :serialization-length)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; string _usName
       (write-long (length _usName) s) (princ _usName s)
     ;; comm_msg/coor2D[] _data
     (write-long (length _data) s)
     (dolist (elem _data)
       (send elem :serialize s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; string _usName
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _usName (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; comm_msg/coor2D[] _data
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _data (let (r) (dotimes (i n) (push (instance comm_msg::coor2D :init) r)) r))
     (dolist (elem- _data)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;;
   self)
  )

(setf (get comm_msg::ultrasonicData :md5sum-) "16d3d45049c1ab8e0a2113ea61e36760")
(setf (get comm_msg::ultrasonicData :datatype-) "comm_msg/ultrasonicData")
(setf (get comm_msg::ultrasonicData :definition-)
      "string   usName
coor2D[] data
================================================================================
MSG: comm_msg/coor2D
coorLabel   label       #坐标类别
float32     pos_x
float32     pos_y
================================================================================
MSG: comm_msg/coorLabel
uint8   UNKNOWN      = 0  #未知
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



(provide :comm_msg/ultrasonicData "16d3d45049c1ab8e0a2113ea61e36760")


