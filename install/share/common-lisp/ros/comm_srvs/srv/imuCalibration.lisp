; Auto-generated. Do not edit!


(cl:in-package comm_srvs-srv)


;//! \htmlinclude imuCalibration-request.msg.html

(cl:defclass <imuCalibration-request> (roslisp-msg-protocol:ros-message)
  ((imuClbtParam
    :reader imuClbtParam
    :initarg :imuClbtParam
    :type comm_srvs-msg:sensorInstallParam
    :initform (cl:make-instance 'comm_srvs-msg:sensorInstallParam)))
)

(cl:defclass imuCalibration-request (<imuCalibration-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <imuCalibration-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'imuCalibration-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_srvs-srv:<imuCalibration-request> is deprecated: use comm_srvs-srv:imuCalibration-request instead.")))

(cl:ensure-generic-function 'imuClbtParam-val :lambda-list '(m))
(cl:defmethod imuClbtParam-val ((m <imuCalibration-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:imuClbtParam-val is deprecated.  Use comm_srvs-srv:imuClbtParam instead.")
  (imuClbtParam m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <imuCalibration-request>) ostream)
  "Serializes a message object of type '<imuCalibration-request>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'imuClbtParam) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <imuCalibration-request>) istream)
  "Deserializes a message object of type '<imuCalibration-request>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'imuClbtParam) istream)
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<imuCalibration-request>)))
  "Returns string type for a service object of type '<imuCalibration-request>"
  "comm_srvs/imuCalibrationRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'imuCalibration-request)))
  "Returns string type for a service object of type 'imuCalibration-request"
  "comm_srvs/imuCalibrationRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<imuCalibration-request>)))
  "Returns md5sum for a message object of type '<imuCalibration-request>"
  "bf5a50f919bcad97898b19cc4dfd4322")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'imuCalibration-request)))
  "Returns md5sum for a message object of type 'imuCalibration-request"
  "bf5a50f919bcad97898b19cc4dfd4322")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<imuCalibration-request>)))
  "Returns full string definition for message of type '<imuCalibration-request>"
  (cl:format cl:nil "sensorInstallParam imuClbtParam~%~%================================================================================~%MSG: comm_srvs/sensorInstallParam~%string  name ~%float32 x~%float32 y~%float32 z~%float32 roll~%float32 pitch~%float32 yaw~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'imuCalibration-request)))
  "Returns full string definition for message of type 'imuCalibration-request"
  (cl:format cl:nil "sensorInstallParam imuClbtParam~%~%================================================================================~%MSG: comm_srvs/sensorInstallParam~%string  name ~%float32 x~%float32 y~%float32 z~%float32 roll~%float32 pitch~%float32 yaw~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <imuCalibration-request>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'imuClbtParam))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <imuCalibration-request>))
  "Converts a ROS message object to a list"
  (cl:list 'imuCalibration-request
    (cl:cons ':imuClbtParam (imuClbtParam msg))
))
;//! \htmlinclude imuCalibration-response.msg.html

(cl:defclass <imuCalibration-response> (roslisp-msg-protocol:ros-message)
  ((success
    :reader success
    :initarg :success
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass imuCalibration-response (<imuCalibration-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <imuCalibration-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'imuCalibration-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_srvs-srv:<imuCalibration-response> is deprecated: use comm_srvs-srv:imuCalibration-response instead.")))

(cl:ensure-generic-function 'success-val :lambda-list '(m))
(cl:defmethod success-val ((m <imuCalibration-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:success-val is deprecated.  Use comm_srvs-srv:success instead.")
  (success m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <imuCalibration-response>) ostream)
  "Serializes a message object of type '<imuCalibration-response>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'success) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <imuCalibration-response>) istream)
  "Deserializes a message object of type '<imuCalibration-response>"
    (cl:setf (cl:slot-value msg 'success) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<imuCalibration-response>)))
  "Returns string type for a service object of type '<imuCalibration-response>"
  "comm_srvs/imuCalibrationResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'imuCalibration-response)))
  "Returns string type for a service object of type 'imuCalibration-response"
  "comm_srvs/imuCalibrationResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<imuCalibration-response>)))
  "Returns md5sum for a message object of type '<imuCalibration-response>"
  "bf5a50f919bcad97898b19cc4dfd4322")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'imuCalibration-response)))
  "Returns md5sum for a message object of type 'imuCalibration-response"
  "bf5a50f919bcad97898b19cc4dfd4322")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<imuCalibration-response>)))
  "Returns full string definition for message of type '<imuCalibration-response>"
  (cl:format cl:nil "bool        success~%~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'imuCalibration-response)))
  "Returns full string definition for message of type 'imuCalibration-response"
  (cl:format cl:nil "bool        success~%~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <imuCalibration-response>))
  (cl:+ 0
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <imuCalibration-response>))
  "Converts a ROS message object to a list"
  (cl:list 'imuCalibration-response
    (cl:cons ':success (success msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'imuCalibration)))
  'imuCalibration-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'imuCalibration)))
  'imuCalibration-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'imuCalibration)))
  "Returns string type for a service object of type '<imuCalibration>"
  "comm_srvs/imuCalibration")