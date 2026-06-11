; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude roborExtriParam.msg.html

(cl:defclass <roborExtriParam> (roslisp-msg-protocol:ros-message)
  ((gnss
    :reader gnss
    :initarg :gnss
    :type comm_msg-msg:sensorExtriParam
    :initform (cl:make-instance 'comm_msg-msg:sensorExtriParam))
   (imu
    :reader imu
    :initarg :imu
    :type comm_msg-msg:sensorExtriParam
    :initform (cl:make-instance 'comm_msg-msg:sensorExtriParam))
   (lidar
    :reader lidar
    :initarg :lidar
    :type comm_msg-msg:sensorExtriParam
    :initform (cl:make-instance 'comm_msg-msg:sensorExtriParam)))
)

(cl:defclass roborExtriParam (<roborExtriParam>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <roborExtriParam>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'roborExtriParam)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<roborExtriParam> is deprecated: use comm_msg-msg:roborExtriParam instead.")))

(cl:ensure-generic-function 'gnss-val :lambda-list '(m))
(cl:defmethod gnss-val ((m <roborExtriParam>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:gnss-val is deprecated.  Use comm_msg-msg:gnss instead.")
  (gnss m))

(cl:ensure-generic-function 'imu-val :lambda-list '(m))
(cl:defmethod imu-val ((m <roborExtriParam>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:imu-val is deprecated.  Use comm_msg-msg:imu instead.")
  (imu m))

(cl:ensure-generic-function 'lidar-val :lambda-list '(m))
(cl:defmethod lidar-val ((m <roborExtriParam>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:lidar-val is deprecated.  Use comm_msg-msg:lidar instead.")
  (lidar m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <roborExtriParam>) ostream)
  "Serializes a message object of type '<roborExtriParam>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'gnss) ostream)
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'imu) ostream)
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'lidar) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <roborExtriParam>) istream)
  "Deserializes a message object of type '<roborExtriParam>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'gnss) istream)
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'imu) istream)
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'lidar) istream)
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<roborExtriParam>)))
  "Returns string type for a message object of type '<roborExtriParam>"
  "comm_msg/roborExtriParam")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'roborExtriParam)))
  "Returns string type for a message object of type 'roborExtriParam"
  "comm_msg/roborExtriParam")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<roborExtriParam>)))
  "Returns md5sum for a message object of type '<roborExtriParam>"
  "ad756381b7e6a7c7a09b59fa09842fbd")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'roborExtriParam)))
  "Returns md5sum for a message object of type 'roborExtriParam"
  "ad756381b7e6a7c7a09b59fa09842fbd")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<roborExtriParam>)))
  "Returns full string definition for message of type '<roborExtriParam>"
  (cl:format cl:nil "sensorExtriParam gnss~%sensorExtriParam imu~%sensorExtriParam lidar~%================================================================================~%MSG: comm_msg/sensorExtriParam~%bool isSupport ~%float32 x~%float32 y~%float32 z~%float32 roll~%float32 pitch~%float32 yaw~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'roborExtriParam)))
  "Returns full string definition for message of type 'roborExtriParam"
  (cl:format cl:nil "sensorExtriParam gnss~%sensorExtriParam imu~%sensorExtriParam lidar~%================================================================================~%MSG: comm_msg/sensorExtriParam~%bool isSupport ~%float32 x~%float32 y~%float32 z~%float32 roll~%float32 pitch~%float32 yaw~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <roborExtriParam>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'gnss))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'imu))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'lidar))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <roborExtriParam>))
  "Converts a ROS message object to a list"
  (cl:list 'roborExtriParam
    (cl:cons ':gnss (gnss msg))
    (cl:cons ':imu (imu msg))
    (cl:cons ':lidar (lidar msg))
))
