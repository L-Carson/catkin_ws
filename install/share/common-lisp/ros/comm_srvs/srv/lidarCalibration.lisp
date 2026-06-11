; Auto-generated. Do not edit!


(cl:in-package comm_srvs-srv)


;//! \htmlinclude lidarCalibration-request.msg.html

(cl:defclass <lidarCalibration-request> (roslisp-msg-protocol:ros-message)
  ((lidarsClbtParam
    :reader lidarsClbtParam
    :initarg :lidarsClbtParam
    :type (cl:vector comm_srvs-msg:LidarInstallParam)
   :initform (cl:make-array 0 :element-type 'comm_srvs-msg:LidarInstallParam :initial-element (cl:make-instance 'comm_srvs-msg:LidarInstallParam))))
)

(cl:defclass lidarCalibration-request (<lidarCalibration-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <lidarCalibration-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'lidarCalibration-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_srvs-srv:<lidarCalibration-request> is deprecated: use comm_srvs-srv:lidarCalibration-request instead.")))

(cl:ensure-generic-function 'lidarsClbtParam-val :lambda-list '(m))
(cl:defmethod lidarsClbtParam-val ((m <lidarCalibration-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:lidarsClbtParam-val is deprecated.  Use comm_srvs-srv:lidarsClbtParam instead.")
  (lidarsClbtParam m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <lidarCalibration-request>) ostream)
  "Serializes a message object of type '<lidarCalibration-request>"
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'lidarsClbtParam))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'lidarsClbtParam))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <lidarCalibration-request>) istream)
  "Deserializes a message object of type '<lidarCalibration-request>"
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'lidarsClbtParam) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'lidarsClbtParam)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'comm_srvs-msg:LidarInstallParam))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<lidarCalibration-request>)))
  "Returns string type for a service object of type '<lidarCalibration-request>"
  "comm_srvs/lidarCalibrationRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'lidarCalibration-request)))
  "Returns string type for a service object of type 'lidarCalibration-request"
  "comm_srvs/lidarCalibrationRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<lidarCalibration-request>)))
  "Returns md5sum for a message object of type '<lidarCalibration-request>"
  "a672746d2ead307649fd58de19b13665")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'lidarCalibration-request)))
  "Returns md5sum for a message object of type 'lidarCalibration-request"
  "a672746d2ead307649fd58de19b13665")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<lidarCalibration-request>)))
  "Returns full string definition for message of type '<lidarCalibration-request>"
  (cl:format cl:nil "LidarInstallParam[] lidarsClbtParam~%~%================================================================================~%MSG: comm_srvs/LidarInstallParam~%string  name    #MainLidar BlindLidar BlindLidar2~%string  serialNum~%float32 x~%float32 y~%float32 z~%float32 roll~%float32 pitch~%float32 yaw~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'lidarCalibration-request)))
  "Returns full string definition for message of type 'lidarCalibration-request"
  (cl:format cl:nil "LidarInstallParam[] lidarsClbtParam~%~%================================================================================~%MSG: comm_srvs/LidarInstallParam~%string  name    #MainLidar BlindLidar BlindLidar2~%string  serialNum~%float32 x~%float32 y~%float32 z~%float32 roll~%float32 pitch~%float32 yaw~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <lidarCalibration-request>))
  (cl:+ 0
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'lidarsClbtParam) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <lidarCalibration-request>))
  "Converts a ROS message object to a list"
  (cl:list 'lidarCalibration-request
    (cl:cons ':lidarsClbtParam (lidarsClbtParam msg))
))
;//! \htmlinclude lidarCalibration-response.msg.html

(cl:defclass <lidarCalibration-response> (roslisp-msg-protocol:ros-message)
  ((success
    :reader success
    :initarg :success
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass lidarCalibration-response (<lidarCalibration-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <lidarCalibration-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'lidarCalibration-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_srvs-srv:<lidarCalibration-response> is deprecated: use comm_srvs-srv:lidarCalibration-response instead.")))

(cl:ensure-generic-function 'success-val :lambda-list '(m))
(cl:defmethod success-val ((m <lidarCalibration-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:success-val is deprecated.  Use comm_srvs-srv:success instead.")
  (success m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <lidarCalibration-response>) ostream)
  "Serializes a message object of type '<lidarCalibration-response>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'success) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <lidarCalibration-response>) istream)
  "Deserializes a message object of type '<lidarCalibration-response>"
    (cl:setf (cl:slot-value msg 'success) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<lidarCalibration-response>)))
  "Returns string type for a service object of type '<lidarCalibration-response>"
  "comm_srvs/lidarCalibrationResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'lidarCalibration-response)))
  "Returns string type for a service object of type 'lidarCalibration-response"
  "comm_srvs/lidarCalibrationResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<lidarCalibration-response>)))
  "Returns md5sum for a message object of type '<lidarCalibration-response>"
  "a672746d2ead307649fd58de19b13665")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'lidarCalibration-response)))
  "Returns md5sum for a message object of type 'lidarCalibration-response"
  "a672746d2ead307649fd58de19b13665")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<lidarCalibration-response>)))
  "Returns full string definition for message of type '<lidarCalibration-response>"
  (cl:format cl:nil "bool        success~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'lidarCalibration-response)))
  "Returns full string definition for message of type 'lidarCalibration-response"
  (cl:format cl:nil "bool        success~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <lidarCalibration-response>))
  (cl:+ 0
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <lidarCalibration-response>))
  "Converts a ROS message object to a list"
  (cl:list 'lidarCalibration-response
    (cl:cons ':success (success msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'lidarCalibration)))
  'lidarCalibration-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'lidarCalibration)))
  'lidarCalibration-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'lidarCalibration)))
  "Returns string type for a service object of type '<lidarCalibration>"
  "comm_srvs/lidarCalibration")