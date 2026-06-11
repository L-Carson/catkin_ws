; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude move_close_loop_ctrl.msg.html

(cl:defclass <move_close_loop_ctrl> (roslisp-msg-protocol:ros-message)
  ((plannedSteeringAngle
    :reader plannedSteeringAngle
    :initarg :plannedSteeringAngle
    :type cl:float
    :initform 0.0)
   (plannedMoveLinearVel
    :reader plannedMoveLinearVel
    :initarg :plannedMoveLinearVel
    :type cl:float
    :initform 0.0)
   (controlSteeringAngle
    :reader controlSteeringAngle
    :initarg :controlSteeringAngle
    :type cl:float
    :initform 0.0)
   (controlMoveLinearVel
    :reader controlMoveLinearVel
    :initarg :controlMoveLinearVel
    :type cl:float
    :initform 0.0)
   (measureSteeringAngle
    :reader measureSteeringAngle
    :initarg :measureSteeringAngle
    :type cl:float
    :initform 0.0)
   (measureMoveLinearVel
    :reader measureMoveLinearVel
    :initarg :measureMoveLinearVel
    :type cl:float
    :initform 0.0))
)

(cl:defclass move_close_loop_ctrl (<move_close_loop_ctrl>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <move_close_loop_ctrl>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'move_close_loop_ctrl)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<move_close_loop_ctrl> is deprecated: use comm_msg-msg:move_close_loop_ctrl instead.")))

(cl:ensure-generic-function 'plannedSteeringAngle-val :lambda-list '(m))
(cl:defmethod plannedSteeringAngle-val ((m <move_close_loop_ctrl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:plannedSteeringAngle-val is deprecated.  Use comm_msg-msg:plannedSteeringAngle instead.")
  (plannedSteeringAngle m))

(cl:ensure-generic-function 'plannedMoveLinearVel-val :lambda-list '(m))
(cl:defmethod plannedMoveLinearVel-val ((m <move_close_loop_ctrl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:plannedMoveLinearVel-val is deprecated.  Use comm_msg-msg:plannedMoveLinearVel instead.")
  (plannedMoveLinearVel m))

(cl:ensure-generic-function 'controlSteeringAngle-val :lambda-list '(m))
(cl:defmethod controlSteeringAngle-val ((m <move_close_loop_ctrl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:controlSteeringAngle-val is deprecated.  Use comm_msg-msg:controlSteeringAngle instead.")
  (controlSteeringAngle m))

(cl:ensure-generic-function 'controlMoveLinearVel-val :lambda-list '(m))
(cl:defmethod controlMoveLinearVel-val ((m <move_close_loop_ctrl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:controlMoveLinearVel-val is deprecated.  Use comm_msg-msg:controlMoveLinearVel instead.")
  (controlMoveLinearVel m))

(cl:ensure-generic-function 'measureSteeringAngle-val :lambda-list '(m))
(cl:defmethod measureSteeringAngle-val ((m <move_close_loop_ctrl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:measureSteeringAngle-val is deprecated.  Use comm_msg-msg:measureSteeringAngle instead.")
  (measureSteeringAngle m))

(cl:ensure-generic-function 'measureMoveLinearVel-val :lambda-list '(m))
(cl:defmethod measureMoveLinearVel-val ((m <move_close_loop_ctrl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:measureMoveLinearVel-val is deprecated.  Use comm_msg-msg:measureMoveLinearVel instead.")
  (measureMoveLinearVel m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <move_close_loop_ctrl>) ostream)
  "Serializes a message object of type '<move_close_loop_ctrl>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'plannedSteeringAngle))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'plannedMoveLinearVel))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'controlSteeringAngle))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'controlMoveLinearVel))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'measureSteeringAngle))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'measureMoveLinearVel))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <move_close_loop_ctrl>) istream)
  "Deserializes a message object of type '<move_close_loop_ctrl>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'plannedSteeringAngle) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'plannedMoveLinearVel) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'controlSteeringAngle) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'controlMoveLinearVel) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'measureSteeringAngle) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'measureMoveLinearVel) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<move_close_loop_ctrl>)))
  "Returns string type for a message object of type '<move_close_loop_ctrl>"
  "comm_msg/move_close_loop_ctrl")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'move_close_loop_ctrl)))
  "Returns string type for a message object of type 'move_close_loop_ctrl"
  "comm_msg/move_close_loop_ctrl")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<move_close_loop_ctrl>)))
  "Returns md5sum for a message object of type '<move_close_loop_ctrl>"
  "e51a1651c7c8b7b7a901faef61d12ad6")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'move_close_loop_ctrl)))
  "Returns md5sum for a message object of type 'move_close_loop_ctrl"
  "e51a1651c7c8b7b7a901faef61d12ad6")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<move_close_loop_ctrl>)))
  "Returns full string definition for message of type '<move_close_loop_ctrl>"
  (cl:format cl:nil "float32 plannedSteeringAngle~%float32 plannedMoveLinearVel~%float32 controlSteeringAngle~%float32 controlMoveLinearVel~%float32 measureSteeringAngle~%float32 measureMoveLinearVel~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'move_close_loop_ctrl)))
  "Returns full string definition for message of type 'move_close_loop_ctrl"
  (cl:format cl:nil "float32 plannedSteeringAngle~%float32 plannedMoveLinearVel~%float32 controlSteeringAngle~%float32 controlMoveLinearVel~%float32 measureSteeringAngle~%float32 measureMoveLinearVel~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <move_close_loop_ctrl>))
  (cl:+ 0
     4
     4
     4
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <move_close_loop_ctrl>))
  "Converts a ROS message object to a list"
  (cl:list 'move_close_loop_ctrl
    (cl:cons ':plannedSteeringAngle (plannedSteeringAngle msg))
    (cl:cons ':plannedMoveLinearVel (plannedMoveLinearVel msg))
    (cl:cons ':controlSteeringAngle (controlSteeringAngle msg))
    (cl:cons ':controlMoveLinearVel (controlMoveLinearVel msg))
    (cl:cons ':measureSteeringAngle (measureSteeringAngle msg))
    (cl:cons ':measureMoveLinearVel (measureMoveLinearVel msg))
))
