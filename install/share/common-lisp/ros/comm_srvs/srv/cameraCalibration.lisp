; Auto-generated. Do not edit!


(cl:in-package comm_srvs-srv)


;//! \htmlinclude cameraCalibration-request.msg.html

(cl:defclass <cameraCalibration-request> (roslisp-msg-protocol:ros-message)
  ((camera_name
    :reader camera_name
    :initarg :camera_name
    :type cl:string
    :initform "")
   (camera_matrix
    :reader camera_matrix
    :initarg :camera_matrix
    :type cl:string
    :initform "")
   (distortion_coefficients
    :reader distortion_coefficients
    :initarg :distortion_coefficients
    :type cl:string
    :initform "")
   (tx
    :reader tx
    :initarg :tx
    :type cl:float
    :initform 0.0)
   (ty
    :reader ty
    :initarg :ty
    :type cl:float
    :initform 0.0)
   (tz
    :reader tz
    :initarg :tz
    :type cl:float
    :initform 0.0)
   (roll
    :reader roll
    :initarg :roll
    :type cl:float
    :initform 0.0)
   (pitch
    :reader pitch
    :initarg :pitch
    :type cl:float
    :initform 0.0)
   (yaw
    :reader yaw
    :initarg :yaw
    :type cl:float
    :initform 0.0))
)

