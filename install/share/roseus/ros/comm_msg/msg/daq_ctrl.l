;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::daq_ctrl)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'daq_ctrl (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::DAQ_CTRL")
  (make-package "COMM_MSG::DAQ_CTRL"))

(in-package "ROS")
;;//! \htmlinclude daq_ctrl.msg.html


(defclass comm_msg::daq_ctrl
  :super ros::object
  :slots (_daq_fusion _daq_hz _daq_camera _daq_lidar ))

(defmethod comm_msg::daq_ctrl
  (:init
   (&key
    ((:daq_fusion __daq_fusion) nil)
    ((:daq_hz __daq_hz) 0)
    ((:daq_camera __daq_camera) (make-array 0 :initial-element 0 :element-type :char))
    ((:daq_lidar __daq_lidar) (make-array 0 :initial-element 0 :element-type :char))
    )
   (send-super :init)
   (setq _daq_fusion __daq_fusion)
   (setq _daq_hz (round __daq_hz))
   (setq _daq_camera __daq_camera)
   (setq _daq_lidar __daq_lidar)
   self)
  (:daq_fusion
   (&optional (__daq_fusion :null))
   (if (not (eq __daq_fusion :null)) (setq _daq_fusion __daq_fusion)) _daq_fusion)
  (:daq_hz
   (&optional __daq_hz)
   (if __daq_hz (setq _daq_hz __daq_hz)) _daq_hz)
  (:daq_camera
   (&optional __daq_camera)
   (if __daq_camera (setq _daq_camera __daq_camera)) _daq_camera)
  (:daq_lidar
   (&optional __daq_lidar)
   (if __daq_lidar (setq _daq_lidar __daq_lidar)) _daq_lidar)
  (:serialization-length
   ()
   (+
    ;; bool _daq_fusion
    1
    ;; uint8 _daq_hz
    1
    ;; uint8[] _daq_camera
    (* 1    (length _daq_camera)) 4
    ;; uint8[] _daq_lidar
    (* 1    (length _daq_lidar)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; bool _daq_fusion
       (if _daq_fusion (write-byte -1 s) (write-byte 0 s))
     ;; uint8 _daq_hz
       (write-byte _daq_hz s)
     ;; uint8[] _daq_camera
     (write-long (length _daq_camera) s)
     (princ _daq_camera s)
     ;; uint8[] _daq_lidar
     (write-long (length _daq_lidar) s)
     (princ _daq_lidar s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; bool _daq_fusion
     (setq _daq_fusion (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;; uint8 _daq_hz
     (setq _daq_hz (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8[] _daq_camera
   (let ((n (sys::peek buf ptr- :integer))) (incf ptr- 4)
     (setq _daq_camera (make-array n :element-type :char))
     (replace _daq_camera buf :start2 ptr-) (incf ptr- n))
   ;; uint8[] _daq_lidar
   (let ((n (sys::peek buf ptr- :integer))) (incf ptr- 4)
     (setq _daq_lidar (make-array n :element-type :char))
     (replace _daq_lidar buf :start2 ptr-) (incf ptr- n))
   ;;
   self)
  )

(setf (get comm_msg::daq_ctrl :md5sum-) "b65aad8c511a16f789728648f67ea4d6")
(setf (get comm_msg::daq_ctrl :datatype-) "comm_msg/daq_ctrl")
(setf (get comm_msg::daq_ctrl :definition-)
      "# e.g. begin: rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl \"{daq_fusion: 1, daq_hz: 1, daq_camera:[1,2,3,4,5,6,7,8,9,10], daq_lidar:[1,2,3,4,5]}\"     # 相机+雷达：时间戳需对齐
# e.g. begin: rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl \"{daq_fusion: 0, daq_hz: 1, daq_camera:[1,2,3,4,5,6,7,8,9,10], daq_lidar:[1,2,3,4,5]}\"     # 相机+雷达：时间戳无需对齐

# e.g. begin: rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl \"{daq_fusion: 1, daq_hz: 1, daq_camera:[1,2,3,4,5,6,7,8,9,10], daq_lidar:[]}\"              # 相机：时间戳需对齐
# e.g. begin: rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl \"{daq_fusion: 0, daq_hz: 1, daq_camera:[1,2,3,4,5,6,7,8,9,10], daq_lidar:[]}\"              # 相机：时间戳无需对齐

# e.g. begin: rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl \"{daq_fusion: 1, daq_hz: 1, daq_camera:[], daq_lidar:[1,2,3,4,5]}\"                         # 雷达：时间戳需对齐
# e.g. begin: rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl \"{daq_fusion: 0, daq_hz: 1, daq_camera:[], daq_lidar:[1,2,3,4,5]}\"                         # 雷达：时间戳无需对齐

# e.g. end:   rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl \"{daq_fusion: 1, daq_hz: 1, daq_camera:[], daq_lidar:[]}\"                                  # 结束采集
# e.g. end:   rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl \"{daq_fusion: 0, daq_hz: 1, daq_camera:[], daq_lidar:[]}\"                                  # 结束采集（同上）

bool    daq_fusion      # 采集融合数据（相机+雷达：时间戳对齐）
uint8   daq_hz          # 采集频率
uint8[] daq_camera      # 采集相机数据
uint8[] daq_lidar       # 采集雷达数据

")



(provide :comm_msg/daq_ctrl "b65aad8c511a16f789728648f67ea4d6")


