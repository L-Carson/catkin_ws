;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::trash_box_obst_state)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'trash_box_obst_state (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::TRASH_BOX_OBST_STATE")
  (make-package "COMM_MSG::TRASH_BOX_OBST_STATE"))

(in-package "ROS")
;;//! \htmlinclude trash_box_obst_state.msg.html


(defclass comm_msg::trash_box_obst_state
  :super ros::object
  :slots (_exisitObstUp _exisitObstDown ))

(defmethod comm_msg::trash_box_obst_state
  (:init
   (&key
    ((:exisitObstUp __exisitObstUp) nil)
    ((:exisitObstDown __exisitObstDown) nil)
    )
   (send-super :init)
   (setq _exisitObstUp __exisitObstUp)
   (setq _exisitObstDown __exisitObstDown)
   self)
  (:exisitObstUp
   (&optional (__exisitObstUp :null))
   (if (not (eq __exisitObstUp :null)) (setq _exisitObstUp __exisitObstUp)) _exisitObstUp)
  (:exisitObstDown
   (&optional (__exisitObstDown :null))
   (if (not (eq __exisitObstDown :null)) (setq _exisitObstDown __exisitObstDown)) _exisitObstDown)
  (:serialization-length
   ()
   (+
    ;; bool _exisitObstUp
    1
    ;; bool _exisitObstDown
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; bool _exisitObstUp
       (if _exisitObstUp (write-byte -1 s) (write-byte 0 s))
     ;; bool _exisitObstDown
       (if _exisitObstDown (write-byte -1 s) (write-byte 0 s))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; bool _exisitObstUp
     (setq _exisitObstUp (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;; bool _exisitObstDown
     (setq _exisitObstDown (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;;
   self)
  )

(setf (get comm_msg::trash_box_obst_state :md5sum-) "051ec62ff947ba3d46f50bc3f41f117f")
(setf (get comm_msg::trash_box_obst_state :datatype-) "comm_msg/trash_box_obst_state")
(setf (get comm_msg::trash_box_obst_state :definition-)
      "# trash box obst state
bool  exisitObstUp        #  垃圾箱抬升方向是否有障碍
bool  exisitObstDown      #  垃圾箱回收方向是否有障碍
")



(provide :comm_msg/trash_box_obst_state "051ec62ff947ba3d46f50bc3f41f117f")


