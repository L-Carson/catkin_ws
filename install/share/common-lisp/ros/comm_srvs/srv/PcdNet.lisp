; Auto-generated. Do not edit!


(cl:in-package comm_srvs-srv)


;//! \htmlinclude PcdNet-request.msg.html

(cl:defclass <PcdNet-request> (roslisp-msg-protocol:ros-message)
  ()
)

(cl:defclass PcdNet-request (<PcdNet-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <PcdNet-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'PcdNet-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_srvs-srv:<PcdNet-request> is deprecated: use comm_srvs-srv:PcdNet-request instead.")))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <PcdNet-request>) ostream)
  "Serializes a message object of type '<PcdNet-request>"
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <PcdNet-request>) istream)
  "Deserializes a message object of type '<PcdNet-request>"
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<PcdNet-request>)))
  "Returns string type for a service object of type '<PcdNet-request>"
  "comm_srvs/PcdNetRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'PcdNet-request)))
  "Returns string type for a service object of type 'PcdNet-request"
  "comm_srvs/PcdNetRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<PcdNet-request>)))
  "Returns md5sum for a message object of type '<PcdNet-request>"
  "d49762f82989f849a33296dad5e777da")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'PcdNet-request)))
  "Returns md5sum for a message object of type 'PcdNet-request"
  "d49762f82989f849a33296dad5e777da")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<PcdNet-request>)))
  "Returns full string definition for message of type '<PcdNet-request>"
  (cl:format cl:nil "~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'PcdNet-request)))
  "Returns full string definition for message of type 'PcdNet-request"
  (cl:format cl:nil "~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <PcdNet-request>))
  (cl:+ 0
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <PcdNet-request>))
  "Converts a ROS message object to a list"
  (cl:list 'PcdNet-request
))
;//! \htmlinclude PcdNet-response.msg.html

(cl:defclass <PcdNet-response> (roslisp-msg-protocol:ros-message)
  ((success
    :reader success
    :initarg :success
    :type cl:boolean
    :initform cl:nil)
   (isOpen
    :reader isOpen
    :initarg :isOpen
    :type cl:boolean
    :initform cl:nil)
   (lidarInstallParamArray
    :reader lidarInstallParamArray
    :initarg :lidarInstallParamArray
    :type (cl:vector comm_srvs-msg:LidarInstallParam)
   :initform (cl:make-array 0 :element-type 'comm_srvs-msg:LidarInstallParam :initial-element (cl:make-instance 'comm_srvs-msg:LidarInstallParam))))
)

(cl:defclass PcdNet-response (<PcdNet-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <PcdNet-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'PcdNet-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_srvs-srv:<PcdNet-response> is deprecated: use comm_srvs-srv:PcdNet-response instead.")))

(cl:ensure-generic-function 'success-val :lambda-list '(m))
(cl:defmethod success-val ((m <PcdNet-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:success-val is deprecated.  Use comm_srvs-srv:success instead.")
  (success m))

(cl:ensure-generic-function 'isOpen-val :lambda-list '(m))
(cl:defmethod isOpen-val ((m <PcdNet-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:isOpen-val is deprecated.  Use comm_srvs-srv:isOpen instead.")
  (isOpen m))

(cl:ensure-generic-function 'lidarInstallParamArray-val :lambda-list '(m))
(cl:defmethod lidarInstallParamArray-val ((m <PcdNet-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:lidarInstallParamArray-val is deprecated.  Use comm_srvs-srv:lidarInstallParamArray instead.")
  (lidarInstallParamArray m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <PcdNet-response>) ostream)
  "Serializes a message object of type '<PcdNet-response>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'success) 1 0)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'isOpen) 1 0)) ostream)
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'lidarInstallParamArray))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'lidarInstallParamArray))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <PcdNet-response>) istream)
  "Deserializes a message object of type '<PcdNet-response>"
    (cl:setf (cl:slot-value msg 'success) (cl:not (cl:zerop (cl:read-byte istream))))
    (cl:setf (cl:slot-value msg 'isOpen) (cl:not (cl:zerop (cl:read-byte istream))))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'lidarInstallParamArray) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'lidarInstallParamArray)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'comm_srvs-msg:LidarInstallParam))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<PcdNet-response>)))
  "Returns string type for a service object of type '<PcdNet-response>"
  "comm_srvs/PcdNetResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'PcdNet-response)))
  "Returns string type for a service object of type 'PcdNet-response"
  "comm_srvs/PcdNetResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<PcdNet-response>)))
  "Returns md5sum for a message object of type '<PcdNet-response>"
  "d49762f82989f849a33296dad5e777da")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'PcdNet-response)))
  "Returns md5sum for a message object of type 'PcdNet-response"
  "d49762f82989f849a33296dad5e777da")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<PcdNet-response>)))
  "Returns full string definition for message of type '<PcdNet-response>"
  (cl:format cl:nil "bool    success~%bool    isOpen~%LidarInstallParam[] lidarInstallParamArray~%~%================================================================================~%MSG: comm_srvs/LidarInstallParam~%string  name    #MainLidar BlindLidar BlindLidar2~%string  serialNum~%float32 x~%float32 y~%float32 z~%float32 roll~%float32 pitch~%float32 yaw~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'PcdNet-response)))
  "Returns full string definition for message of type 'PcdNet-response"
  (cl:format cl:nil "bool    success~%bool    isOpen~%LidarInstallParam[] lidarInstallParamArray~%~%================================================================================~%MSG: comm_srvs/LidarInstallParam~%string  name    #MainLidar BlindLidar BlindLidar2~%string  serialNum~%float32 x~%float32 y~%float32 z~%float32 roll~%float32 pitch~%float32 yaw~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <PcdNet-response>))
  (cl:+ 0
     1
     1
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'lidarInstallParamArray) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <PcdNet-response>))
  "Converts a ROS message object to a list"
  (cl:list 'PcdNet-response
    (cl:cons ':success (success msg))
    (cl:cons ':isOpen (isOpen msg))
    (cl:cons ':lidarInstallParamArray (lidarInstallParamArray msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'PcdNet)))
  'PcdNet-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'PcdNet)))
  'PcdNet-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'PcdNet)))
  "Returns string type for a service object of type '<PcdNet>"
  "comm_srvs/PcdNet")