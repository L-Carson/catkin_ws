;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::serialPassData)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'serialPassData (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::SERIALPASSDATA")
  (make-package "COMM_MSG::SERIALPASSDATA"))

(in-package "ROS")
;;//! \htmlinclude serialPassData.msg.html


(defclass comm_msg::serialPassData
  :super ros::object
  :slots (_dataStream ))

(defmethod comm_msg::serialPassData
  (:init
   (&key
    ((:dataStream __dataStream) (make-array 0 :initial-element 0 :element-type :char))
    )
   (send-super :init)
   (setq _dataStream __dataStream)
   self)
  (:dataStream
   (&optional __dataStream)
   (if __dataStream (setq _dataStream __dataStream)) _dataStream)
  (:serialization-length
   ()
   (+
    ;; uint8[] _dataStream
    (* 1    (length _dataStream)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint8[] _dataStream
     (write-long (length _dataStream) s)
     (princ _dataStream s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint8[] _dataStream
   (let ((n (sys::peek buf ptr- :integer))) (incf ptr- 4)
     (setq _dataStream (make-array n :element-type :char))
     (replace _dataStream buf :start2 ptr-) (incf ptr- n))
   ;;
   self)
  )

(setf (get comm_msg::serialPassData :md5sum-) "356d46a591b661111ed0ede4b4ae78ca")
(setf (get comm_msg::serialPassData :datatype-) "comm_msg/serialPassData")
(setf (get comm_msg::serialPassData :definition-)
      "# serial byte stream
uint8[] dataStream

")



(provide :comm_msg/serialPassData "356d46a591b661111ed0ede4b4ae78ca")