(cl:defclass cameraCalibration-request (<cameraCalibration-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <cameraCalibration-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'cameraCalibration-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_srvs-srv:<cameraCalibration-request> is deprecated: use comm_srvs-srv:cameraCalibration-request instead.")))

(cl:ensure-generic-function 'camera_name-val :lambda-list '(m))
(cl:defmethod camera_name-val ((m <cameraCalibration-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:camera_name-val is deprecated.  Use comm_srvs-srv:camera_name instead.")
  (camera_name m))

(cl:ensure-generic-function 'camera_matrix-val :lambda-list '(m))
(cl:defmethod camera_matrix-val ((m <cameraCalibration-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:camera_matrix-val is deprecated.  Use comm_srvs-srv:camera_matrix instead.")
  (camera_matrix m))

(cl:ensure-generic-function 'distortion_coefficients-val :lambda-list '(m))
(cl:defmethod distortion_coefficients-val ((m <cameraCalibration-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:distortion_coefficients-val is deprecated.  Use comm_srvs-srv:distortion_coefficients instead.")
  (distortion_coefficients m))

(cl:ensure-generic-function 'tx-val :lambda-list '(m))
(cl:defmethod tx-val ((m <cameraCalibration-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:tx-val is deprecated.  Use comm_srvs-srv:tx instead.")
  (tx m))

(cl:ensure-generic-function 'ty-val :lambda-list '(m))
(cl:defmethod ty-val ((m <cameraCalibration-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:ty-val is deprecated.  Use comm_srvs-srv:ty instead.")
  (ty m))

(cl:ensure-generic-function 'tz-val :lambda-list '(m))
(cl:defmethod tz-val ((m <cameraCalibration-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:tz-val is deprecated.  Use comm_srvs-srv:tz instead.")
  (tz m))

(cl:ensure-generic-function 'roll-val :lambda-list '(m))
(cl:defmethod roll-val ((m <cameraCalibration-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:roll-val is deprecated.  Use comm_srvs-srv:roll instead.")
  (roll m))

(cl:ensure-generic-function 'pitch-val :lambda-list '(m))
(cl:defmethod pitch-val ((m <cameraCalibration-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:pitch-val is deprecated.  Use comm_srvs-srv:pitch instead.")
  (pitch m))

(cl:ensure-generic-function 'yaw-val :lambda-list '(m))
(cl:defmethod yaw-val ((m <cameraCalibration-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:yaw-val is deprecated.  Use comm_srvs-srv:yaw instead.")
  (yaw m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <cameraCalibration-request>) ostream)
  "Serializes a message object of type '<cameraCalibration-request>"
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'camera_name))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'camera_name))
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'camera_matrix))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'camera_matrix))
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'distortion_coefficients))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'distortion_coefficients))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'tx))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'ty))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'tz))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'roll))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'pitch))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'yaw))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <cameraCalibration-request>) istream)
  "Deserializes a message object of type '<cameraCalibration-request>"
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'camera_name) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'camera_name) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'camera_matrix) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'camera_matrix) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'distortion_coefficients) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'distortion_coefficients) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'tx) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'ty) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'tz) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'roll) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'pitch) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'yaw) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<cameraCalibration-request>)))
  "Returns string type for a service object of type '<cameraCalibration-request>"
  "comm_srvs/cameraCalibrationRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'cameraCalibration-request)))
  "Returns string type for a service object of type 'cameraCalibration-request"
  "comm_srvs/cameraCalibrationRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<cameraCalibration-request>)))
  "Returns md5sum for a message object of type '<cameraCalibration-request>"
  "79a62af09061e3caf062e9248160d26b")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'cameraCalibration-request)))
  "Returns md5sum for a message object of type 'cameraCalibration-request"
  "79a62af09061e3caf062e9248160d26b")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<cameraCalibration-request>)))
  "Returns full string definition for message of type '<cameraCalibration-request>"
  (cl:format cl:nil "string  camera_name             #相机名称~%string  camera_matrix           #相机内参~%string  distortion_coefficients #畸变系数~%float32 tx~%float32 ty~%float32 tz~%float32 roll~%float32 pitch~%float32 yaw~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'cameraCalibration-request)))
  "Returns full string definition for message of type 'cameraCalibration-request"
  (cl:format cl:nil "string  camera_name             #相机名称~%string  camera_matrix           #相机内参~%string  distortion_coefficients #畸变系数~%float32 tx~%float32 ty~%float32 tz~%float32 roll~%float32 pitch~%float32 yaw~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <cameraCalibration-request>))
  (cl:+ 0
     4 (cl:length (cl:slot-value msg 'camera_name))
     4 (cl:length (cl:slot-value msg 'camera_matrix))
     4 (cl:length (cl:slot-value msg 'distortion_coefficients))
     4
     4
     4
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <cameraCalibration-request>))
  "Converts a ROS message object to a list"
  (cl:list 'cameraCalibration-request
    (cl:cons ':camera_name (camera_name msg))
    (cl:cons ':camera_matrix (camera_matrix msg))
    (cl:cons ':distortion_coefficients (distortion_coefficients msg))
    (cl:cons ':tx (tx msg))
    (cl:cons ':ty (ty msg))
    (cl:cons ':tz (tz msg))
    (cl:cons ':roll (roll msg))
    (cl:cons ':pitch (pitch msg))
    (cl:cons ':yaw (yaw msg))
))
;//! \htmlinclude cameraCalibration-response.msg.html

(cl:defclass <cameraCalibration-response> (roslisp-msg-protocol:ros-message)
  ((success
    :reader success
    :initarg :success
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass cameraCalibration-response (<cameraCalibration-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <cameraCalibration-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'cameraCalibration-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_srvs-srv:<cameraCalibration-response> is deprecated: use comm_srvs-srv:cameraCalibration-response instead.")))

(cl:ensure-generic-function 'success-val :lambda-list '(m))
(cl:defmethod success-val ((m <cameraCalibration-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-srv:success-val is deprecated.  Use comm_srvs-srv:success instead.")
  (success m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <cameraCalibration-response>) ostream)
  "Serializes a message object of type '<cameraCalibration-response>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'success) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <cameraCalibration-response>) istream)
  "Deserializes a message object of type '<cameraCalibration-response>"
    (cl:setf (cl:slot-value msg 'success) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<cameraCalibration-response>)))
  "Returns string type for a service object of type '<cameraCalibration-response>"
  "comm_srvs/cameraCalibrationResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'cameraCalibration-response)))
  "Returns string type for a service object of type 'cameraCalibration-response"
  "comm_srvs/cameraCalibrationResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<cameraCalibration-response>)))
  "Returns md5sum for a message object of type '<cameraCalibration-response>"
  "79a62af09061e3caf062e9248160d26b")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'cameraCalibration-response)))
  "Returns md5sum for a message object of type 'cameraCalibration-response"
  "79a62af09061e3caf062e9248160d26b")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<cameraCalibration-response>)))
  "Returns full string definition for message of type '<cameraCalibration-response>"
  (cl:format cl:nil "bool    success~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'cameraCalibration-response)))
  "Returns full string definition for message of type 'cameraCalibration-response"
  (cl:format cl:nil "bool    success~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <cameraCalibration-response>))
  (cl:+ 0
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <cameraCalibration-response>))
  "Converts a ROS message object to a list"
  (cl:list 'cameraCalibration-response
    (cl:cons ':success (success msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'cameraCalibration)))
  'cameraCalibration-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'cameraCalibration)))
  'cameraCalibration-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'cameraCalibration)))
  "Returns string type for a service object of type '<cameraCalibration>"
  "comm_srvs/cameraCalibration")