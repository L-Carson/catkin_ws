;; Auto-generated. Do not edit!


(when (boundp 'comm_srvs::visionLabelCfg)
  (if (not (find-package "COMM_SRVS"))
    (make-package "COMM_SRVS"))
  (shadow 'visionLabelCfg (find-package "COMM_SRVS")))
(unless (find-package "COMM_SRVS::VISIONLABELCFG")
  (make-package "COMM_SRVS::VISIONLABELCFG"))

(in-package "ROS")
;;//! \htmlinclude visionLabelCfg.msg.html


(defclass comm_srvs::visionLabelCfg
  :super ros::object
  :slots (_vectDetLabel _vectSegLabel _isSupportThickPipe _thickPipeWidth ))

(defmethod comm_srvs::visionLabelCfg
  (:init
   (&key
    ((:vectDetLabel __vectDetLabel) (make-array 0 :initial-element 0 :element-type :integer))
    ((:vectSegLabel __vectSegLabel) (make-array 0 :initial-element 0 :element-type :integer))
    ((:isSupportThickPipe __isSupportThickPipe) nil)
    ((:thickPipeWidth __thickPipeWidth) 0.0)
    )
   (send-super :init)
   (setq _vectDetLabel __vectDetLabel)
   (setq _vectSegLabel __vectSegLabel)
   (setq _isSupportThickPipe __isSupportThickPipe)
   (setq _thickPipeWidth (float __thickPipeWidth))
   self)
  (:vectDetLabel
   (&optional __vectDetLabel)
   (if __vectDetLabel (setq _vectDetLabel __vectDetLabel)) _vectDetLabel)
  (:vectSegLabel
   (&optional __vectSegLabel)
   (if __vectSegLabel (setq _vectSegLabel __vectSegLabel)) _vectSegLabel)
  (:isSupportThickPipe
   (&optional (__isSupportThickPipe :null))
   (if (not (eq __isSupportThickPipe :null)) (setq _isSupportThickPipe __isSupportThickPipe)) _isSupportThickPipe)
  (:thickPipeWidth
   (&optional __thickPipeWidth)
   (if __thickPipeWidth (setq _thickPipeWidth __thickPipeWidth)) _thickPipeWidth)
  (:serialization-length
   ()
   (+
    ;; uint32[] _vectDetLabel
    (* 4    (length _vectDetLabel)) 4
    ;; uint32[] _vectSegLabel
    (* 4    (length _vectSegLabel)) 4
    ;; bool _isSupportThickPipe
    1
    ;; float32 _thickPipeWidth
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint32[] _vectDetLabel
     (write-long (length _vectDetLabel) s)
     (dotimes (i (length _vectDetLabel))
       (write-long (elt _vectDetLabel i) s)
       )
     ;; uint32[] _vectSegLabel
     (write-long (length _vectSegLabel) s)
     (dotimes (i (length _vectSegLabel))
       (write-long (elt _vectSegLabel i) s)
       )
     ;; bool _isSupportThickPipe
       (if _isSupportThickPipe (write-byte -1 s) (write-byte 0 s))
     ;; float32 _thickPipeWidth
       (sys::poke _thickPipeWidth (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint32[] _vectDetLabel
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _vectDetLabel (instantiate integer-vector n))
     (dotimes (i n)
     (setf (elt _vectDetLabel i) (sys::peek buf ptr- :integer)) (incf ptr- 4)
     ))
   ;; uint32[] _vectSegLabel
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _vectSegLabel (instantiate integer-vector n))
     (dotimes (i n)
     (setf (elt _vectSegLabel i) (sys::peek buf ptr- :integer)) (incf ptr- 4)
     ))
   ;; bool _isSupportThickPipe
     (setq _isSupportThickPipe (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;; float32 _thickPipeWidth
     (setq _thickPipeWidth (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get comm_srvs::visionLabelCfg :md5sum-) "c57896cd544874b34ea95938d00716d9")
(setf (get comm_srvs::visionLabelCfg :datatype-) "comm_srvs/visionLabelCfg")
(setf (get comm_srvs::visionLabelCfg :definition-)
      "uint32[] vectDetLabel
uint32[] vectSegLabel

bool    isSupportThickPipe #是否支持粗水管类别
float32 thickPipeWidth     #粗水管宽度阈值
")



(provide :comm_srvs/visionLabelCfg "c57896cd544874b34ea95938d00716d9")


