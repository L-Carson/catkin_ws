;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::coor2DArray)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'coor2DArray (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::COOR2DARRAY")
  (make-package "COMM_MSG::COOR2DARRAY"))

(in-package "ROS")
;;//! \htmlinclude coor2DArray.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass comm_msg::coor2DArray
  :super ros::object
  :slots (_header _isContainForwardObst _isContainBackwardObst _coor2DArray _coor2DArrayTop _coor2DArrayMid _coor2DArrayDown ))

(defmethod comm_msg::coor2DArray
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:isContainForwardObst __isContainForwardObst) 0)
    ((:isContainBackwardObst __isContainBackwardObst) 0)
    ((:coor2DArray __coor2DArray) ())
    ((:coor2DArrayTop __coor2DArrayTop) ())
    ((:coor2DArrayMid __coor2DArrayMid) ())
    ((:coor2DArrayDown __coor2DArrayDown) ())
    )
   (send-super :init)
   (setq _header __header)
   (setq _isContainForwardObst (round __isContainForwardObst))
   (setq _isContainBackwardObst (round __isContainBackwardObst))
   (setq _coor2DArray __coor2DArray)
   (setq _coor2DArrayTop __coor2DArrayTop)
   (setq _coor2DArrayMid __coor2DArrayMid)
   (setq _coor2DArrayDown __coor2DArrayDown)
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:isContainForwardObst
   (&optional __isContainForwardObst)
   (if __isContainForwardObst (setq _isContainForwardObst __isContainForwardObst)) _isContainForwardObst)
  (:isContainBackwardObst
   (&optional __isContainBackwardObst)
   (if __isContainBackwardObst (setq _isContainBackwardObst __isContainBackwardObst)) _isContainBackwardObst)
  (:coor2DArray
   (&rest __coor2DArray)
   (if (keywordp (car __coor2DArray))
       (send* _coor2DArray __coor2DArray)
     (progn
       (if __coor2DArray (setq _coor2DArray (car __coor2DArray)))
       _coor2DArray)))
  (:coor2DArrayTop
   (&rest __coor2DArrayTop)
   (if (keywordp (car __coor2DArrayTop))
       (send* _coor2DArrayTop __coor2DArrayTop)
     (progn
       (if __coor2DArrayTop (setq _coor2DArrayTop (car __coor2DArrayTop)))
       _coor2DArrayTop)))
  (:coor2DArrayMid
   (&rest __coor2DArrayMid)
   (if (keywordp (car __coor2DArrayMid))
       (send* _coor2DArrayMid __coor2DArrayMid)
     (progn
       (if __coor2DArrayMid (setq _coor2DArrayMid (car __coor2DArrayMid)))
       _coor2DArrayMid)))
  (:coor2DArrayDown
   (&rest __coor2DArrayDown)
   (if (keywordp (car __coor2DArrayDown))
       (send* _coor2DArrayDown __coor2DArrayDown)
     (progn
       (if __coor2DArrayDown (setq _coor2DArrayDown (car __coor2DArrayDown)))
       _coor2DArrayDown)))
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; uint8 _isContainForwardObst
    1
    ;; uint8 _isContainBackwardObst
    1
    ;; comm_msg/coor2D[] _coor2DArray
    (apply #'+ (send-all _coor2DArray :serialization-length)) 4
    ;; comm_msg/coor2D[] _coor2DArrayTop
    (apply #'+ (send-all _coor2DArrayTop :serialization-length)) 4
    ;; comm_msg/coor2D[] _coor2DArrayMid
    (apply #'+ (send-all _coor2DArrayMid :serialization-length)) 4
    ;; comm_msg/coor2D[] _coor2DArrayDown
    (apply #'+ (send-all _coor2DArrayDown :serialization-length)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; uint8 _isContainForwardObst
       (write-byte _isContainForwardObst s)
     ;; uint8 _isContainBackwardObst
       (write-byte _isContainBackwardObst s)
     ;; comm_msg/coor2D[] _coor2DArray
     (write-long (length _coor2DArray) s)
     (dolist (elem _coor2DArray)
       (send elem :serialize s)
       )
     ;; comm_msg/coor2D[] _coor2DArrayTop
     (write-long (length _coor2DArrayTop) s)
     (dolist (elem _coor2DArrayTop)
       (send elem :serialize s)
       )
     ;; comm_msg/coor2D[] _coor2DArrayMid
     (write-long (length _coor2DArrayMid) s)
     (dolist (elem _coor2DArrayMid)
       (send elem :serialize s)
       )
     ;; comm_msg/coor2D[] _coor2DArrayDown
     (write-long (length _coor2DArrayDown) s)
     (dolist (elem _coor2DArrayDown)
       (send elem :serialize s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; uint8 _isContainForwardObst
     (setq _isContainForwardObst (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _isContainBackwardObst
     (setq _isContainBackwardObst (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; comm_msg/coor2D[] _coor2DArray
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _coor2DArray (let (r) (dotimes (i n) (push (instance comm_msg::coor2D :init) r)) r))
     (dolist (elem- _coor2DArray)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;; comm_msg/coor2D[] _coor2DArrayTop
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _coor2DArrayTop (let (r) (dotimes (i n) (push (instance comm_msg::coor2D :init) r)) r))
     (dolist (elem- _coor2DArrayTop)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;; comm_msg/coor2D[] _coor2DArrayMid
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _coor2DArrayMid (let (r) (dotimes (i n) (push (instance comm_msg::coor2D :init) r)) r))
     (dolist (elem- _coor2DArrayMid)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;; comm_msg/coor2D[] _coor2DArrayDown
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _coor2DArrayDown (let (r) (dotimes (i n) (push (instance comm_msg::coor2D :init) r)) r))
     (dolist (elem- _coor2DArrayDown)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;;
   self)
  )

(setf (get comm_msg::coor2DArray :md5sum-) "c15593ec81512cc55beecc6a469f2978")
(setf (get comm_msg::coor2DArray :datatype-) "comm_msg/coor2DArray")
(setf (get comm_msg::coor2DArray :definition-)
      "Header    header
uint8     isContainForwardObst
uint8     isContainBackwardObst
coor2D[]  coor2DArray       #待删除
coor2D[]  coor2DArrayTop
coor2D[]  coor2DArrayMid
coor2D[]  coor2DArrayDown
================================================================================
MSG: std_msgs/Header
# Standard metadata for higher-level stamped data types.
# This is generally used to communicate timestamped data 
# in a particular coordinate frame.
# 
# sequence ID: consecutively increasing ID 
uint32 seq
#Two-integer timestamp that is expressed as:
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
# time-handling sugar is provided by the client library
time stamp
#Frame this data is associated with
string frame_id

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



(provide :comm_msg/coor2DArray "c15593ec81512cc55beecc6a469f2978")


