; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude trash_box_obst_state.msg.html

(cl:defclass <trash_box_obst_state> (roslisp-msg-protocol:ros-message)
  ((exisitObstUp
    :reader exisitObstUp
    :initarg :exisitObstUp
    :type cl:boolean
    :initform cl:nil)
   (exisitObstDown
    :reader exisitObstDown
    :initarg :exisitObstDown
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass trash_box_obst_state (<trash_box_obst_state>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <trash_box_obst_state>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'trash_box_obst_state)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<trash_box_obst_state> is deprecated: use comm_msg-msg:trash_box_obst_state instead.")))

(cl:ensure-generic-function 'exisitObstUp-val :lambda-list '(m))
(cl:defmethod exisitObstUp-val ((m <trash_box_obst_state>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:exisitObstUp-val is deprecated.  Use comm_msg-msg:exisitObstUp instead.")
  (exisitObstUp m))

(cl:ensure-generic-function 'exisitObstDown-val :lambda-list '(m))
(cl:defmethod exisitObstDown-val ((m <trash_box_obst_state>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:exisitObstDown-val is deprecated.  Use comm_msg-msg:exisitObstDown instead.")
  (exisitObstDown m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <trash_box_obst_state>) ostream)
  "Serializes a message object of type '<trash_box_obst_state>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'exisitObstUp) 1 0)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'exisitObstDown) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <trash_box_obst_state>) istream)
  "Deserializes a message object of type '<trash_box_obst_state>"
    (cl:setf (cl:slot-value msg 'exisitObstUp) (cl:not (cl:zerop (cl:read-byte istream))))
    (cl:setf (cl:slot-value msg 'exisitObstDown) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<trash_box_obst_state>)))
  "Returns string type for a message object of type '<trash_box_obst_state>"
  "comm_msg/trash_box_obst_state")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'trash_box_obst_state)))
  "Returns string type for a message object of type 'trash_box_obst_state"
  "comm_msg/trash_box_obst_state")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<trash_box_obst_state>)))
  "Returns md5sum for a message object of type '<trash_box_obst_state>"
  "051ec62ff947ba3d46f50bc3f41f117f")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'trash_box_obst_state)))
  "Returns md5sum for a message object of type 'trash_box_obst_state"
  "051ec62ff947ba3d46f50bc3f41f117f")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<trash_box_obst_state>)))
  "Returns full string definition for message of type '<trash_box_obst_state>"
  (cl:format cl:nil "# trash box obst state~%bool  exisitObstUp        #  垃圾箱抬升方向是否有障碍~%bool  exisitObstDown      #  垃圾箱回收方向是否有障碍~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'trash_box_obst_state)))
  "Returns full string definition for message of type 'trash_box_obst_state"
  (cl:format cl:nil "# trash box obst state~%bool  exisitObstUp        #  垃圾箱抬升方向是否有障碍~%bool  exisitObstDown      #  垃圾箱回收方向是否有障碍~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <trash_box_obst_state>))
  (cl:+ 0
     1
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <trash_box_obst_state>))
  "Converts a ROS message object to a list"
  (cl:list 'trash_box_obst_state
    (cl:cons ':exisitObstUp (exisitObstUp msg))
    (cl:cons ':exisitObstDown (exisitObstDown msg))
))
