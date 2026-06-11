; Auto-generated. Do not edit!


(cl:in-package comm_srvs-msg)


;//! \htmlinclude cameraParam.msg.html

(cl:defclass <cameraParam> (roslisp-msg-protocol:ros-message)
  ((name_id
    :reader name_id
    :initarg :name_id
    :type cl:string
    :initform "")
   (port_name
    :reader port_name
    :initarg :port_name
    :type cl:string
    :initform "")
   (frame_hz
    :reader frame_hz
    :initarg :frame_hz
    :type cl:integer
    :initform 0)
   (height
    :reader height
    :initarg :height
    :type cl:integer
    :initform 0)
   (width
    :reader width
    :initarg :width
    :type cl:integer
    :initform 0)
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
    :initform 0.0)
   (camera_matrix
    :reader camera_matrix
    :initarg :camera_matrix
    :type (cl:vector cl:float)
   :initform (cl:make-array 0 :element-type 'cl:float :initial-element 0.0))
   (distortion_coefficients
    :reader distortion_coefficients
    :initarg :distortion_coefficients
    :type (cl:vector cl:float)
   :initform (cl:make-array 0 :element-type 'cl:float :initial-element 0.0)))
)

(cl:defclass cameraParam (<cameraParam>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <cameraParam>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'cameraParam)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_srvs-msg:<cameraParam> is deprecated: use comm_srvs-msg:cameraParam instead.")))

(cl:ensure-generic-function 'name_id-val :lambda-list '(m))
(cl:defmethod name_id-val ((m <cameraParam>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-msg:name_id-val is deprecated.  Use comm_srvs-msg:name_id instead.")
  (name_id m))

(cl:ensure-generic-function 'port_name-val :lambda-list '(m))
(cl:defmethod port_name-val ((m <cameraParam>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-msg:port_name-val is deprecated.  Use comm_srvs-msg:port_name instead.")
  (port_name m))

(cl:ensure-generic-function 'frame_hz-val :lambda-list '(m))
(cl:defmethod frame_hz-val ((m <cameraParam>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-msg:frame_hz-val is deprecated.  Use comm_srvs-msg:frame_hz instead.")
  (frame_hz m))

(cl:ensure-generic-function 'height-val :lambda-list '(m))
(cl:defmethod height-val ((m <cameraParam>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-msg:height-val is deprecated.  Use comm_srvs-msg:height instead.")
  (height m))

(cl:ensure-generic-function 'width-val :lambda-list '(m))
(cl:defmethod width-val ((m <cameraParam>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-msg:width-val is deprecated.  Use comm_srvs-msg:width instead.")
  (width m))

(cl:ensure-generic-function 'tx-val :lambda-list '(m))
(cl:defmethod tx-val ((m <cameraParam>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-msg:tx-val is deprecated.  Use comm_srvs-msg:tx instead.")
  (tx m))

(cl:ensure-generic-function 'ty-val :lambda-list '(m))
(cl:defmethod ty-val ((m <cameraParam>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-msg:ty-val is deprecated.  Use comm_srvs-msg:ty instead.")
  (ty m))

(cl:ensure-generic-function 'tz-val :lambda-list '(m))
(cl:defmethod tz-val ((m <cameraParam>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-msg:tz-val is deprecated.  Use comm_srvs-msg:tz instead.")
  (tz m))

(cl:ensure-generic-function 'roll-val :lambda-list '(m))
(cl:defmethod roll-val ((m <cameraParam>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-msg:roll-val is deprecated.  Use comm_srvs-msg:roll instead.")
  (roll m))

(cl:ensure-generic-function 'pitch-val :lambda-list '(m))
(cl:defmethod pitch-val ((m <cameraParam>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-msg:pitch-val is deprecated.  Use comm_srvs-msg:pitch instead.")
  (pitch m))

(cl:ensure-generic-function 'yaw-val :lambda-list '(m))
(cl:defmethod yaw-val ((m <cameraParam>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-msg:yaw-val is deprecated.  Use comm_srvs-msg:yaw instead.")
  (yaw m))

(cl:ensure-generic-function 'camera_matrix-val :lambda-list '(m))
(cl:defmethod camera_matrix-val ((m <cameraParam>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-msg:camera_matrix-val is deprecated.  Use comm_srvs-msg:camera_matrix instead.")
  (camera_matrix m))

(cl:ensure-generic-function 'distortion_coefficients-val :lambda-list '(m))
(cl:defmethod distortion_coefficients-val ((m <cameraParam>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_srvs-msg:distortion_coefficients-val is deprecated.  Use comm_srvs-msg:distortion_coefficients instead.")
  (distortion_coefficients m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <cameraParam>) ostream)
  "Serializes a message object of type '<cameraParam>"
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'name_id))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'name_id))
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'port_name))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'port_name))
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'frame_hz)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'frame_hz)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'frame_hz)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'frame_hz)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'height)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'height)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'height)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'height)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'width)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'width)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'width)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'width)) ostream)
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
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'camera_matrix))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let ((bits (roslisp-utils:encode-double-float-bits ele)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream)))
   (cl:slot-value msg 'camera_matrix))
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'distortion_coefficients))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let ((bits (roslisp-utils:encode-double-float-bits ele)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream)))
   (cl:slot-value msg 'distortion_coefficients))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <cameraParam>) istream)
  "Deserializes a message object of type '<cameraParam>"
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'name_id) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'name_id) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'port_name) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'port_name) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'frame_hz)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'frame_hz)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'frame_hz)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'frame_hz)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'height)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'height)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'height)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'height)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'width)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'width)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'width)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'width)) (cl:read-byte istream))
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
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'camera_matrix) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'camera_matrix)))
    (cl:dotimes (i __ros_arr_len)
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:aref vals i) (roslisp-utils:decode-double-float-bits bits))))))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'distortion_coefficients) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'distortion_coefficients)))
    (cl:dotimes (i __ros_arr_len)
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:aref vals i) (roslisp-utils:decode-double-float-bits bits))))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<cameraParam>)))
  "Returns string type for a message object of type '<cameraParam>"
  "comm_srvs/cameraParam")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'cameraParam)))
  "Returns string type for a message object of type 'cameraParam"
  "comm_srvs/cameraParam")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<cameraParam>)))
  "Returns md5sum for a message object of type '<cameraParam>"
  "4f9f9ccd79905f101e4555a21c54abfd")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'cameraParam)))
  "Returns md5sum for a message object of type 'cameraParam"
  "4f9f9ccd79905f101e4555a21c54abfd")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<cameraParam>)))
  "Returns full string definition for message of type '<cameraParam>"
  (cl:format cl:nil "string      name_id~%string      port_name~%uint32      frame_hz~%~%uint32      height~%uint32      width~%~%float32     tx~%float32     ty~%float32     tz~%float32     roll~%float32     pitch~%float32     yaw~%~%float64[]   camera_matrix~%float64[]   distortion_coefficients~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'cameraParam)))
  "Returns full string definition for message of type 'cameraParam"
  (cl:format cl:nil "string      name_id~%string      port_name~%uint32      frame_hz~%~%uint32      height~%uint32      width~%~%float32     tx~%float32     ty~%float32     tz~%float32     roll~%float32     pitch~%float32     yaw~%~%float64[]   camera_matrix~%float64[]   distortion_coefficients~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <cameraParam>))
  (cl:+ 0
     4 (cl:length (cl:slot-value msg 'name_id))
     4 (cl:length (cl:slot-value msg 'port_name))
     4
     4
     4
     4
     4
     4
     4
     4
     4
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'camera_matrix) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 8)))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'distortion_coefficients) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 8)))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <cameraParam>))
  "Converts a ROS message object to a list"
  (cl:list 'cameraParam
    (cl:cons ':name_id (name_id msg))
    (cl:cons ':port_name (port_name msg))
    (cl:cons ':frame_hz (frame_hz msg))
    (cl:cons ':height (height msg))
    (cl:cons ':width (width msg))
    (cl:cons ':tx (tx msg))
    (cl:cons ':ty (ty msg))
    (cl:cons ':tz (tz msg))
    (cl:cons ':roll (roll msg))
    (cl:cons ':pitch (pitch msg))
    (cl:cons ':yaw (yaw msg))
    (cl:cons ':camera_matrix (camera_matrix msg))
    (cl:cons ':distortion_coefficients (distortion_coefficients msg))
))
