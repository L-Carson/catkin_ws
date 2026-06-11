; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude marker_visible.msg.html

(cl:defclass <marker_visible> (roslisp-msg-protocol:ros-message)
  ((point_P
    :reader point_P
    :initarg :point_P
    :type (cl:vector geometry_msgs-msg:Point)
   :initform (cl:make-array 0 :element-type 'geometry_msgs-msg:Point :initial-element (cl:make-instance 'geometry_msgs-msg:Point)))
   (point_V
    :reader point_V
    :initarg :point_V
    :type (cl:vector geometry_msgs-msg:Point)
   :initform (cl:make-array 0 :element-type 'geometry_msgs-msg:Point :initial-element (cl:make-instance 'geometry_msgs-msg:Point)))
   (velocity
    :reader velocity
    :initarg :velocity
    :type (cl:vector cl:float)
   :initform (cl:make-array 0 :element-type 'cl:float :initial-element 0.0)))
)

(cl:defclass marker_visible (<marker_visible>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <marker_visible>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'marker_visible)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<marker_visible> is deprecated: use comm_msg-msg:marker_visible instead.")))

(cl:ensure-generic-function 'point_P-val :lambda-list '(m))
(cl:defmethod point_P-val ((m <marker_visible>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:point_P-val is deprecated.  Use comm_msg-msg:point_P instead.")
  (point_P m))

(cl:ensure-generic-function 'point_V-val :lambda-list '(m))
(cl:defmethod point_V-val ((m <marker_visible>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:point_V-val is deprecated.  Use comm_msg-msg:point_V instead.")
  (point_V m))

(cl:ensure-generic-function 'velocity-val :lambda-list '(m))
(cl:defmethod velocity-val ((m <marker_visible>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:velocity-val is deprecated.  Use comm_msg-msg:velocity instead.")
  (velocity m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <marker_visible>) ostream)
  "Serializes a message object of type '<marker_visible>"
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'point_P))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'point_P))
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'point_V))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'point_V))
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'velocity))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let ((bits (roslisp-utils:encode-single-float-bits ele)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)))
   (cl:slot-value msg 'velocity))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <marker_visible>) istream)
  "Deserializes a message object of type '<marker_visible>"
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'point_P) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'point_P)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'geometry_msgs-msg:Point))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'point_V) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'point_V)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'geometry_msgs-msg:Point))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'velocity) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'velocity)))
    (cl:dotimes (i __ros_arr_len)
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:aref vals i) (roslisp-utils:decode-single-float-bits bits))))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<marker_visible>)))
  "Returns string type for a message object of type '<marker_visible>"
  "comm_msg/marker_visible")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'marker_visible)))
  "Returns string type for a message object of type 'marker_visible"
  "comm_msg/marker_visible")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<marker_visible>)))
  "Returns md5sum for a message object of type '<marker_visible>"
  "8c091b688cfba7fce69c6312f128ed30")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'marker_visible)))
  "Returns md5sum for a message object of type 'marker_visible"
  "8c091b688cfba7fce69c6312f128ed30")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<marker_visible>)))
  "Returns full string definition for message of type '<marker_visible>"
  (cl:format cl:nil "geometry_msgs/Point[] 	point_P~%geometry_msgs/Point[] 	point_V~%~%float32[]               velocity~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'marker_visible)))
  "Returns full string definition for message of type 'marker_visible"
  (cl:format cl:nil "geometry_msgs/Point[] 	point_P~%geometry_msgs/Point[] 	point_V~%~%float32[]               velocity~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <marker_visible>))
  (cl:+ 0
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'point_P) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'point_V) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'velocity) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4)))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <marker_visible>))
  "Converts a ROS message object to a list"
  (cl:list 'marker_visible
    (cl:cons ':point_P (point_P msg))
    (cl:cons ':point_V (point_V msg))
    (cl:cons ':velocity (velocity msg))
))
