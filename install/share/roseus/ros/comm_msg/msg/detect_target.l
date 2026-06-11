;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::detect_target)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'detect_target (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::DETECT_TARGET")
  (make-package "COMM_MSG::DETECT_TARGET"))

(in-package "ROS")
;;//! \htmlinclude detect_target.msg.html


(defclass comm_msg::detect_target
  :super ros::object
  :slots (_type _vertexes ))

(defmethod comm_msg::detect_target
  (:init
   (&key
    ((:type __type) 0)
    ((:vertexes __vertexes) ())
    )
   (send-super :init)
   (setq _type (round __type))
   (setq _vertexes __vertexes)
   self)
  (:type
   (&optional __type)
   (if __type (setq _type __type)) _type)
  (:vertexes
   (&rest __vertexes)
   (if (keywordp (car __vertexes))
       (send* _vertexes __vertexes)
     (progn
       (if __vertexes (setq _vertexes (car __vertexes)))
       _vertexes)))
  (:serialization-length
   ()
   (+
    ;; uint8 _type
    1
    ;; comm_msg/coor2D[] _vertexes
    (apply #'+ (send-all _vertexes :serialization-length)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint8 _type
       (write-byte _type s)
     ;; comm_msg/coor2D[] _vertexes
     (write-long (length _vertexes) s)
     (dolist (elem _vertexes)
       (send elem :serialize s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint8 _type
     (setq _type (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; comm_msg/coor2D[] _vertexes
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _vertexes (let (r) (dotimes (i n) (push (instance comm_msg::coor2D :init) r)) r))
     (dolist (elem- _vertexes)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;;
   self)
  )

(setf (get comm_msg::detect_target :md5sum-) "4e9958080bbdd6468da1c554f1002943")
(setf (get comm_msg::detect_target :datatype-) "comm_msg/detect_target")
(setf (get comm_msg::detect_target :definition-)
      "# 1开门 2关门
uint8    type
coor2D[] vertexes
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



(provide :comm_msg/detect_target "4e9958080bbdd6468da1c554f1002943")


