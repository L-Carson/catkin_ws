; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude coor3DArray.msg.html

(cl:defclass <coor3DArray> (roslisp-msg-protocol:ros-message)
  ((coor3DArray
    :reader coor3DArray
    :initarg :coor3DArray
    :type (cl:vector comm_msg-msg:coor3D)
   :initform (cl:make-array 0 :element-type 'comm_msg-msg:coor3D :initial-element (cl:make-instance 'comm_msg-msg:coor3D))))
)

(cl:defclass coor3DArray (<coor3DArray>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <coor3DArray>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'coor3DArray)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<coor3DArray> is deprecated: use comm_msg-msg:coor3DArray instead.")))

(cl:ensure-generic-function 'coor3DArray-val :lambda-list '(m))
(cl:defmethod coor3DArray-val ((m <coor3DArray>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:coor3DArray-val is deprecated.  Use comm_msg-msg:coor3DArray instead.")
  (coor3DArray m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <coor3DArray>) ostream)
  "Serializes a message object of type '<coor3DArray>"
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'coor3DArray))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'coor3DArray))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <coor3DArray>) istream)
  "Deserializes a message object of type '<coor3DArray>"
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'coor3DArray) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'coor3DArray)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'comm_msg-msg:coor3D))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<coor3DArray>)))
  "Returns string type for a message object of type '<coor3DArray>"
  "comm_msg/coor3DArray")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'coor3DArray)))
  "Returns string type for a message object of type 'coor3DArray"
  "comm_msg/coor3DArray")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<coor3DArray>)))
  "Returns md5sum for a message object of type '<coor3DArray>"
  "f1d540e658b8fab024c8f2c1ea50f3fd")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'coor3DArray)))
  "Returns md5sum for a message object of type 'coor3DArray"
  "f1d540e658b8fab024c8f2c1ea50f3fd")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<coor3DArray>)))
  "Returns full string definition for message of type '<coor3DArray>"
  (cl:format cl:nil "coor3D[]  coor3DArray~%================================================================================~%MSG: comm_msg/coor3D~%float32 pos_x~%float32 pos_y~%float32 pos_z~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'coor3DArray)))
  "Returns full string definition for message of type 'coor3DArray"
  (cl:format cl:nil "coor3D[]  coor3DArray~%================================================================================~%MSG: comm_msg/coor3D~%float32 pos_x~%float32 pos_y~%float32 pos_z~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <coor3DArray>))
  (cl:+ 0
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'coor3DArray) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <coor3DArray>))
  "Converts a ROS message object to a list"
  (cl:list 'coor3DArray
    (cl:cons ':coor3DArray (coor3DArray msg))
))
