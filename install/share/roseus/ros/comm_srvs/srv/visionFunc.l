;; Auto-generated. Do not edit!


(when (boundp 'comm_srvs::visionFunc)
  (if (not (find-package "COMM_SRVS"))
    (make-package "COMM_SRVS"))
  (shadow 'visionFunc (find-package "COMM_SRVS")))
(unless (find-package "COMM_SRVS::VISIONFUNC")
  (make-package "COMM_SRVS::VISIONFUNC"))
(unless (find-package "COMM_SRVS::VISIONFUNCREQUEST")
  (make-package "COMM_SRVS::VISIONFUNCREQUEST"))
(unless (find-package "COMM_SRVS::VISIONFUNCRESPONSE")
  (make-package "COMM_SRVS::VISIONFUNCRESPONSE"))

(in-package "ROS")





(defclass comm_srvs::visionFuncRequest
  :super ros::object
  :slots ())

(defmethod comm_srvs::visionFuncRequest
  (:init
   (&key
    )
   (send-super :init)
   self)
  (:serialization-length
   ()
   (+
    0
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;;
   self)
  )

(defclass comm_srvs::visionFuncResponse
  :super ros::object
  :slots (_success _isUseApi _visionFuncCfgArray _visionLabelCfg ))

(defmethod comm_srvs::visionFuncResponse
  (:init
   (&key
    ((:success __success) nil)
    ((:isUseApi __isUseApi) nil)
    ((:visionFuncCfgArray __visionFuncCfgArray) ())
    ((:visionLabelCfg __visionLabelCfg) (instance comm_srvs::visionLabelCfg :init))
    )
   (send-super :init)
   (setq _success __success)
   (setq _isUseApi __isUseApi)
   (setq _visionFuncCfgArray __visionFuncCfgArray)
   (setq _visionLabelCfg __visionLabelCfg)
   self)
  (:success
   (&optional (__success :null))
   (if (not (eq __success :null)) (setq _success __success)) _success)
  (:isUseApi
   (&optional (__isUseApi :null))
   (if (not (eq __isUseApi :null)) (setq _isUseApi __isUseApi)) _isUseApi)
  (:visionFuncCfgArray
   (&rest __visionFuncCfgArray)
   (if (keywordp (car __visionFuncCfgArray))
       (send* _visionFuncCfgArray __visionFuncCfgArray)
     (progn
       (if __visionFuncCfgArray (setq _visionFuncCfgArray (car __visionFuncCfgArray)))
       _visionFuncCfgArray)))
  (:visionLabelCfg
   (&rest __visionLabelCfg)
   (if (keywordp (car __visionLabelCfg))
       (send* _visionLabelCfg __visionLabelCfg)
     (progn
       (if __visionLabelCfg (setq _visionLabelCfg (car __visionLabelCfg)))
       _visionLabelCfg)))
  (:serialization-length
   ()
   (+
    ;; bool _success
    1
    ;; bool _isUseApi
    1
    ;; comm_srvs/visionFuncCfg[] _visionFuncCfgArray
    (apply #'+ (send-all _visionFuncCfgArray :serialization-length)) 4
    ;; comm_srvs/visionLabelCfg _visionLabelCfg
    (send _visionLabelCfg :serialization-length)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; bool _success
       (if _success (write-byte -1 s) (write-byte 0 s))
     ;; bool _isUseApi
       (if _isUseApi (write-byte -1 s) (write-byte 0 s))
     ;; comm_srvs/visionFuncCfg[] _visionFuncCfgArray
     (write-long (length _visionFuncCfgArray) s)
     (dolist (elem _visionFuncCfgArray)
       (send elem :serialize s)
       )
     ;; comm_srvs/visionLabelCfg _visionLabelCfg
       (send _visionLabelCfg :serialize s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; bool _success
     (setq _success (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;; bool _isUseApi
     (setq _isUseApi (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;; comm_srvs/visionFuncCfg[] _visionFuncCfgArray
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _visionFuncCfgArray (let (r) (dotimes (i n) (push (instance comm_srvs::visionFuncCfg :init) r)) r))
     (dolist (elem- _visionFuncCfgArray)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;; comm_srvs/visionLabelCfg _visionLabelCfg
     (send _visionLabelCfg :deserialize buf ptr-) (incf ptr- (send _visionLabelCfg :serialization-length))
   ;;
   self)
  )

(defclass comm_srvs::visionFunc
  :super ros::object
  :slots ())

(setf (get comm_srvs::visionFunc :md5sum-) "adf5e73fc1530eec7e82d92a5cfadc9f")
(setf (get comm_srvs::visionFunc :datatype-) "comm_srvs/visionFunc")
(setf (get comm_srvs::visionFunc :request) comm_srvs::visionFuncRequest)
(setf (get comm_srvs::visionFunc :response) comm_srvs::visionFuncResponse)

(defmethod comm_srvs::visionFuncRequest
  (:response () (instance comm_srvs::visionFuncResponse :init)))

(setf (get comm_srvs::visionFuncRequest :md5sum-) "adf5e73fc1530eec7e82d92a5cfadc9f")
(setf (get comm_srvs::visionFuncRequest :datatype-) "comm_srvs/visionFuncRequest")
(setf (get comm_srvs::visionFuncRequest :definition-)
      "---
bool                success
bool                isUseApi
visionFuncCfg[]     visionFuncCfgArray
visionLabelCfg      visionLabelCfg

================================================================================
MSG: comm_srvs/visionFuncCfg
uint8   funcId      # 功能ID
bool    ipu         # 是否使用图像处理单元
uint8   hz          # 执行频率
================================================================================
MSG: comm_srvs/visionLabelCfg
uint32[] vectDetLabel
uint32[] vectSegLabel

bool    isSupportThickPipe #是否支持粗水管类别
")

(setf (get comm_srvs::visionFuncResponse :md5sum-) "adf5e73fc1530eec7e82d92a5cfadc9f")
(setf (get comm_srvs::visionFuncResponse :datatype-) "comm_srvs/visionFuncResponse")
(setf (get comm_srvs::visionFuncResponse :definition-)
      "---
bool                success
bool                isUseApi
visionFuncCfg[]     visionFuncCfgArray
visionLabelCfg      visionLabelCfg

================================================================================
MSG: comm_srvs/visionFuncCfg
uint8   funcId      # 功能ID
bool    ipu         # 是否使用图像处理单元
uint8   hz          # 执行频率
================================================================================
MSG: comm_srvs/visionLabelCfg
uint32[] vectDetLabel
uint32[] vectSegLabel

bool    isSupportThickPipe #是否支持粗水管类别
")



(provide :comm_srvs/visionFunc "adf5e73fc1530eec7e82d92a5cfadc9f")


