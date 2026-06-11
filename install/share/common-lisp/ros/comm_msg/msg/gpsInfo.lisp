; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude gpsInfo.msg.html

(cl:defclass <gpsInfo> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (satelliteNum
    :reader satelliteNum
    :initarg :satelliteNum
    :type cl:fixnum
    :initform 0)
   (rsltType
    :reader rsltType
    :initarg :rsltType
    :type cl:fixnum
    :initform 0)
   (longitude
    :reader longitude
    :initarg :longitude
    :type cl:float
    :initform 0.0)
   (latitude
    :reader latitude
    :initarg :latitude
    :type cl:float
    :initform 0.0)
   (altitude
    :reader altitude
    :initarg :altitude
    :type cl:float
    :initform 0.0)
   (yamAngle
    :reader yamAngle
    :initarg :yamAngle
    :type cl:float
    :initform 0.0))
)

(cl:defclass gpsInfo (<gpsInfo>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <gpsInfo>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'gpsInfo)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<gpsInfo> is deprecated: use comm_msg-msg:gpsInfo instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <gpsInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:header-val is deprecated.  Use comm_msg-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'satelliteNum-val :lambda-list '(m))
(cl:defmethod satelliteNum-val ((m <gpsInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:satelliteNum-val is deprecated.  Use comm_msg-msg:satelliteNum instead.")
  (satelliteNum m))

(cl:ensure-generic-function 'rsltType-val :lambda-list '(m))
(cl:defmethod rsltType-val ((m <gpsInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:rsltType-val is deprecated.  Use comm_msg-msg:rsltType instead.")
  (rsltType m))

(cl:ensure-generic-function 'longitude-val :lambda-list '(m))
(cl:defmethod longitude-val ((m <gpsInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:longitude-val is deprecated.  Use comm_msg-msg:longitude instead.")
  (longitude m))

(cl:ensure-generic-function 'latitude-val :lambda-list '(m))
(cl:defmethod latitude-val ((m <gpsInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:latitude-val is deprecated.  Use comm_msg-msg:latitude instead.")
  (latitude m))

(cl:ensure-generic-function 'altitude-val :lambda-list '(m))
(cl:defmethod altitude-val ((m <gpsInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:altitude-val is deprecated.  Use comm_msg-msg:altitude instead.")
  (altitude m))

(cl:ensure-generic-function 'yamAngle-val :lambda-list '(m))
(cl:defmethod yamAngle-val ((m <gpsInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:yamAngle-val is deprecated.  Use comm_msg-msg:yamAngle instead.")
  (yamAngle m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <gpsInfo>) ostream)
  "Serializes a message object of type '<gpsInfo>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'satelliteNum)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'rsltType)) ostream)
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'longitude))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'latitude))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'altitude))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'yamAngle))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <gpsInfo>) istream)
  "Deserializes a message object of type '<gpsInfo>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'satelliteNum)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'rsltType)) (cl:read-byte istream))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'longitude) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'latitude) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'altitude) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'yamAngle) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<gpsInfo>)))
  "Returns string type for a message object of type '<gpsInfo>"
  "comm_msg/gpsInfo")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'gpsInfo)))
  "Returns string type for a message object of type 'gpsInfo"
  "comm_msg/gpsInfo")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<gpsInfo>)))
  "Returns md5sum for a message object of type '<gpsInfo>"
  "7af55e227f3750810313d5dec1b64d15")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'gpsInfo)))
  "Returns md5sum for a message object of type 'gpsInfo"
  "7af55e227f3750810313d5dec1b64d15")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<gpsInfo>)))
  "Returns full string definition for message of type '<gpsInfo>"
  (cl:format cl:nil "Header  header~%~%# satellite num~%uint8 satelliteNum~%~%# Rslt Type~%uint8 rsltType~%~%# ~%float64 longitude~%float64 latitude~%float32 altitude~%~%#~%float32 yamAngle~%~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'gpsInfo)))
  "Returns full string definition for message of type 'gpsInfo"
  (cl:format cl:nil "Header  header~%~%# satellite num~%uint8 satelliteNum~%~%# Rslt Type~%uint8 rsltType~%~%# ~%float64 longitude~%float64 latitude~%float32 altitude~%~%#~%float32 yamAngle~%~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <gpsInfo>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     1
     1
     8
     8
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <gpsInfo>))
  "Converts a ROS message object to a list"
  (cl:list 'gpsInfo
    (cl:cons ':header (header msg))
    (cl:cons ':satelliteNum (satelliteNum msg))
    (cl:cons ':rsltType (rsltType msg))
    (cl:cons ':longitude (longitude msg))
    (cl:cons ':latitude (latitude msg))
    (cl:cons ':altitude (altitude msg))
    (cl:cons ':yamAngle (yamAngle msg))
))
