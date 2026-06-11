;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::trash_box_area_obst_state)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'trash_box_area_obst_state (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::TRASH_BOX_AREA_OBST_STATE")
  (make-package "COMM_MSG::TRASH_BOX_AREA_OBST_STATE"))

(in-package "ROS")
;;//! \htmlinclude trash_box_area_obst_state.msg.html


(defclass comm_msg::trash_box_area_obst_state
  :super ros::object
  :slots (_isExistHighObstInBoxArea _isExistHighObstOutBoxArea ))

(defmethod comm_msg::trash_box_area_obst_state
  (:init
   (&key
    ((:isExistHighObstInBoxArea __isExistHighObstInBoxArea) nil)
    ((:isExistHighObstOutBoxArea __isExistHighObstOutBoxArea) nil)
    )
   (send-super :init)
   (setq _isExistHighObstInBoxArea __isExistHighObstInBoxArea)
   (setq _isExistHighObstOutBoxArea __isExistHighObstOutBoxArea)
   self)
  (:isExistHighObstInBoxArea
   (&optional (__isExistHighObstInBoxArea :null))
   (if (not (eq __isExistHighObstInBoxArea :null)) (setq _isExistHighObstInBoxArea __isExistHighObstInBoxArea)) _isExistHighObstInBoxArea)
  (:isExistHighObstOutBoxArea
   (&optional (__isExistHighObstOutBoxArea :null))
   (if (not (eq __isExistHighObstOutBoxArea :null)) (setq _isExistHighObstOutBoxArea __isExistHighObstOutBoxArea)) _isExistHighObstOutBoxArea)
  (:serialization-length
   ()
   (+
    ;; bool _isExistHighObstInBoxArea
    1
    ;; bool _isExistHighObstOutBoxArea
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; bool _isExistHighObstInBoxArea
       (if _isExistHighObstInBoxArea (write-byte -1 s) (write-byte 0 s))
     ;; bool _isExistHighObstOutBoxArea
       (if _isExistHighObstOutBoxArea (write-byte -1 s) (write-byte 0 s))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; bool _isExistHighObstInBoxArea
     (setq _isExistHighObstInBoxArea (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;; bool _isExistHighObstOutBoxArea
     (setq _isExistHighObstOutBoxArea (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;;
   self)
  )

(setf (get comm_msg::trash_box_area_obst_state :md5sum-) "4ae1ec08cec9667102da97364468a9db")
(setf (get comm_msg::trash_box_area_obst_state :datatype-) "comm_msg/trash_box_area_obst_state")
(setf (get comm_msg::trash_box_area_obst_state :definition-)
      "# trash box area obst state

bool  isExistHighObstInBoxArea      #  车辆后方在垃圾箱翻起内的区域是否存在高于垃圾箱的障碍物
bool  isExistHighObstOutBoxArea     #  车辆后方在垃圾箱翻起外的区域是否存在高于垃圾箱的障碍物

")



(provide :comm_msg/trash_box_area_obst_state "4ae1ec08cec9667102da97364468a9db")


