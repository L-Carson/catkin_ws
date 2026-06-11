;; Auto-generated. Do not edit!


(when (boundp 'comm_srvs::MappingStart)
  (if (not (find-package "COMM_SRVS"))
    (make-package "COMM_SRVS"))
  (shadow 'MappingStart (find-package "COMM_SRVS")))
(unless (find-package "COMM_SRVS::MAPPINGSTART")
  (make-package "COMM_SRVS::MAPPINGSTART"))
(unless (find-package "COMM_SRVS::MAPPINGSTARTREQUEST")
  (make-package "COMM_SRVS::MAPPINGSTARTREQUEST"))
(unless (find-package "COMM_SRVS::MAPPINGSTARTRESPONSE")
  (make-package "COMM_SRVS::MAPPINGSTARTRESPONSE"))

(in-package "ROS")





(defclass comm_srvs::MappingStartRequest
  :super ros::object
  :slots (_mode _state _frontEndMethod _currFrameLeafSize _localMapLeafSize _localMapWindowSize _loopDetectDist _cfgDir _dstDir _bagPath _srcDir ))

(defmethod comm_srvs::MappingStartRequest
  (:init
   (&key
    ((:mode __mode) 0)
    ((:state __state) 0)
    ((:frontEndMethod __frontEndMethod) 0)
    ((:currFrameLeafSize __currFrameLeafSize) 0.0)
    ((:localMapLeafSize __localMapLeafSize) 0.0)
    ((:localMapWindowSize __localMapWindowSize) 0)
    ((:loopDetectDist __loopDetectDist) 0.0)
    ((:cfgDir __cfgDir) "")
    ((:dstDir __dstDir) "")
    ((:bagPath __bagPath) "")
    ((:srcDir __srcDir) "")
    )
   (send-super :init)
   (setq _mode (round __mode))
   (setq _state (round __state))
   (setq _frontEndMethod (round __frontEndMethod))
   (setq _currFrameLeafSize (float __currFrameLeafSize))
   (setq _localMapLeafSize (float __localMapLeafSize))
   (setq _localMapWindowSize (round __localMapWindowSize))
   (setq _loopDetectDist (float __loopDetectDist))
   (setq _cfgDir (string __cfgDir))
   (setq _dstDir (string __dstDir))
   (setq _bagPath (string __bagPath))
   (setq _srcDir (string __srcDir))
   self)
  (:mode
   (&optional __mode)
   (if __mode (setq _mode __mode)) _mode)
  (:state
   (&optional __state)
   (if __state (setq _state __state)) _state)
  (:frontEndMethod
   (&optional __frontEndMethod)
   (if __frontEndMethod (setq _frontEndMethod __frontEndMethod)) _frontEndMethod)
  (:currFrameLeafSize
   (&optional __currFrameLeafSize)
   (if __currFrameLeafSize (setq _currFrameLeafSize __currFrameLeafSize)) _currFrameLeafSize)
  (:localMapLeafSize
   (&optional __localMapLeafSize)
   (if __localMapLeafSize (setq _localMapLeafSize __localMapLeafSize)) _localMapLeafSize)
  (:localMapWindowSize
   (&optional __localMapWindowSize)
   (if __localMapWindowSize (setq _localMapWindowSize __localMapWindowSize)) _localMapWindowSize)
  (:loopDetectDist
   (&optional __loopDetectDist)
   (if __loopDetectDist (setq _loopDetectDist __loopDetectDist)) _loopDetectDist)
  (:cfgDir
   (&optional __cfgDir)
   (if __cfgDir (setq _cfgDir __cfgDir)) _cfgDir)
  (:dstDir
   (&optional __dstDir)
   (if __dstDir (setq _dstDir __dstDir)) _dstDir)
  (:bagPath
   (&optional __bagPath)
   (if __bagPath (setq _bagPath __bagPath)) _bagPath)
  (:srcDir
   (&optional __srcDir)
   (if __srcDir (setq _srcDir __srcDir)) _srcDir)
  (:serialization-length
   ()
   (+
    ;; int32 _mode
    4
    ;; int32 _state
    4
    ;; int32 _frontEndMethod
    4
    ;; float32 _currFrameLeafSize
    4
    ;; float32 _localMapLeafSize
    4
    ;; int32 _localMapWindowSize
    4
    ;; float32 _loopDetectDist
    4
    ;; string _cfgDir
    4 (length _cfgDir)
    ;; string _dstDir
    4 (length _dstDir)
    ;; string _bagPath
    4 (length _bagPath)
    ;; string _srcDir
    4 (length _srcDir)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int32 _mode
       (write-long _mode s)
     ;; int32 _state
       (write-long _state s)
     ;; int32 _frontEndMethod
       (write-long _frontEndMethod s)
     ;; float32 _currFrameLeafSize
       (sys::poke _currFrameLeafSize (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _localMapLeafSize
       (sys::poke _localMapLeafSize (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; int32 _localMapWindowSize
       (write-long _localMapWindowSize s)
     ;; float32 _loopDetectDist
       (sys::poke _loopDetectDist (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; string _cfgDir
       (write-long (length _cfgDir) s) (princ _cfgDir s)
     ;; string _dstDir
       (write-long (length _dstDir) s) (princ _dstDir s)
     ;; string _bagPath
       (write-long (length _bagPath) s) (princ _bagPath s)
     ;; string _srcDir
       (write-long (length _srcDir) s) (princ _srcDir s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int32 _mode
     (setq _mode (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; int32 _state
     (setq _state (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; int32 _frontEndMethod
     (setq _frontEndMethod (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; float32 _currFrameLeafSize
     (setq _currFrameLeafSize (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _localMapLeafSize
     (setq _localMapLeafSize (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; int32 _localMapWindowSize
     (setq _localMapWindowSize (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; float32 _loopDetectDist
     (setq _loopDetectDist (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; string _cfgDir
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _cfgDir (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; string _dstDir
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _dstDir (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; string _bagPath
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _bagPath (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; string _srcDir
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _srcDir (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;;
   self)
  )

(defclass comm_srvs::MappingStartResponse
  :super ros::object
  :slots (_success _message ))

(defmethod comm_srvs::MappingStartResponse
  (:init
   (&key
    ((:success __success) nil)
    ((:message __message) "")
    )
   (send-super :init)
   (setq _success __success)
   (setq _message (string __message))
   self)
  (:success
   (&optional (__success :null))
   (if (not (eq __success :null)) (setq _success __success)) _success)
  (:message
   (&optional __message)
   (if __message (setq _message __message)) _message)
  (:serialization-length
   ()
   (+
    ;; bool _success
    1
    ;; string _message
    4 (length _message)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; bool _success
       (if _success (write-byte -1 s) (write-byte 0 s))
     ;; string _message
       (write-long (length _message) s) (princ _message s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; bool _success
     (setq _success (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;; string _message
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _message (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;;
   self)
  )

(defclass comm_srvs::MappingStart
  :super ros::object
  :slots ())

(setf (get comm_srvs::MappingStart :md5sum-) "50d318bcef824d9f76f338ae7c891086")
(setf (get comm_srvs::MappingStart :datatype-) "comm_srvs/MappingStart")
(setf (get comm_srvs::MappingStart :request) comm_srvs::MappingStartRequest)
(setf (get comm_srvs::MappingStart :response) comm_srvs::MappingStartResponse)

(defmethod comm_srvs::MappingStartRequest
  (:response () (instance comm_srvs::MappingStartResponse :init)))

(setf (get comm_srvs::MappingStartRequest :md5sum-) "50d318bcef824d9f76f338ae7c891086")
(setf (get comm_srvs::MappingStartRequest :datatype-) "comm_srvs/MappingStartRequest")
(setf (get comm_srvs::MappingStartRequest :definition-)
      "int32   mode
int32   state
int32   frontEndMethod
float32 currFrameLeafSize
float32 localMapLeafSize  
int32   localMapWindowSize 
float32 loopDetectDist 
string  cfgDir
string  dstDir
string  bagPath
string  srcDir
---
bool   success
string message
")

(setf (get comm_srvs::MappingStartResponse :md5sum-) "50d318bcef824d9f76f338ae7c891086")
(setf (get comm_srvs::MappingStartResponse :datatype-) "comm_srvs/MappingStartResponse")
(setf (get comm_srvs::MappingStartResponse :definition-)
      "int32   mode
int32   state
int32   frontEndMethod
float32 currFrameLeafSize
float32 localMapLeafSize  
int32   localMapWindowSize 
float32 loopDetectDist 
string  cfgDir
string  dstDir
string  bagPath
string  srcDir
---
bool   success
string message
")



(provide :comm_srvs/MappingStart "50d318bcef824d9f76f338ae7c891086")


