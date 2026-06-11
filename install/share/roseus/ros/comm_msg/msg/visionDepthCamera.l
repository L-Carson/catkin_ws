;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::visionDepthCamera)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'visionDepthCamera (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::VISIONDEPTHCAMERA")
  (make-package "COMM_MSG::VISIONDEPTHCAMERA"))

(in-package "ROS")
;;//! \htmlinclude visionDepthCamera.msg.html


(defclass comm_msg::visionDepthCamera
  :super ros::object
  :slots (_selectDepthCamera ))

(defmethod comm_msg::visionDepthCamera
  (:init
   (&key
    ((:selectDepthCamera __selectDepthCamera) 0)
    )
   (send-super :init)
   (setq _selectDepthCamera (round __selectDepthCamera))
   self)
  (:selectDepthCamera
   (&optional __selectDepthCamera)
   (if __selectDepthCamera (setq _selectDepthCamera __selectDepthCamera)) _selectDepthCamera)
  (:serialization-length
   ()
   (+
    ;; uint8 _selectDepthCamera
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint8 _selectDepthCamera
       (write-byte _selectDepthCamera s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint8 _selectDepthCamera
     (setq _selectDepthCamera (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;;
   self)
  )

(setf (get comm_msg::visionDepthCamera :md5sum-) "81b78f084b6d77e189e72dcfcb2c3fa3")
(setf (get comm_msg::visionDepthCamera :datatype-) "comm_msg/visionDepthCamera")
(setf (get comm_msg::visionDepthCamera :definition-)
      "# Select Depth Camera
# 0: not select depth camera
# 1: the head depth camera
# 2: the back depth camera
# 3: the left depth camera
# 4: the right depth camera

uint8 selectDepthCamera
")



(provide :comm_msg/visionDepthCamera "81b78f084b6d77e189e72dcfcb2c3fa3")


