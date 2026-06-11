;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::coor2D)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'coor2D (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::COOR2D")
  (make-package "COMM_MSG::COOR2D"))

(in-package "ROS")
;;//! \htmlinclude coor2D.msg.html


(defclass comm_msg::coor2D
  :super ros::object
  :slots (_label _pos_x _pos_y ))

(defmethod comm_msg::coor2D
  (:init
   (&key
    ((:label __label) (instance comm_msg::coorLabel :init))
    ((:pos_x __pos_x) 0.0)
    ((:pos_y __pos_y) 0.0)
    )
   (send-super :init)
   (setq _label __label)
   (setq _pos_x (float __pos_x))
   (setq _pos_y (float __pos_y))
   self)
  (:label
   (&rest __label)
   (if (keywordp (car __label))
       (send* _label __label)
     (progn
       (if __label (setq _label (car __label)))
       _label)))
  (:pos_x
   (&optional __pos_x)
   (if __pos_x (setq _pos_x __pos_x)) _pos_x)
  (:pos_y
   (&optional __pos_y)
   (if __pos_y (setq _pos_y __pos_y)) _pos_y)
  (:serialization-length
   ()
   (+
    ;; comm_msg/coorLabel _label
    (send _label :serialization-length)
    ;; float32 _pos_x
    4
    ;; float32 _pos_y
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; comm_msg/coorLabel _label
       (send _label :serialize s)
     ;; float32 _pos_x
       (sys::poke _pos_x (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _pos_y
       (sys::poke _pos_y (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; comm_msg/coorLabel _label
     (send _label :deserialize buf ptr-) (incf ptr- (send _label :serialization-length))
   ;; float32 _pos_x
     (setq _pos_x (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _pos_y
     (setq _pos_y (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get comm_msg::coor2D :md5sum-) "a605ddea652ab63420ad444791949b49")
(setf (get comm_msg::coor2D :datatype-) "comm_msg/coor2D")
(setf (get comm_msg::coor2D :definition-)
      "coorLabel   label       #坐标类别
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



(provide :comm_msg/coor2D "a605ddea652ab63420ad444791949b49")


