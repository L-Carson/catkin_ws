; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude pose_score.msg.html

(cl:defclass <pose_score> (roslisp-msg-protocol:ros-message)
  ((score
    :reader score
    :initarg :score
    :type cl:float
    :initform 0.0)
   (pose
    :reader pose
    :initarg :pose
    :type comm_msg-msg:pose3D
    :initform (cl:make-instance 'comm_msg-msg:pose3D)))
)

(cl:defclass pose_score (<pose_score>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <pose_score>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'pose_score)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<pose_score> is deprecated: use comm_msg-msg:pose_score instead.")))

(cl:ensure-generic-function 'score-val :lambda-list '(m))
(cl:defmethod score-val ((m <pose_score>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:score-val is deprecated.  Use comm_msg-msg:score instead.")
  (score m))

(cl:ensure-generic-function 'pose-val :lambda-list '(m))
(cl:defmethod pose-val ((m <pose_score>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:pose-val is deprecated.  Use comm_msg-msg:pose instead.")
  (pose m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <pose_score>) ostream)
  "Serializes a message object of type '<pose_score>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'score))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'pose) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <pose_score>) istream)
  "Deserializes a message object of type '<pose_score>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'score) (roslisp-utils:decode-single-float-bits bits)))
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'pose) istream)
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<pose_score>)))
  "Returns string type for a message object of type '<pose_score>"
  "comm_msg/pose_score")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'pose_score)))
  "Returns string type for a message object of type 'pose_score"
  "comm_msg/pose_score")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<pose_score>)))
  "Returns md5sum for a message object of type '<pose_score>"
  "1ba278e321050d4e0b42ab7a7a69fe0c")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'pose_score)))
  "Returns md5sum for a message object of type 'pose_score"
  "1ba278e321050d4e0b42ab7a7a69fe0c")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<pose_score>)))
  "Returns full string definition for message of type '<pose_score>"
  (cl:format cl:nil "float32 score~%pose3D  pose~%================================================================================~%MSG: comm_msg/pose3D~%float32 posX~%float32 posY~%float32 posZ~%float32 roll~%float32 pitch~%float32 yaw~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'pose_score)))
  "Returns full string definition for message of type 'pose_score"
  (cl:format cl:nil "float32 score~%pose3D  pose~%================================================================================~%MSG: comm_msg/pose3D~%float32 posX~%float32 posY~%float32 posZ~%float32 roll~%float32 pitch~%float32 yaw~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <pose_score>))
  (cl:+ 0
     4
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'pose))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <pose_score>))
  "Converts a ROS message object to a list"
  (cl:list 'pose_score
    (cl:cons ':score (score msg))
    (cl:cons ':pose (pose msg))
))
