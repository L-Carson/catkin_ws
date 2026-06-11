; Auto-generated. Do not edit!


(cl:in-package comm_srvs-srv)


;//! \htmlinclude imuOffsetCalibration-request.msg.html

(cl:defclass <imuOffsetCalibration-request> (roslisp-msg-protocol:ros-message)
  ((imuOffsetParam
    :reader imuOffsetParam
    :initarg :imuOffsetParam
    :type comm_srvs-msg:sensorInstallParam
    :initform (cl:make-instance 'comm_srvs-msg:sensorInstallParam)))
)

(cl:defclass imuOffsetCalibration-request (<imuOffsetCalibration-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <imuOffsetCalibration-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'imuOffsetCalibration-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_srvs-srv:<imuOffsetCalibration-request> is deprecated: use comm_srvs-srv:imuOffsetCalibration-request instead.")))

(cl:ensure-generic-function 'imuOffsetParam-val :lambda-list '(m))
(cl:defmethod imuOffsetParam-val ((m <imuOffsetCalibration-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:imuOffsetParam-val is deprecated.  Use comm_srvs-srv:imuOffsetParam instead.")
  (imuOffsetParam m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <imuOffsetCalibration-request>) ostream)
  "Serializes a message object of type '<imuOffsetCalibration-request>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'imuOffsetParam) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <imuOffsetCalibration-request>) istream)
  "Deserializes a message object of type '<imuOffsetCalibration-request>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'imuOffsetParam) istream)
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<imuOffsetCalibration-request>)))
  "Returns string type for a service object of type '<imuOffsetCalibration-request>"
  "comm_srvs/imuOffsetCalibrationRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'imuOffsetCalibration-request)))
  "Returns string type for a service object of type 'imuOffsetCalibration-request"
  "comm_srvs/imuOffsetCalibrationRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<imuOffsetCalibration-request>)))
  "Returns md5sum for a message object of type '<imuOffsetCalibration-request>"
  "c4b920e716f4c5969027af9eec7a747b")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'imuOffsetCalibration-request)))
  "Returns md5sum for a message object of type 'imuOffsetCalibration-request"
  "c4b920e716f4c5969027af9eec7a747b")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<imuOffsetCalibration-request>)))
  "Returns full string definition for message of type '<imuOffsetCalibration-request>"
  (cl:format cl:nil "sensorInstallParam imuOffsetParam~%~%================================================================================~%MSG: comm_srvs/sensorInstallParam~%string  name ~%float32 x~%float32 y~%float32 z~%float32 roll~%float32 pitch~%float32 yaw~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'imuOffsetCalibration-request)))
  "Returns full string definition for message of type 'imuOffsetCalibration-request"
  (cl:format cl:nil "sensorInstallParam imuOffsetParam~%~%================================================================================~%MSG: comm_srvs/sensorInstallParam~%string  name ~%float32 x~%float32 y~%float32 z~%float32 roll~%float32 pitch~%float32 yaw~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <imuOffsetCalibration-request>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'imuOffsetParam))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <imuOffsetCalibration-request>))
  "Converts a ROS message object to a list"
  (cl:list 'imuOffsetCalibration-request
    (cl:cons ':imuOffsetParam (imuOffsetParam msg))
))
;//! \htmlinclude imuOffsetCalibration-response.msg.html

(cl:defclass <imuOffsetCalibration-response> (roslisp-msg-protocol:ros-message)
  ((success
    :reader success
    :initarg :success
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass imuOffsetCalibration-response (<imuOffsetCalibration-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <imuOffsetCalibration-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'imuOffsetCalibration-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_srvs-srv:<imuOffsetCalibration-response> is deprecated: use comm_srvs-srv:imuOffsetCalibration-response instead.")))

(cl:ensure-generic-function 'success-val :lambda-list '(m))
(cl:defmethod success-val ((m <imuOffsetCalibration-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:success-val is deprecated.  Use comm_srvs-srv:success instead.")
  (success m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <imuOffsetCalibration-response>) ostream)
  "Serializes a message object of type '<imuOffsetCalibration-response>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'success) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <imuOffsetCalibration-response>) istream)
  "Deserializes a message object of type '<imuOffsetCalibration-response>"
    (cl:setf (cl:slot-value msg 'success) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<imuOffsetCalibration-response>)))
  "Returns string type for a service object of type '<imuOffsetCalibration-response>"
  "comm_srvs/imuOffsetCalibrationResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'imuOffsetCalibration-response)))
  "Returns string type for a service object of type 'imuOffsetCalibration-response"
  "comm_srvs/imuOffsetCalibrationResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<imuOffsetCalibration-response>)))
  "Returns md5sum for a message object of type '<imuOffsetCalibration-response>"
  "c4b920e716f4c5969027af9eec7a747b")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'imuOffsetCalibration-response)))
  "Returns md5sum for a message object of type 'imuOffsetCalibration-response"
  "c4b920e716f4c5969027af9eec7a747b")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<imuOffsetCalibration-response>)))
  "Returns full string definition for message of type '<imuOffsetCalibration-response>"
  (cl:format cl:nil "bool        success~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'imuOffsetCalibration-response)))
  "Returns full string definition for message of type 'imuOffsetCalibration-response"
  (cl:format cl:nil "bool        success~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <imuOffsetCalibration-response>))
  (cl:+ 0
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <imuOffsetCalibration-response>))
  "Converts a ROS message object to a list"
  (cl:list 'imuOffsetCalibration-response
    (cl:cons ':success (success msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'imuOffsetCalibration)))
  'imuOffsetCalibration-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'imuOffsetCalibration)))
  'imuOffsetCalibration-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'imuOffsetCalibration)))
  "Returns string type for a service object of type '<imuOffsetCalibration>"
  "comm_srvs/imuOffsetCalibration")