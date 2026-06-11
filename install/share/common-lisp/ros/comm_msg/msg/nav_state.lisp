; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude nav_state.msg.html

(cl:defclass <nav_state> (roslisp-msg-protocol:ros-message)
  ((planMoveTrend
    :reader planMoveTrend
    :initarg :planMoveTrend
    :type cl:fixnum
    :initform 0)
   (navCtrl
    :reader navCtrl
    :initarg :navCtrl
    :type (cl:vector comm_msg-msg:nav_ctrl)
   :initform (cl:make-array 0 :element-type 'comm_msg-msg:nav_ctrl :initial-element (cl:make-instance 'comm_msg-msg:nav_ctrl))))
)

(cl:defclass nav_state (<nav_state>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <nav_state>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'nav_state)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<nav_state> is deprecated: use comm_msg-msg:nav_state instead.")))

(cl:ensure-generic-function 'planMoveTrend-val :lambda-list '(m))
(cl:defmethod planMoveTrend-val ((m <nav_state>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:planMoveTrend-val is deprecated.  Use comm_msg-msg:planMoveTrend instead.")
  (planMoveTrend m))

(cl:ensure-generic-function 'navCtrl-val :lambda-list '(m))
(cl:defmethod navCtrl-val ((m <nav_state>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:navCtrl-val is deprecated.  Use comm_msg-msg:navCtrl instead.")
  (navCtrl m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <nav_state>) ostream)
  "Serializes a message object of type '<nav_state>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'planMoveTrend)) ostream)
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'navCtrl))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'navCtrl))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <nav_state>) istream)
  "Deserializes a message object of type '<nav_state>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'planMoveTrend)) (cl:read-byte istream))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'navCtrl) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'navCtrl)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'comm_msg-msg:nav_ctrl))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<nav_state>)))
  "Returns string type for a message object of type '<nav_state>"
  "comm_msg/nav_state")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'nav_state)))
  "Returns string type for a message object of type 'nav_state"
  "comm_msg/nav_state")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<nav_state>)))
  "Returns md5sum for a message object of type '<nav_state>"
  "2b0151b05a659a933eedcf8f25c3b286")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'nav_state)))
  "Returns md5sum for a message object of type 'nav_state"
  "2b0151b05a659a933eedcf8f25c3b286")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<nav_state>)))
  "Returns full string definition for message of type '<nav_state>"
  (cl:format cl:nil "uint8       planMoveTrend        # 0:停止前进 1:向前运动 2:向后运动 3:自由运动~%nav_ctrl[]  navCtrl~%================================================================================~%MSG: comm_msg/nav_ctrl~%uint8 GARBAGE_DETECT = 1    # 垃圾寻扫~%uint8 CONTACT_EDGE   = 2    # 触边生成障碍物~%~%uint8 value~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'nav_state)))
  "Returns full string definition for message of type 'nav_state"
  (cl:format cl:nil "uint8       planMoveTrend        # 0:停止前进 1:向前运动 2:向后运动 3:自由运动~%nav_ctrl[]  navCtrl~%================================================================================~%MSG: comm_msg/nav_ctrl~%uint8 GARBAGE_DETECT = 1    # 垃圾寻扫~%uint8 CONTACT_EDGE   = 2    # 触边生成障碍物~%~%uint8 value~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <nav_state>))
  (cl:+ 0
     1
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'navCtrl) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <nav_state>))
  "Converts a ROS message object to a list"
  (cl:list 'nav_state
    (cl:cons ':planMoveTrend (planMoveTrend msg))
    (cl:cons ':navCtrl (navCtrl msg))
))
