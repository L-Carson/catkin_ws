; Auto-generated. Do not edit!


(cl:in-package comm_msg-msg)


;//! \htmlinclude roadmark.msg.html

(cl:defclass <roadmark> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (polygons
    :reader polygons
    :initarg :polygons
    :type (cl:vector geometry_msgs-msg:Polygon)
   :initform (cl:make-array 0 :element-type 'geometry_msgs-msg:Polygon :initial-element (cl:make-instance 'geometry_msgs-msg:Polygon))))
)

(cl:defclass roadmark (<roadmark>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <roadmark>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'roadmark)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm_msg-msg:<roadmark> is deprecated: use comm_msg-msg:roadmark instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <roadmark>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:header-val is deprecated.  Use comm_msg-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'polygons-val :lambda-list '(m))
(cl:defmethod polygons-val ((m <roadmark>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm_msg-msg:polygons-val is deprecated.  Use comm_msg-msg:polygons instead.")
  (polygons m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <roadmark>) ostream)
  "Serializes a message object of type '<roadmark>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'polygons))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'polygons))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <roadmark>) istream)
  "Deserializes a message object of type '<roadmark>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'polygons) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'polygons)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'geometry_msgs-msg:Polygon))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<roadmark>)))
  "Returns string type for a message object of type '<roadmark>"
  "comm_msg/roadmark")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'roadmark)))
  "Returns string type for a message object of type 'roadmark"
  "comm_msg/roadmark")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<roadmark>)))
  "Returns md5sum for a message object of type '<roadmark>"
  "436cca0b8e50ceff14d30527fc67b7a4")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'roadmark)))
  "Returns md5sum for a message object of type 'roadmark"
  "436cca0b8e50ceff14d30527fc67b7a4")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<roadmark>)))
  "Returns full string definition for message of type '<roadmark>"
  (cl:format cl:nil "Header                  header~%geometry_msgs/Polygon[] polygons~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%================================================================================~%MSG: geometry_msgs/Polygon~%#A specification of a polygon where the first and last points are assumed to be connected~%Point32[] points~%~%================================================================================~%MSG: geometry_msgs/Point32~%# This contains the position of a point in free space(with 32 bits of precision).~%# It is recommeded to use Point wherever possible instead of Point32.  ~%# ~%# This recommendation is to promote interoperability.  ~%#~%# This message is designed to take up less space when sending~%# lots of points at once, as in the case of a PointCloud.  ~%~%float32 x~%float32 y~%float32 z~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'roadmark)))
  "Returns full string definition for message of type 'roadmark"
  (cl:format cl:nil "Header                  header~%geometry_msgs/Polygon[] polygons~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%================================================================================~%MSG: geometry_msgs/Polygon~%#A specification of a polygon where the first and last points are assumed to be connected~%Point32[] points~%~%================================================================================~%MSG: geometry_msgs/Point32~%# This contains the position of a point in free space(with 32 bits of precision).~%# It is recommeded to use Point wherever possible instead of Point32.  ~%# ~%# This recommendation is to promote interoperability.  ~%#~%# This message is designed to take up less space when sending~%# lots of points at once, as in the case of a PointCloud.  ~%~%float32 x~%float32 y~%float32 z~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <roadmark>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'polygons) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <roadmark>))
  "Converts a ROS message object to a list"
  (cl:list 'roadmark
    (cl:cons ':header (header msg))
    (cl:cons ':polygons (polygons msg))
))
