; Auto-generated. Do not edit!


(cl:in-package comm_srvs-srv)


;//! \htmlinclude lidarOffsetCalibration-request.msg.html

(cl:defclass <lidarOffsetCalibration-request> (roslisp-msg-protocol:ros-message)
  ((lidarOffsetParam
    :reader lidarOffsetParam
    :initarg :lidarOffsetParam
    :type comm_srvs-msg:sensorInstallParam
    :initform (cl:make-instance 'comm_srvs-msg:sensorInstallParam)))
)

(cl:defclass lidarOffsetCalibration-request (<lidarOffsetCalibration-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <lidarOffsetCalibration-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'lidarOffsetCalibration-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_srvs-srv:<lidarOffsetCalibration-request> is deprecated: use comm_srvs-srv:lidarOffsetCalibration-request instead.")))

(cl:ensure-generic-function 'lidarOffsetParam-val :lambda-list '(m))
(cl:defmethod lidarOffsetParam-val ((m <lidarOffsetCalibration-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:lidarOffsetParam-val is deprecated.  Use comm_srvs-srv:lidarOffsetParam instead.")
  (lidarOffsetParam m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <lidarOffsetCalibration-request>) ostream)
  "Serializes a message object of type '<lidarOffsetCalibration-request>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'lidarOffsetParam) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <lidarOffsetCalibration-request>) istream)
  "Deserializes a message object of type '<lidarOffsetCalibration-request>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'lidarOffsetParam) istream)
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<lidarOffsetCalibration-request>)))
  "Returns string type for a service object of type '<lidarOffsetCalibration-request>"
  "comm_srvs/lidarOffsetCalibrationRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'lidarOffsetCalibration-request)))
  "Returns string type for a service object of type 'lidarOffsetCalibration-request"
  "comm_srvs/lidarOffsetCalibrationRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<lidarOffsetCalibration-request>)))
  "Returns md5sum for a message object of type '<lidarOffsetCalibration-request>"
  "c4589f9ccc2d3ea9fba7e7bffbeb08d8")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'lidarOffsetCalibration-request)))
  "Returns md5sum for a message object of type 'lidarOffsetCalibration-request"
  "c4589f9ccc2d3ea9fba7e7bffbeb08d8")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<lidarOffsetCalibration-request>)))
  "Returns full string definition for message of type '<lidarOffsetCalibration-request>"
  (cl:format cl:nil "sensorInstallParam lidarOffsetParam~%~%================================================================================~%MSG: comm_srvs/sensorInstallParam~%string  name ~%float32 x~%float32 y~%float32 z~%float32 roll~%float32 pitch~%float32 yaw~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'lidarOffsetCalibration-request)))
  "Returns full string definition for message of type 'lidarOffsetCalibration-request"
  (cl:format cl:nil "sensorInstallParam lidarOffsetParam~%~%================================================================================~%MSG: comm_srvs/sensorInstallParam~%string  name ~%float32 x~%float32 y~%float32 z~%float32 roll~%float32 pitch~%float32 yaw~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <lidarOffsetCalibration-request>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'lidarOffsetParam))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <lidarOffsetCalibration-request>))
  "Converts a ROS message object to a list"
  (cl:list 'lidarOffsetCalibration-request
    (cl:cons ':lidarOffsetParam (lidarOffsetParam msg))
))
;//! \htmlinclude lidarOffsetCalibration-response.msg.html

(cl:defclass <lidarOffsetCalibration-response> (roslisp-msg-protocol:ros-message)
  ((success
    :reader success
    :initarg :success
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass lidarOffsetCalibration-response (<lidarOffsetCalibration-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <lidarOffsetCalibration-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'lidarOffsetCalibration-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_srvs-srv:<lidarOffsetCalibration-response> is deprecated: use comm_srvs-srv:lidarOffsetCalibration-response instead.")))

(cl:ensure-generic-function 'success-val :lambda-list '(m))
(cl:defmethod success-val ((m <lidarOffsetCalibration-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:success-val is deprecated.  Use comm_srvs-srv:success instead.")
  (success m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <lidarOffsetCalibration-response>) ostream)
  "Serializes a message object of type '<lidarOffsetCalibration-response>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'success) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <lidarOffsetCalibration-response>) istream)
  "Deserializes a message object of type '<lidarOffsetCalibration-response>"
    (cl:setf (cl:slot-value msg 'success) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<lidarOffsetCalibration-response>)))
  "Returns string type for a service object of type '<lidarOffsetCalibration-response>"
  "comm_srvs/lidarOffsetCalibrationResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'lidarOffsetCalibration-response)))
  "Returns string type for a service object of type 'lidarOffsetCalibration-response"
  "comm_srvs/lidarOffsetCalibrationResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<lidarOffsetCalibration-response>)))
  "Returns md5sum for a message object of type '<lidarOffsetCalibration-response>"
  "c4589f9ccc2d3ea9fba7e7bffbeb08d8")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'lidarOffsetCalibration-response)))
  "Returns md5sum for a message object of type 'lidarOffsetCalibration-response"
  "c4589f9ccc2d3ea9fba7e7bffbeb08d8")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<lidarOffsetCalibration-response>)))
  "Returns full string definition for message of type '<lidarOffsetCalibration-response>"
  (cl:format cl:nil "bool        success~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'lidarOffsetCalibration-response)))
  "Returns full string definition for message of type 'lidarOffsetCalibration-response"
  (cl:format cl:nil "bool        success~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <lidarOffsetCalibration-response>))
  (cl:+ 0
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <lidarOffsetCalibration-response>))
  "Converts a ROS message object to a list"
  (cl:list 'lidarOffsetCalibration-response
    (cl:cons ':success (success msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'lidarOffsetCalibration)))
  'lidarOffsetCalibration-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'lidarOffsetCalibration)))
  'lidarOffsetCalibration-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'lidarOffsetCalibration)))
  "Returns string type for a service object of type '<lidarOffsetCalibration>"
  "comm_srvs/lidarOffsetCalibration")