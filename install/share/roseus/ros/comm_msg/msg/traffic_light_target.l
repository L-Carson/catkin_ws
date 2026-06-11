;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::traffic_light_target)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'traffic_light_target (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::TRAFFIC_LIGHT_TARGET")
  (make-package "COMM_MSG::TRAFFIC_LIGHT_TARGET"))

(in-package "ROS")
;;//! \htmlinclude traffic_light_target.msg.html


(defclass comm_msg::traffic_light_target
  :super ros::object
  :slots (_is_open _pixel_bias _rect_bias _target _nears ))

(defmethod comm_msg::traffic_light_target
  (:init
   (&key
    ((:is_open __is_open) 0)
    ((:pixel_bias __pixel_bias) 0.0)
    ((:rect_bias __rect_bias) 0.0)
    ((:target __target) (instance comm_msg::traffic_light_info :init))
    ((:nears __nears) ())
    )
   (send-super :init)
   (setq _is_open (round __is_open))
   (setq _pixel_bias (float __pixel_bias))
   (setq _rect_bias (float __rect_bias))
   (setq _target __target)
   (setq _nears __nears)
   self)
  (:is_open
   (&optional __is_open)
   (if __is_open (setq _is_open __is_open)) _is_open)
  (:pixel_bias
   (&optional __pixel_bias)
   (if __pixel_bias (setq _pixel_bias __pixel_bias)) _pixel_bias)
  (:rect_bias
   (&optional __rect_bias)
   (if __rect_bias (setq _rect_bias __rect_bias)) _rect_bias)
  (:target
   (&rest __target)
   (if (keywordp (car __target))
       (send* _target __target)
     (progn
       (if __target (setq _target (car __target)))
       _target)))
  (:nears
   (&rest __nears)
   (if (keywordp (car __nears))
       (send* _nears __nears)
     (progn
       (if __nears (setq _nears (car __nears)))
       _nears)))
  (:serialization-length
   ()
   (+
    ;; uint8 _is_open
    1
    ;; float32 _pixel_bias
    4
    ;; float32 _rect_bias
    4
    ;; comm_msg/traffic_light_info _target
    (send _target :serialization-length)
    ;; comm_msg/traffic_light_info[] _nears
    (apply #'+ (send-all _nears :serialization-length)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint8 _is_open
       (write-byte _is_open s)
     ;; float32 _pixel_bias
       (sys::poke _pixel_bias (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _rect_bias
       (sys::poke _rect_bias (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; comm_msg/traffic_light_info _target
       (send _target :serialize s)
     ;; comm_msg/traffic_light_info[] _nears
     (write-long (length _nears) s)
     (dolist (elem _nears)
       (send elem :serialize s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint8 _is_open
     (setq _is_open (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; float32 _pixel_bias
     (setq _pixel_bias (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _rect_bias
     (setq _rect_bias (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; comm_msg/traffic_light_info _target
     (send _target :deserialize buf ptr-) (incf ptr- (send _target :serialization-length))
   ;; comm_msg/traffic_light_info[] _nears
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _nears (let (r) (dotimes (i n) (push (instance comm_msg::traffic_light_info :init) r)) r))
     (dolist (elem- _nears)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;;
   self)
  )

(setf (get comm_msg::traffic_light_target :md5sum-) "5c8d3210265e381480cecfd925687150")
(setf (get comm_msg::traffic_light_target :datatype-) "comm_msg/traffic_light_target")
(setf (get comm_msg::traffic_light_target :definition-)
      "uint8                   is_open     # 是否开启交通灯检测：0(关闭) 1(开启)
float32                 pixel_bias  # 像素偏差
float32                 rect_bias   # 检测框匹配率偏差
traffic_light_info      target      # 目标交通灯信息
traffic_light_info[]    nears       # 周边交通灯信息
================================================================================
MSG: comm_msg/traffic_light_info
uint8    type           # 交通灯显示类型：1(读秒类型) 2(频闪类型) 3(高频亮灭型)
float32  pos_x          # 交通灯中在地图上的坐标x
float32  pos_y          # 交通灯中在地图上的坐标y
float32  pos_z          # 交通灯中在地图上的坐标z
float32  row_length     # 交通灯横向长度
float32  col_length     # 交通灯纵向长度
")



(provide :comm_msg/traffic_light_target "5c8d3210265e381480cecfd925687150")


