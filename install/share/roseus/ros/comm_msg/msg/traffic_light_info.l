;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::traffic_light_info)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'traffic_light_info (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::TRAFFIC_LIGHT_INFO")
  (make-package "COMM_MSG::TRAFFIC_LIGHT_INFO"))

(in-package "ROS")
;;//! \htmlinclude traffic_light_info.msg.html


(defclass comm_msg::traffic_light_info
  :super ros::object
  :slots (_type _pos_x _pos_y _pos_z _row_length _col_length ))

(defmethod comm_msg::traffic_light_info
  (:init
   (&key
    ((:type __type) 0)
    ((:pos_x __pos_x) 0.0)
    ((:pos_y __pos_y) 0.0)
    ((:pos_z __pos_z) 0.0)
    ((:row_length __row_length) 0.0)
    ((:col_length __col_length) 0.0)
    )
   (send-super :init)
   (setq _type (round __type))
   (setq _pos_x (float __pos_x))
   (setq _pos_y (float __pos_y))
   (setq _pos_z (float __pos_z))
   (setq _row_length (float __row_length))
   (setq _col_length (float __col_length))
   self)
  (:type
   (&optional __type)
   (if __type (setq _type __type)) _type)
  (:pos_x
   (&optional __pos_x)
   (if __pos_x (setq _pos_x __pos_x)) _pos_x)
  (:pos_y
   (&optional __pos_y)
   (if __pos_y (setq _pos_y __pos_y)) _pos_y)
  (:pos_z
   (&optional __pos_z)
   (if __pos_z (setq _pos_z __pos_z)) _pos_z)
  (:row_length
   (&optional __row_length)
   (if __row_length (setq _row_length __row_length)) _row_length)
  (:col_length
   (&optional __col_length)
   (if __col_length (setq _col_length __col_length)) _col_length)
  (:serialization-length
   ()
   (+
    ;; uint8 _type
    1
    ;; float32 _pos_x
    4
    ;; float32 _pos_y
    4
    ;; float32 _pos_z
    4
    ;; float32 _row_length
    4
    ;; float32 _col_length
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint8 _type
       (write-byte _type s)
     ;; float32 _pos_x
       (sys::poke _pos_x (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _pos_y
       (sys::poke _pos_y (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _pos_z
       (sys::poke _pos_z (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _row_length
       (sys::poke _row_length (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _col_length
       (sys::poke _col_length (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint8 _type
     (setq _type (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; float32 _pos_x
     (setq _pos_x (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _pos_y
     (setq _pos_y (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _pos_z
     (setq _pos_z (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _row_length
     (setq _row_length (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _col_length
     (setq _col_length (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get comm_msg::traffic_light_info :md5sum-) "b4755f326c42173c8ecd115dcd1f1001")
(setf (get comm_msg::traffic_light_info :datatype-) "comm_msg/traffic_light_info")
(setf (get comm_msg::traffic_light_info :definition-)
      "uint8    type           # 交通灯显示类型：1(读秒类型) 2(频闪类型) 3(高频亮灭型)
float32  pos_x          # 交通灯中在地图上的坐标x
float32  pos_y          # 交通灯中在地图上的坐标y
float32  pos_z          # 交通灯中在地图上的坐标z
float32  row_length     # 交通灯横向长度
float32  col_length     # 交通灯纵向长度
")



(provide :comm_msg/traffic_light_info "b4755f326c42173c8ecd115dcd1f1001")


