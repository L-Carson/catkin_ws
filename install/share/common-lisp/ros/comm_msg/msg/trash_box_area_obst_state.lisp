; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude trash_box_area_obst_state.msg.html

(cl:defclass <trash_box_area_obst_state> (roslisp-msg-protocol:ros-message)
  ((isExistHighObstInBoxArea
    :reader isExistHighObstInBoxArea
    :initarg :isExistHighObstInBoxArea
    :type cl:boolean
    :initform cl:nil)
   (isExistHighObstOutBoxArea
    :reader isExistHighObstOutBoxArea
    :initarg :isExistHighObstOutBoxArea
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass trash_box_area_obst_state (<trash_box_area_obst_state>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <trash_box_area_obst_state>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'trash_box_area_obst_state)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<trash_box_area_obst_state> is deprecated: use comm_msg-msg:trash_box_area_obst_state instead.")))

(cl:ensure-generic-function 'isExistHighObstInBoxArea-val :lambda-list '(m))
(cl:defmethod isExistHighObstInBoxArea-val ((m <trash_box_area_obst_state>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isExistHighObstInBoxArea-val is deprecated.  Use comm_msg-msg:isExistHighObstInBoxArea instead.")
  (isExistHighObstInBoxArea m))

(cl:ensure-generic-function 'isExistHighObstOutBoxArea-val :lambda-list '(m))
(cl:defmethod isExistHighObstOutBoxArea-val ((m <trash_box_area_obst_state>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:isExistHighObstOutBoxArea-val is deprecated.  Use comm_msg-msg:isExistHighObstOutBoxArea instead.")
  (isExistHighObstOutBoxArea m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <trash_box_area_obst_state>) ostream)
  "Serializes a message object of type '<trash_box_area_obst_state>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'isExistHighObstInBoxArea) 1 0)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'isExistHighObstOutBoxArea) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <trash_box_area_obst_state>) istream)
  "Deserializes a message object of type '<trash_box_area_obst_state>"
    (cl:setf (cl:slot-value msg 'isExistHighObstInBoxArea) (cl:not (cl:zerop (cl:read-byte istream))))
    (cl:setf (cl:slot-value msg 'isExistHighObstOutBoxArea) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<trash_box_area_obst_state>)))
  "Returns string type for a message object of type '<trash_box_area_obst_state>"
  "comm_msg/trash_box_area_obst_state")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'trash_box_area_obst_state)))
  "Returns string type for a message object of type 'trash_box_area_obst_state"
  "comm_msg/trash_box_area_obst_state")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<trash_box_area_obst_state>)))
  "Returns md5sum for a message object of type '<trash_box_area_obst_state>"
  "4ae1ec08cec9667102da97364468a9db")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'trash_box_area_obst_state)))
  "Returns md5sum for a message object of type 'trash_box_area_obst_state"
  "4ae1ec08cec9667102da97364468a9db")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<trash_box_area_obst_state>)))
  "Returns full string definition for message of type '<trash_box_area_obst_state>"
  (cl:format cl:nil "# trash box area obst state~%~%bool  isExistHighObstInBoxArea      #  车辆后方在垃圾箱翻起内的区域是否存在高于垃圾箱的障碍物~%bool  isExistHighObstOutBoxArea     #  车辆后方在垃圾箱翻起外的区域是否存在高于垃圾箱的障碍物~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'trash_box_area_obst_state)))
  "Returns full string definition for message of type 'trash_box_area_obst_state"
  (cl:format cl:nil "# trash box area obst state~%~%bool  isExistHighObstInBoxArea      #  车辆后方在垃圾箱翻起内的区域是否存在高于垃圾箱的障碍物~%bool  isExistHighObstOutBoxArea     #  车辆后方在垃圾箱翻起外的区域是否存在高于垃圾箱的障碍物~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <trash_box_area_obst_state>))
  (cl:+ 0
     1
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <trash_box_area_obst_state>))
  "Converts a ROS message object to a list"
  (cl:list 'trash_box_area_obst_state
    (cl:cons ':isExistHighObstInBoxArea (isExistHighObstInBoxArea msg))
    (cl:cons ':isExistHighObstOutBoxArea (isExistHighObstOutBoxArea msg))
))
