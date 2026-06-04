uint8                   status

# 0是二维码，1是二维码加反光条
uint8                   equipmentType

uint32                  QRCodeId
pose                    QRCodePose
coor3D                  QRCodeOffset
geometry_msgs/Point32[] reflectiveStripPosLeft
geometry_msgs/Point32[] reflectiveStripPosRight