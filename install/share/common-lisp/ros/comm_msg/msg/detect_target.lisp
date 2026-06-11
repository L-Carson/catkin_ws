; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude detect_target.msg.html

(cl:defclass <detect_target> (roslisp-msg-protocol:ros-message)
  ((type
    :reader type
    :initarg :type
    :type cl:fixnum
    :initform 0)
   (vertexes
    :reader vertexes
    :initarg :vertexes
    :type (cl:vector comm_msg-msg:coor2D)
   :initform (cl:make-array 0 :element-type 'comm_msg-msg:coor2D :initial-element (cl:make-instance 'comm_msg-msg:coor2D))))
)

(cl:defclass detect_target (<detect_target>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <detect_target>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'detect_target)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<detect_target> is deprecated: use comm_msg-msg:detect_target instead.")))

(cl:ensure-generic-function 'type-val :lambda-list '(m))
(cl:defmethod type-val ((m <detect_target>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:type-val is deprecated.  Use comm_msg-msg:type instead.")
  (type m))

(cl:ensure-generic-function 'vertexes-val :lambda-list '(m))
(cl:defmethod vertexes-val ((m <detect_target>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:vertexes-val is deprecated.  Use comm_msg-msg:vertexes instead.")
  (vertexes m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <detect_target>) ostream)
  "Serializes a message object of type '<detect_target>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'type)) ostream)
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'vertexes))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'vertexes))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <detect_target>) istream)
  "Deserializes a message object of type '<detect_target>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'type)) (cl:read-byte istream))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'vertexes) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'vertexes)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'comm_msg-msg:coor2D))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<detect_target>)))
  "Returns string type for a message object of type '<detect_target>"
  "comm_msg/detect_target")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'detect_target)))
  "Returns string type for a message object of type 'detect_target"
  "comm_msg/detect_target")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<detect_target>)))
  "Returns md5sum for a message object of type '<detect_target>"
  "4e9958080bbdd6468da1c554f1002943")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'detect_target)))
  "Returns md5sum for a message object of type 'detect_target"
  "4e9958080bbdd6468da1c554f1002943")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<detect_target>)))
  "Returns full string definition for message of type '<detect_target>"
  (cl:format cl:nil "# 1开门 2关门~%uint8    type~%coor2D[] vertexes~%================================================================================~%MSG: comm_msg/coor2D~%coorLabel   label       #坐标类别~%float32     pos_x~%float32     pos_y~%================================================================================~%MSG: comm_msg/coorLabel~%uint8   UNKNOWN      = 0  #未知~%uint8   STATIC       = 1  #静态~%uint8   DYNAMIC      = 2  #动态~%uint8   HOLLOW_OBST  = 3  #空洞障碍~%uint8   SHORT_HOLD   = 4  #低矮维持~%uint8   COMMON_HOLD  = 5  #常规维持~%uint8   BLIND_SPOT   = 6  #盲区障碍~%uint8   SLOPE_PAD    = 7  #斜坡垫~%uint8   CONTACT_EDGE = 8  #触边障碍~%uint8   TREELAWN     = 9  #绿植障碍~%uint8   ULTRASONIC   = 10 #超声~%~%uint8 value~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'detect_target)))
  "Returns full string definition for message of type 'detect_target"
  (cl:format cl:nil "# 1开门 2关门~%uint8    type~%coor2D[] vertexes~%================================================================================~%MSG: comm_msg/coor2D~%coorLabel   label       #坐标类别~%float32     pos_x~%float32     pos_y~%================================================================================~%MSG: comm_msg/coorLabel~%uint8   UNKNOWN      = 0  #未知~%uint8   STATIC       = 1  #静态~%uint8   DYNAMIC      = 2  #动态~%uint8   HOLLOW_OBST  = 3  #空洞障碍~%uint8   SHORT_HOLD   = 4  #低矮维持~%uint8   COMMON_HOLD  = 5  #常规维持~%uint8   BLIND_SPOT   = 6  #盲区障碍~%uint8   SLOPE_PAD    = 7  #斜坡垫~%uint8   CONTACT_EDGE = 8  #触边障碍~%uint8   TREELAWN     = 9  #绿植障碍~%uint8   ULTRASONIC   = 10 #超声~%~%uint8 value~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <detect_target>))
  (cl:+ 0
     1
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'vertexes) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <detect_target>))
  "Converts a ROS message object to a list"
  (cl:list 'detect_target
    (cl:cons ':type (type msg))
    (cl:cons ':vertexes (vertexes msg))
))
