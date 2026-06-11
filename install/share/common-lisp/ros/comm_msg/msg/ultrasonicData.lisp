; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude ultrasonicData.msg.html

(cl:defclass <ultrasonicData> (roslisp-msg-protocol:ros-message)
  ((usName
    :reader usName
    :initarg :usName
    :type cl:string
    :initform "")
   (data
    :reader data
    :initarg :data
    :type (cl:vector comm_msg-msg:coor2D)
   :initform (cl:make-array 0 :element-type 'comm_msg-msg:coor2D :initial-element (cl:make-instance 'comm_msg-msg:coor2D))))
)

(cl:defclass ultrasonicData (<ultrasonicData>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <ultrasonicData>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'ultrasonicData)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<ultrasonicData> is deprecated: use comm_msg-msg:ultrasonicData instead.")))

(cl:ensure-generic-function 'usName-val :lambda-list '(m))
(cl:defmethod usName-val ((m <ultrasonicData>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:usName-val is deprecated.  Use comm_msg-msg:usName instead.")
  (usName m))

(cl:ensure-generic-function 'data-val :lambda-list '(m))
(cl:defmethod data-val ((m <ultrasonicData>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:data-val is deprecated.  Use comm_msg-msg:data instead.")
  (data m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <ultrasonicData>) ostream)
  "Serializes a message object of type '<ultrasonicData>"
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'usName))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'usName))
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'data))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'data))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <ultrasonicData>) istream)
  "Deserializes a message object of type '<ultrasonicData>"
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'usName) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'usName) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'data) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'data)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'comm_msg-msg:coor2D))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<ultrasonicData>)))
  "Returns string type for a message object of type '<ultrasonicData>"
  "comm_msg/ultrasonicData")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'ultrasonicData)))
  "Returns string type for a message object of type 'ultrasonicData"
  "comm_msg/ultrasonicData")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<ultrasonicData>)))
  "Returns md5sum for a message object of type '<ultrasonicData>"
  "16d3d45049c1ab8e0a2113ea61e36760")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'ultrasonicData)))
  "Returns md5sum for a message object of type 'ultrasonicData"
  "16d3d45049c1ab8e0a2113ea61e36760")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<ultrasonicData>)))
  "Returns full string definition for message of type '<ultrasonicData>"
  (cl:format cl:nil "string   usName~%coor2D[] data~%================================================================================~%MSG: comm_msg/coor2D~%coorLabel   label       #坐标类别~%float32     pos_x~%float32     pos_y~%================================================================================~%MSG: comm_msg/coorLabel~%uint8   UNKNOWN      = 0  #未知~%uint8   STATIC       = 1  #静态~%uint8   DYNAMIC      = 2  #动态~%uint8   HOLLOW_OBST  = 3  #空洞障碍~%uint8   SHORT_HOLD   = 4  #低矮维持~%uint8   COMMON_HOLD  = 5  #常规维持~%uint8   BLIND_SPOT   = 6  #盲区障碍~%uint8   SLOPE_PAD    = 7  #斜坡垫~%uint8   CONTACT_EDGE = 8  #触边障碍~%uint8   TREELAWN     = 9  #绿植障碍~%uint8   ULTRASONIC   = 10 #超声~%~%uint8 value~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'ultrasonicData)))
  "Returns full string definition for message of type 'ultrasonicData"
  (cl:format cl:nil "string   usName~%coor2D[] data~%================================================================================~%MSG: comm_msg/coor2D~%coorLabel   label       #坐标类别~%float32     pos_x~%float32     pos_y~%================================================================================~%MSG: comm_msg/coorLabel~%uint8   UNKNOWN      = 0  #未知~%uint8   STATIC       = 1  #静态~%uint8   DYNAMIC      = 2  #动态~%uint8   HOLLOW_OBST  = 3  #空洞障碍~%uint8   SHORT_HOLD   = 4  #低矮维持~%uint8   COMMON_HOLD  = 5  #常规维持~%uint8   BLIND_SPOT   = 6  #盲区障碍~%uint8   SLOPE_PAD    = 7  #斜坡垫~%uint8   CONTACT_EDGE = 8  #触边障碍~%uint8   TREELAWN     = 9  #绿植障碍~%uint8   ULTRASONIC   = 10 #超声~%~%uint8 value~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <ultrasonicData>))
  (cl:+ 0
     4 (cl:length (cl:slot-value msg 'usName))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'data) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <ultrasonicData>))
  "Converts a ROS message object to a list"
  (cl:list 'ultrasonicData
    (cl:cons ':usName (usName msg))
    (cl:cons ':data (data msg))
))
