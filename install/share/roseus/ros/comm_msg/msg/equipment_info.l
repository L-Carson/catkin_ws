;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::equipment_info)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'equipment_info (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::EQUIPMENT_INFO")
  (make-package "COMM_MSG::EQUIPMENT_INFO"))

(in-package "ROS")
;;//! \htmlinclude equipment_info.msg.html
(if (not (find-package "GEOMETRY_MSGS"))
  (ros::roseus-add-msgs "geometry_msgs"))


(defclass comm_msg::equipment_info
  :super ros::object
  :slots (_status _equipmentType _QRCodeId _QRCodePose _QRCodeOffset _reflectiveStripPosLeft _reflectiveStripPosRight ))

(defmethod comm_msg::equipment_info
  (:init
   (&key
    ((:status __status) 0)
    ((:equipmentType __equipmentType) 0)
    ((:QRCodeId __QRCodeId) 0)
    ((:QRCodePose __QRCodePose) (instance comm_msg::pose :init))
    ((:QRCodeOffset __QRCodeOffset) (instance comm_msg::coor3D :init))
    ((:reflectiveStripPosLeft __reflectiveStripPosLeft) ())
    ((:reflectiveStripPosRight __reflectiveStripPosRight) ())
    )
   (send-super :init)
   (setq _status (round __status))
   (setq _equipmentType (round __equipmentType))
   (setq _QRCodeId (round __QRCodeId))
   (setq _QRCodePose __QRCodePose)
   (setq _QRCodeOffset __QRCodeOffset)
   (setq _reflectiveStripPosLeft __reflectiveStripPosLeft)
   (setq _reflectiveStripPosRight __reflectiveStripPosRight)
   self)
  (:status
   (&optional __status)
   (if __status (setq _status __status)) _status)
  (:equipmentType
   (&optional __equipmentType)
   (if __equipmentType (setq _equipmentType __equipmentType)) _equipmentType)
  (:QRCodeId
   (&optional __QRCodeId)
   (if __QRCodeId (setq _QRCodeId __QRCodeId)) _QRCodeId)
  (:QRCodePose
   (&rest __QRCodePose)
   (if (keywordp (car __QRCodePose))
       (send* _QRCodePose __QRCodePose)
     (progn
       (if __QRCodePose (setq _QRCodePose (car __QRCodePose)))
       _QRCodePose)))
  (:QRCodeOffset
   (&rest __QRCodeOffset)
   (if (keywordp (car __QRCodeOffset))
       (send* _QRCodeOffset __QRCodeOffset)
     (progn
       (if __QRCodeOffset (setq _QRCodeOffset (car __QRCodeOffset)))
       _QRCodeOffset)))
  (:reflectiveStripPosLeft
   (&rest __reflectiveStripPosLeft)
   (if (keywordp (car __reflectiveStripPosLeft))
       (send* _reflectiveStripPosLeft __reflectiveStripPosLeft)
     (progn
       (if __reflectiveStripPosLeft (setq _reflectiveStripPosLeft (car __reflectiveStripPosLeft)))
       _reflectiveStripPosLeft)))
  (:reflectiveStripPosRight
   (&rest __reflectiveStripPosRight)
   (if (keywordp (car __reflectiveStripPosRight))
       (send* _reflectiveStripPosRight __reflectiveStripPosRight)
     (progn
       (if __reflectiveStripPosRight (setq _reflectiveStripPosRight (car __reflectiveStripPosRight)))
       _reflectiveStripPosRight)))
  (:serialization-length
   ()
   (+
    ;; uint8 _status
    1
    ;; uint8 _equipmentType
    1
    ;; uint32 _QRCodeId
    4
    ;; comm_msg/pose _QRCodePose
    (send _QRCodePose :serialization-length)
    ;; comm_msg/coor3D _QRCodeOffset
    (send _QRCodeOffset :serialization-length)
    ;; geometry_msgs/Point32[] _reflectiveStripPosLeft
    (apply #'+ (send-all _reflectiveStripPosLeft :serialization-length)) 4
    ;; geometry_msgs/Point32[] _reflectiveStripPosRight
    (apply #'+ (send-all _reflectiveStripPosRight :serialization-length)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint8 _status
       (write-byte _status s)
     ;; uint8 _equipmentType
       (write-byte _equipmentType s)
     ;; uint32 _QRCodeId
       (write-long _QRCodeId s)
     ;; comm_msg/pose _QRCodePose
       (send _QRCodePose :serialize s)
     ;; comm_msg/coor3D _QRCodeOffset
       (send _QRCodeOffset :serialize s)
     ;; geometry_msgs/Point32[] _reflectiveStripPosLeft
     (write-long (length _reflectiveStripPosLeft) s)
     (dolist (elem _reflectiveStripPosLeft)
       (send elem :serialize s)
       )
     ;; geometry_msgs/Point32[] _reflectiveStripPosRight
     (write-long (length _reflectiveStripPosRight) s)
     (dolist (elem _reflectiveStripPosRight)
       (send elem :serialize s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint8 _status
     (setq _status (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _equipmentType
     (setq _equipmentType (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint32 _QRCodeId
     (setq _QRCodeId (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; comm_msg/pose _QRCodePose
     (send _QRCodePose :deserialize buf ptr-) (incf ptr- (send _QRCodePose :serialization-length))
   ;; comm_msg/coor3D _QRCodeOffset
     (send _QRCodeOffset :deserialize buf ptr-) (incf ptr- (send _QRCodeOffset :serialization-length))
   ;; geometry_msgs/Point32[] _reflectiveStripPosLeft
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _reflectiveStripPosLeft (let (r) (dotimes (i n) (push (instance geometry_msgs::Point32 :init) r)) r))
     (dolist (elem- _reflectiveStripPosLeft)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;; geometry_msgs/Point32[] _reflectiveStripPosRight
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _reflectiveStripPosRight (let (r) (dotimes (i n) (push (instance geometry_msgs::Point32 :init) r)) r))
     (dolist (elem- _reflectiveStripPosRight)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;;
   self)
  )

(setf (get comm_msg::equipment_info :md5sum-) "b8804889ce9395f076bb976f79f9f21b")
(setf (get comm_msg::equipment_info :datatype-) "comm_msg/equipment_info")
(setf (get comm_msg::equipment_info :definition-)
      "uint8                   status

# 0是二维码，1是二维码加反光条
uint8                   equipmentType

uint32                  QRCodeId
pose                    QRCodePose
coor3D                  QRCodeOffset
geometry_msgs/Point32[] reflectiveStripPosLeft
geometry_msgs/Point32[] reflectiveStripPosRight
================================================================================
MSG: comm_msg/pose
float32 px
float32 py
float32 pz
float32 roll
float32 pitch
float32 yaw
================================================================================
MSG: comm_msg/coor3D
float32 pos_x
float32 pos_y
float32 pos_z
================================================================================
MSG: geometry_msgs/Point32
# This contains the position of a point in free space(with 32 bits of precision).
# It is recommeded to use Point wherever possible instead of Point32.  
# 
# This recommendation is to promote interoperability.  
#
# This message is designed to take up less space when sending
# lots of points at once, as in the case of a PointCloud.  

float32 x
float32 y
float32 z
")



(provide :comm_msg/equipment_info "b8804889ce9395f076bb976f79f9f21b")


