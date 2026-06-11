;; Auto-generated. Do not edit!


(when (boundp 'comm_srvs::visionFuncCfg)
  (if (not (find-package "COMM_SRVS"))
    (make-package "COMM_SRVS"))
  (shadow 'visionFuncCfg (find-package "COMM_SRVS")))
(unless (find-package "COMM_SRVS::VISIONFUNCCFG")
  (make-package "COMM_SRVS::VISIONFUNCCFG"))

(in-package "ROS")
;;//! \htmlinclude visionFuncCfg.msg.html


(defclass comm_srvs::visionFuncCfg
  :super ros::object
  :slots (_funcId _ipu _hz ))

(defmethod comm_srvs::visionFuncCfg
  (:init
   (&key
    ((:funcId __funcId) 0)
    ((:ipu __ipu) nil)
    ((:hz __hz) 0)
    )
   (send-super :init)
   (setq _funcId (round __funcId))
   (setq _ipu __ipu)
   (setq _hz (round __hz))
   self)
  (:funcId
   (&optional __funcId)
   (if __funcId (setq _funcId __funcId)) _funcId)
  (:ipu
   (&optional (__ipu :null))
   (if (not (eq __ipu :null)) (setq _ipu __ipu)) _ipu)
  (:hz
   (&optional __hz)
   (if __hz (setq _hz __hz)) _hz)
  (:serialization-length
   ()
   (+
    ;; uint8 _funcId
    1
    ;; bool _ipu
    1
    ;; uint8 _hz
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint8 _funcId
       (write-byte _funcId s)
     ;; bool _ipu
       (if _ipu (write-byte -1 s) (write-byte 0 s))
     ;; uint8 _hz
       (write-byte _hz s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint8 _funcId
     (setq _funcId (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; bool _ipu
     (setq _ipu (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;; uint8 _hz
     (setq _hz (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;;
   self)
  )

(setf (get comm_srvs::visionFuncCfg :md5sum-) "788b771f7f7a06c374a8246119139c2d")
(setf (get comm_srvs::visionFuncCfg :datatype-) "comm_srvs/visionFuncCfg")
(setf (get comm_srvs::visionFuncCfg :definition-)
      "uint8   funcId      # 功能ID
bool    ipu         # 是否使用图像处理单元
uint8   hz          # 执行频率
")



(provide :comm_srvs/visionFuncCfg "788b771f7f7a06c374a8246119139c2d")


