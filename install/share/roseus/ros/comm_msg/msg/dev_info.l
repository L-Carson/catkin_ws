;; Auto-generated. Do not edit!


(when (boundp 'comm_msg::dev_info)
  (if (not (find-package "COMM_MSG"))
    (make-package "COMM_MSG"))
  (shadow 'dev_info (find-package "COMM_MSG")))
(unless (find-package "COMM_MSG::DEV_INFO")
  (make-package "COMM_MSG::DEV_INFO"))

(in-package "ROS")
;;//! \htmlinclude dev_info.msg.html


(defclass comm_msg::dev_info
  :super ros::object
  :slots (_chargerMotorState _chargerMotorTravel _liftMotorState _rollMotorState _leftLiftMotorTravel _rightLiftMotorTravel _leftRollMotorTravel _rightRollMotorTravel _angleSensorOne _angleSensorTwo _isStationEmergeStop _isManualChargeMode _proximitySw _chargeSw _waterSw _stationLockSw _chargerTravelSw _leftTravelSw _rightTravelSw _trashBinCoverOpenAngle ))

(defmethod comm_msg::dev_info
  (:init
   (&key
    ((:chargerMotorState __chargerMotorState) 0)
    ((:chargerMotorTravel __chargerMotorTravel) 0)
    ((:liftMotorState __liftMotorState) 0)
    ((:rollMotorState __rollMotorState) 0)
    ((:leftLiftMotorTravel __leftLiftMotorTravel) 0)
    ((:rightLiftMotorTravel __rightLiftMotorTravel) 0)
    ((:leftRollMotorTravel __leftRollMotorTravel) 0)
    ((:rightRollMotorTravel __rightRollMotorTravel) 0)
    ((:angleSensorOne __angleSensorOne) 0.0)
    ((:angleSensorTwo __angleSensorTwo) 0.0)
    ((:isStationEmergeStop __isStationEmergeStop) 0)
    ((:isManualChargeMode __isManualChargeMode) 0)
    ((:proximitySw __proximitySw) 0)
    ((:chargeSw __chargeSw) 0)
    ((:waterSw __waterSw) 0)
    ((:stationLockSw __stationLockSw) 0)
    ((:chargerTravelSw __chargerTravelSw) 0)
    ((:leftTravelSw __leftTravelSw) 0)
    ((:rightTravelSw __rightTravelSw) 0)
    ((:trashBinCoverOpenAngle __trashBinCoverOpenAngle) 0.0)
    )
   (send-super :init)
   (setq _chargerMotorState (round __chargerMotorState))
   (setq _chargerMotorTravel (round __chargerMotorTravel))
   (setq _liftMotorState (round __liftMotorState))
   (setq _rollMotorState (round __rollMotorState))
   (setq _leftLiftMotorTravel (round __leftLiftMotorTravel))
   (setq _rightLiftMotorTravel (round __rightLiftMotorTravel))
   (setq _leftRollMotorTravel (round __leftRollMotorTravel))
   (setq _rightRollMotorTravel (round __rightRollMotorTravel))
   (setq _angleSensorOne (float __angleSensorOne))
   (setq _angleSensorTwo (float __angleSensorTwo))
   (setq _isStationEmergeStop (round __isStationEmergeStop))
   (setq _isManualChargeMode (round __isManualChargeMode))
   (setq _proximitySw (round __proximitySw))
   (setq _chargeSw (round __chargeSw))
   (setq _waterSw (round __waterSw))
   (setq _stationLockSw (round __stationLockSw))
   (setq _chargerTravelSw (round __chargerTravelSw))
   (setq _leftTravelSw (round __leftTravelSw))
   (setq _rightTravelSw (round __rightTravelSw))
   (setq _trashBinCoverOpenAngle (float __trashBinCoverOpenAngle))
   self)
  (:chargerMotorState
   (&optional __chargerMotorState)
   (if __chargerMotorState (setq _chargerMotorState __chargerMotorState)) _chargerMotorState)
  (:chargerMotorTravel
   (&optional __chargerMotorTravel)
   (if __chargerMotorTravel (setq _chargerMotorTravel __chargerMotorTravel)) _chargerMotorTravel)
  (:liftMotorState
   (&optional __liftMotorState)
   (if __liftMotorState (setq _liftMotorState __liftMotorState)) _liftMotorState)
  (:rollMotorState
   (&optional __rollMotorState)
   (if __rollMotorState (setq _rollMotorState __rollMotorState)) _rollMotorState)
  (:leftLiftMotorTravel
   (&optional __leftLiftMotorTravel)
   (if __leftLiftMotorTravel (setq _leftLiftMotorTravel __leftLiftMotorTravel)) _leftLiftMotorTravel)
  (:rightLiftMotorTravel
   (&optional __rightLiftMotorTravel)
   (if __rightLiftMotorTravel (setq _rightLiftMotorTravel __rightLiftMotorTravel)) _rightLiftMotorTravel)
  (:leftRollMotorTravel
   (&optional __leftRollMotorTravel)
   (if __leftRollMotorTravel (setq _leftRollMotorTravel __leftRollMotorTravel)) _leftRollMotorTravel)
  (:rightRollMotorTravel
   (&optional __rightRollMotorTravel)
   (if __rightRollMotorTravel (setq _rightRollMotorTravel __rightRollMotorTravel)) _rightRollMotorTravel)
  (:angleSensorOne
   (&optional __angleSensorOne)
   (if __angleSensorOne (setq _angleSensorOne __angleSensorOne)) _angleSensorOne)
  (:angleSensorTwo
   (&optional __angleSensorTwo)
   (if __angleSensorTwo (setq _angleSensorTwo __angleSensorTwo)) _angleSensorTwo)
  (:isStationEmergeStop
   (&optional __isStationEmergeStop)
   (if __isStationEmergeStop (setq _isStationEmergeStop __isStationEmergeStop)) _isStationEmergeStop)
  (:isManualChargeMode
   (&optional __isManualChargeMode)
   (if __isManualChargeMode (setq _isManualChargeMode __isManualChargeMode)) _isManualChargeMode)
  (:proximitySw
   (&optional __proximitySw)
   (if __proximitySw (setq _proximitySw __proximitySw)) _proximitySw)
  (:chargeSw
   (&optional __chargeSw)
   (if __chargeSw (setq _chargeSw __chargeSw)) _chargeSw)
  (:waterSw
   (&optional __waterSw)
   (if __waterSw (setq _waterSw __waterSw)) _waterSw)
  (:stationLockSw
   (&optional __stationLockSw)
   (if __stationLockSw (setq _stationLockSw __stationLockSw)) _stationLockSw)
  (:chargerTravelSw
   (&optional __chargerTravelSw)
   (if __chargerTravelSw (setq _chargerTravelSw __chargerTravelSw)) _chargerTravelSw)
  (:leftTravelSw
   (&optional __leftTravelSw)
   (if __leftTravelSw (setq _leftTravelSw __leftTravelSw)) _leftTravelSw)
  (:rightTravelSw
   (&optional __rightTravelSw)
   (if __rightTravelSw (setq _rightTravelSw __rightTravelSw)) _rightTravelSw)
  (:trashBinCoverOpenAngle
   (&optional __trashBinCoverOpenAngle)
   (if __trashBinCoverOpenAngle (setq _trashBinCoverOpenAngle __trashBinCoverOpenAngle)) _trashBinCoverOpenAngle)
  (:serialization-length
   ()
   (+
    ;; uint8 _chargerMotorState
    1
    ;; uint32 _chargerMotorTravel
    4
    ;; uint8 _liftMotorState
    1
    ;; uint8 _rollMotorState
    1
    ;; uint32 _leftLiftMotorTravel
    4
    ;; uint32 _rightLiftMotorTravel
    4
    ;; uint32 _leftRollMotorTravel
    4
    ;; uint32 _rightRollMotorTravel
    4
    ;; float32 _angleSensorOne
    4
    ;; float32 _angleSensorTwo
    4
    ;; uint8 _isStationEmergeStop
    1
    ;; uint8 _isManualChargeMode
    1
    ;; uint8 _proximitySw
    1
    ;; uint8 _chargeSw
    1
    ;; uint8 _waterSw
    1
    ;; uint8 _stationLockSw
    1
    ;; uint8 _chargerTravelSw
    1
    ;; uint8 _leftTravelSw
    1
    ;; uint8 _rightTravelSw
    1
    ;; float32 _trashBinCoverOpenAngle
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint8 _chargerMotorState
       (write-byte _chargerMotorState s)
     ;; uint32 _chargerMotorTravel
       (write-long _chargerMotorTravel s)
     ;; uint8 _liftMotorState
       (write-byte _liftMotorState s)
     ;; uint8 _rollMotorState
       (write-byte _rollMotorState s)
     ;; uint32 _leftLiftMotorTravel
       (write-long _leftLiftMotorTravel s)
     ;; uint32 _rightLiftMotorTravel
       (write-long _rightLiftMotorTravel s)
     ;; uint32 _leftRollMotorTravel
       (write-long _leftRollMotorTravel s)
     ;; uint32 _rightRollMotorTravel
       (write-long _rightRollMotorTravel s)
     ;; float32 _angleSensorOne
       (sys::poke _angleSensorOne (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _angleSensorTwo
       (sys::poke _angleSensorTwo (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; uint8 _isStationEmergeStop
       (write-byte _isStationEmergeStop s)
     ;; uint8 _isManualChargeMode
       (write-byte _isManualChargeMode s)
     ;; uint8 _proximitySw
       (write-byte _proximitySw s)
     ;; uint8 _chargeSw
       (write-byte _chargeSw s)
     ;; uint8 _waterSw
       (write-byte _waterSw s)
     ;; uint8 _stationLockSw
       (write-byte _stationLockSw s)
     ;; uint8 _chargerTravelSw
       (write-byte _chargerTravelSw s)
     ;; uint8 _leftTravelSw
       (write-byte _leftTravelSw s)
     ;; uint8 _rightTravelSw
       (write-byte _rightTravelSw s)
     ;; float32 _trashBinCoverOpenAngle
       (sys::poke _trashBinCoverOpenAngle (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint8 _chargerMotorState
     (setq _chargerMotorState (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint32 _chargerMotorTravel
     (setq _chargerMotorTravel (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; uint8 _liftMotorState
     (setq _liftMotorState (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _rollMotorState
     (setq _rollMotorState (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint32 _leftLiftMotorTravel
     (setq _leftLiftMotorTravel (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; uint32 _rightLiftMotorTravel
     (setq _rightLiftMotorTravel (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; uint32 _leftRollMotorTravel
     (setq _leftRollMotorTravel (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; uint32 _rightRollMotorTravel
     (setq _rightRollMotorTravel (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; float32 _angleSensorOne
     (setq _angleSensorOne (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _angleSensorTwo
     (setq _angleSensorTwo (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; uint8 _isStationEmergeStop
     (setq _isStationEmergeStop (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _isManualChargeMode
     (setq _isManualChargeMode (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _proximitySw
     (setq _proximitySw (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _chargeSw
     (setq _chargeSw (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _waterSw
     (setq _waterSw (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _stationLockSw
     (setq _stationLockSw (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _chargerTravelSw
     (setq _chargerTravelSw (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _leftTravelSw
     (setq _leftTravelSw (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _rightTravelSw
     (setq _rightTravelSw (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; float32 _trashBinCoverOpenAngle
     (setq _trashBinCoverOpenAngle (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get comm_msg::dev_info :md5sum-) "17ff66948ac49c3e03faf3f4e7f62471")
(setf (get comm_msg::dev_info :datatype-) "comm_msg/dev_info")
(setf (get comm_msg::dev_info :definition-)
      "# charger state
uint8  chargerMotorState
uint32 chargerMotorTravel

# trash box state
uint8   liftMotorState
uint8   rollMotorState
uint32  leftLiftMotorTravel
uint32  rightLiftMotorTravel
uint32  leftRollMotorTravel
uint32  rightRollMotorTravel
float32 angleSensorOne
float32 angleSensorTwo

# dev state
uint8 isStationEmergeStop
uint8 isManualChargeMode
uint8 proximitySw
uint8 chargeSw
uint8 waterSw
uint8 stationLockSw
uint8 chargerTravelSw
uint8 leftTravelSw
uint8 rightTravelSw

# trash cover state
float32 trashBinCoverOpenAngle
")



(provide :comm_msg/dev_info "17ff66948ac49c3e03faf3f4e7f62471")


