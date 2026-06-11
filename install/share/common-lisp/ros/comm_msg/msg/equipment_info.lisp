; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude equipment_info.msg.html

(cl:defclass <equipment_info> (roslisp-msg-protocol:ros-message)
  ((status
    :reader status
    :initarg :status
    :type cl:fixnum
    :initform 0)
   (equipmentType
    :reader equipmentType
    :initarg :equipmentType
    :type cl:fixnum
    :initform 0)
   (QRCodeId
    :reader QRCodeId
    :initarg :QRCodeId
    :type cl:integer
    :initform 0)
   (QRCodePose
    :reader QRCodePose
    :initarg :QRCodePose
    :type comm_msg-msg:pose
    :initform (cl:make-instance 'comm_msg-msg:pose))
   (QRCodeOffset
    :reader QRCodeOffset
    :initarg :QRCodeOffset
    :type comm_msg-msg:coor3D
    :initform (cl:make-instance 'comm_msg-msg:coor3D))
   (reflectiveStripPosLeft
    :reader reflectiveStripPosLeft
    :initarg :reflectiveStripPosLeft
    :type (cl:vector geometry_msgs-msg:Point32)
   :initform (cl:make-array 0 :element-type 'geometry_msgs-msg:Point32 :initial-element (cl:make-instance 'geometry_msgs-msg:Point32)))
   (reflectiveStripPosRight
    :reader reflectiveStripPosRight
    :initarg :reflectiveStripPosRight
    :type (cl:vector geometry_msgs-msg:Point32)
   :initform (cl:make-array 0 :element-type 'geometry_msgs-msg:Point32 :initial-element (cl:make-instance 'geometry_msgs-msg:Point32))))
)

(cl:defclass equipment_info (<equipment_info>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <equipment_info>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'equipment_info)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<equipment_info> is deprecated: use comm_msg-msg:equipment_info instead.")))

(cl:ensure-generic-function 'status-val :lambda-list '(m))
(cl:defmethod status-val ((m <equipment_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:status-val is deprecated.  Use comm_msg-msg:status instead.")
  (status m))

(cl:ensure-generic-function 'equipmentType-val :lambda-list '(m))
(cl:defmethod equipmentType-val ((m <equipment_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:equipmentType-val is deprecated.  Use comm_msg-msg:equipmentType instead.")
  (equipmentType m))

(cl:ensure-generic-function 'QRCodeId-val :lambda-list '(m))
(cl:defmethod QRCodeId-val ((m <equipment_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:QRCodeId-val is deprecated.  Use comm_msg-msg:QRCodeId instead.")
  (QRCodeId m))

(cl:ensure-generic-function 'QRCodePose-val :lambda-list '(m))
(cl:defmethod QRCodePose-val ((m <equipment_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:QRCodePose-val is deprecated.  Use comm_msg-msg:QRCodePose instead.")
  (QRCodePose m))

(cl:ensure-generic-function 'QRCodeOffset-val :lambda-list '(m))
(cl:defmethod QRCodeOffset-val ((m <equipment_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:QRCodeOffset-val is deprecated.  Use comm_msg-msg:QRCodeOffset instead.")
  (QRCodeOffset m))

(cl:ensure-generic-function 'reflectiveStripPosLeft-val :lambda-list '(m))
(cl:defmethod reflectiveStripPosLeft-val ((m <equipment_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:reflectiveStripPosLeft-val is deprecated.  Use comm_msg-msg:reflectiveStripPosLeft instead.")
  (reflectiveStripPosLeft m))

(cl:ensure-generic-function 'reflectiveStripPosRight-val :lambda-list '(m))
(cl:defmethod reflectiveStripPosRight-val ((m <equipment_info>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:reflectiveStripPosRight-val is deprecated.  Use comm_msg-msg:reflectiveStripPosRight instead.")
  (reflectiveStripPosRight m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <equipment_info>) ostream)
  "Serializes a message object of type '<equipment_info>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'status)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'equipmentType)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'QRCodeId)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'QRCodeId)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'QRCodeId)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'QRCodeId)) ostream)
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'QRCodePose) ostream)
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'QRCodeOffset) ostream)
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'reflectiveStripPosLeft))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'reflectiveStripPosLeft))
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'reflectiveStripPosRight))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'reflectiveStripPosRight))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <equipment_info>) istream)
  "Deserializes a message object of type '<equipment_info>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'status)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'equipmentType)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'QRCodeId)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'QRCodeId)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'QRCodeId)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'QRCodeId)) (cl:read-byte istream))
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'QRCodePose) istream)
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'QRCodeOffset) istream)
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'reflectiveStripPosLeft) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'reflectiveStripPosLeft)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'geometry_msgs-msg:Point32))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'reflectiveStripPosRight) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'reflectiveStripPosRight)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'geometry_msgs-msg:Point32))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<equipment_info>)))
  "Returns string type for a message object of type '<equipment_info>"
  "comm_msg/equipment_info")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'equipment_info)))
  "Returns string type for a message object of type 'equipment_info"
  "comm_msg/equipment_info")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<equipment_info>)))
  "Returns md5sum for a message object of type '<equipment_info>"
  "b8804889ce9395f076bb976f79f9f21b")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'equipment_info)))
  "Returns md5sum for a message object of type 'equipment_info"
  "b8804889ce9395f076bb976f79f9f21b")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<equipment_info>)))
  "Returns full string definition for message of type '<equipment_info>"
  (cl:format cl:nil "uint8                   status~%~%# 0是二维码，1是二维码加反光条~%uint8                   equipmentType~%~%uint32                  QRCodeId~%pose                    QRCodePose~%coor3D                  QRCodeOffset~%geometry_msgs/Point32[] reflectiveStripPosLeft~%geometry_msgs/Point32[] reflectiveStripPosRight~%================================================================================~%MSG: comm_msg/pose~%float32 px~%float32 py~%float32 pz~%float32 roll~%float32 pitch~%float32 yaw~%================================================================================~%MSG: comm_msg/coor3D~%float32 pos_x~%float32 pos_y~%float32 pos_z~%================================================================================~%MSG: geometry_msgs/Point32~%# This contains the position of a point in free space(with 32 bits of precision).~%# It is recommeded to use Point wherever possible instead of Point32.  ~%# ~%# This recommendation is to promote interoperability.  ~%#~%# This message is designed to take up less space when sending~%# lots of points at once, as in the case of a PointCloud.  ~%~%float32 x~%float32 y~%float32 z~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'equipment_info)))
  "Returns full string definition for message of type 'equipment_info"
  (cl:format cl:nil "uint8                   status~%~%# 0是二维码，1是二维码加反光条~%uint8                   equipmentType~%~%uint32                  QRCodeId~%pose                    QRCodePose~%coor3D                  QRCodeOffset~%geometry_msgs/Point32[] reflectiveStripPosLeft~%geometry_msgs/Point32[] reflectiveStripPosRight~%================================================================================~%MSG: comm_msg/pose~%float32 px~%float32 py~%float32 pz~%float32 roll~%float32 pitch~%float32 yaw~%================================================================================~%MSG: comm_msg/coor3D~%float32 pos_x~%float32 pos_y~%float32 pos_z~%================================================================================~%MSG: geometry_msgs/Point32~%# This contains the position of a point in free space(with 32 bits of precision).~%# It is recommeded to use Point wherever possible instead of Point32.  ~%# ~%# This recommendation is to promote interoperability.  ~%#~%# This message is designed to take up less space when sending~%# lots of points at once, as in the case of a PointCloud.  ~%~%float32 x~%float32 y~%float32 z~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <equipment_info>))
  (cl:+ 0
     1
     1
     4
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'QRCodePose))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'QRCodeOffset))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'reflectiveStripPosLeft) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'reflectiveStripPosRight) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <equipment_info>))
  "Converts a ROS message object to a list"
  (cl:list 'equipment_info
    (cl:cons ':status (status msg))
    (cl:cons ':equipmentType (equipmentType msg))
    (cl:cons ':QRCodeId (QRCodeId msg))
    (cl:cons ':QRCodePose (QRCodePose msg))
    (cl:cons ':QRCodeOffset (QRCodeOffset msg))
    (cl:cons ':reflectiveStripPosLeft (reflectiveStripPosLeft msg))
    (cl:cons ':reflectiveStripPosRight (reflectiveStripPosRight msg))
))
