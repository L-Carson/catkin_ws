; Auto-generated. Do not edit!


(cl:in-package comm_srvs-srv)


;//! \htmlinclude camerasParam-request.msg.html

(cl:defclass <camerasParam-request> (roslisp-msg-protocol:ros-message)
  ()
)

(cl:defclass camerasParam-request (<camerasParam-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <camerasParam-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'camerasParam-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_srvs-srv:<camerasParam-request> is deprecated: use comm_srvs-srv:camerasParam-request instead.")))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <camerasParam-request>) ostream)
  "Serializes a message object of type '<camerasParam-request>"
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <camerasParam-request>) istream)
  "Deserializes a message object of type '<camerasParam-request>"
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<camerasParam-request>)))
  "Returns string type for a service object of type '<camerasParam-request>"
  "comm_srvs/camerasParamRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'camerasParam-request)))
  "Returns string type for a service object of type 'camerasParam-request"
  "comm_srvs/camerasParamRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<camerasParam-request>)))
  "Returns md5sum for a message object of type '<camerasParam-request>"
  "fd18540e50d2c9de8ab7b244039cf247")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'camerasParam-request)))
  "Returns md5sum for a message object of type 'camerasParam-request"
  "fd18540e50d2c9de8ab7b244039cf247")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<camerasParam-request>)))
  "Returns full string definition for message of type '<camerasParam-request>"
  (cl:format cl:nil "~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'camerasParam-request)))
  "Returns full string definition for message of type 'camerasParam-request"
  (cl:format cl:nil "~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <camerasParam-request>))
  (cl:+ 0
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <camerasParam-request>))
  "Converts a ROS message object to a list"
  (cl:list 'camerasParam-request
))
;//! \htmlinclude camerasParam-response.msg.html

(cl:defclass <camerasParam-response> (roslisp-msg-protocol:ros-message)
  ((success
    :reader success
    :initarg :success
    :type cl:boolean
    :initform cl:nil)
   (cameraParamArray
    :reader cameraParamArray
    :initarg :cameraParamArray
    :type (cl:vector comm_srvs-msg:cameraParam)
   :initform (cl:make-array 0 :element-type 'comm_srvs-msg:cameraParam :initial-element (cl:make-instance 'comm_srvs-msg:cameraParam))))
)

(cl:defclass camerasParam-response (<camerasParam-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <camerasParam-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'camerasParam-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_srvs-srv:<camerasParam-response> is deprecated: use comm_srvs-srv:camerasParam-response instead.")))

(cl:ensure-generic-function 'success-val :lambda-list '(m))
(cl:defmethod success-val ((m <camerasParam-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:success-val is deprecated.  Use comm_srvs-srv:success instead.")
  (success m))

(cl:ensure-generic-function 'cameraParamArray-val :lambda-list '(m))
(cl:defmethod cameraParamArray-val ((m <camerasParam-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:cameraParamArray-val is deprecated.  Use comm_srvs-srv:cameraParamArray instead.")
  (cameraParamArray m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <camerasParam-response>) ostream)
  "Serializes a message object of type '<camerasParam-response>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'success) 1 0)) ostream)
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'cameraParamArray))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'cameraParamArray))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <camerasParam-response>) istream)
  "Deserializes a message object of type '<camerasParam-response>"
    (cl:setf (cl:slot-value msg 'success) (cl:not (cl:zerop (cl:read-byte istream))))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'cameraParamArray) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'cameraParamArray)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'comm_srvs-msg:cameraParam))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<camerasParam-response>)))
  "Returns string type for a service object of type '<camerasParam-response>"
  "comm_srvs/camerasParamResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'camerasParam-response)))
  "Returns string type for a service object of type 'camerasParam-response"
  "comm_srvs/camerasParamResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<camerasParam-response>)))
  "Returns md5sum for a message object of type '<camerasParam-response>"
  "fd18540e50d2c9de8ab7b244039cf247")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'camerasParam-response)))
  "Returns md5sum for a message object of type 'camerasParam-response"
  "fd18540e50d2c9de8ab7b244039cf247")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<camerasParam-response>)))
  "Returns full string definition for message of type '<camerasParam-response>"
  (cl:format cl:nil "bool          success~%cameraParam[] cameraParamArray~%~%================================================================================~%MSG: comm_srvs/cameraParam~%string      name_id~%string      port_name~%uint32      frame_hz~%~%uint32      height~%uint32      width~%~%float32     tx~%float32     ty~%float32     tz~%float32     roll~%float32     pitch~%float32     yaw~%~%float64[]   camera_matrix~%float64[]   distortion_coefficients~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'camerasParam-response)))
  "Returns full string definition for message of type 'camerasParam-response"
  (cl:format cl:nil "bool          success~%cameraParam[] cameraParamArray~%~%================================================================================~%MSG: comm_srvs/cameraParam~%string      name_id~%string      port_name~%uint32      frame_hz~%~%uint32      height~%uint32      width~%~%float32     tx~%float32     ty~%float32     tz~%float32     roll~%float32     pitch~%float32     yaw~%~%float64[]   camera_matrix~%float64[]   distortion_coefficients~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <camerasParam-response>))
  (cl:+ 0
     1
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'cameraParamArray) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <camerasParam-response>))
  "Converts a ROS message object to a list"
  (cl:list 'camerasParam-response
    (cl:cons ':success (success msg))
    (cl:cons ':cameraParamArray (cameraParamArray msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'camerasParam)))
  'camerasParam-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'camerasParam)))
  'camerasParam-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'camerasParam)))
  "Returns string type for a service object of type '<camerasParam>"
  "comm_srvs/camerasParam")