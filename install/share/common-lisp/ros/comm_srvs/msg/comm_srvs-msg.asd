
(cl:in-package :asdf)

(defsystem "comm_srvs-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "LidarInstallParam" :depends-on ("_package_LidarInstallParam"))
    (:file "_package_LidarInstallParam" :depends-on ("_package"))
    (:file "cameraParam" :depends-on ("_package_cameraParam"))
    (:file "_package_cameraParam" :depends-on ("_package"))
    (:file "sensorInstallParam" :depends-on ("_package_sensorInstallParam"))
    (:file "_package_sensorInstallParam" :depends-on ("_package"))
    (:file "visionFuncCfg" :depends-on ("_package_visionFuncCfg"))
    (:file "_package_visionFuncCfg" :depends-on ("_package"))
    (:file "visionLabelCfg" :depends-on ("_package_visionLabelCfg"))
    (:file "_package_visionLabelCfg" :depends-on ("_package"))
  ))