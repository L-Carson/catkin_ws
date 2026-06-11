; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude simulation_obsts.msg.html

(cl:defclass <simulation_obsts> (roslisp-msg-protocol:ros-message)
  ((circularObsts
    :reader circularObsts
    :initarg :circularObsts
    :type (cl:vector comm_msg-msg:simulation_circular_obst)
   :initform (cl:make-array 0 :element-type 'comm_msg-msg:simulation_circular_obst :initial-element (cl:make-instance 'comm_msg-msg:simulation_circular_obst))))
)

(cl:defclass simulation_obsts (<simulation_obsts>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <simulation_obsts>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'simulation_obsts)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<simulation_obsts> is deprecated: use comm_msg-msg:simulation_obsts instead.")))

(cl:ensure-generic-function 'circularObsts-val :lambda-list '(m))
(cl:defmethod circularObsts-val ((m <simulation_obsts>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:circularObsts-val is deprecated.  Use comm_msg-msg:circularObsts instead.")
  (circularObsts m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <simulation_obsts>) ostream)
  "Serializes a message object of type '<simulation_obsts>"
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'circularObsts))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'circularObsts))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <simulation_obsts>) istream)
  "Deserializes a message object of type '<simulation_obsts>"
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'circularObsts) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'circularObsts)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'comm_msg-msg:simulation_circular_obst))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<simulation_obsts>)))
  "Returns string type for a message object of type '<simulation_obsts>"
  "comm_msg/simulation_obsts")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'simulation_obsts)))
  "Returns string type for a message object of type 'simulation_obsts"
  "comm_msg/simulation_obsts")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<simulation_obsts>)))
  "Returns md5sum for a message object of type '<simulation_obsts>"
  "addd4aa7ed3126b9ca217aeb1d69aa70")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'simulation_obsts)))
  "Returns md5sum for a message object of type 'simulation_obsts"
  "addd4aa7ed3126b9ca217aeb1d69aa70")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<simulation_obsts>)))
  "Returns full string definition for message of type '<simulation_obsts>"
  (cl:format cl:nil "simulation_circular_obst[] circularObsts~%================================================================================~%MSG: comm_msg/simulation_circular_obst~%float32 x~%float32 y~%float32 radius~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'simulation_obsts)))
  "Returns full string definition for message of type 'simulation_obsts"
  (cl:format cl:nil "simulation_circular_obst[] circularObsts~%================================================================================~%MSG: comm_msg/simulation_circular_obst~%float32 x~%float32 y~%float32 radius~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <simulation_obsts>))
  (cl:+ 0
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'circularObsts) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <simulation_obsts>))
  "Converts a ROS message object to a list"
  (cl:list 'simulation_obsts
    (cl:cons ':circularObsts (circularObsts msg))
))
