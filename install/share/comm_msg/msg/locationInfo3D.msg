# location status, refer to MAP_NODE_STATE_ENUM
Header header
int32 status

# sub map no, this is valid when >= 0, if -ing status, this is the new sub map no
int32 subMapNo

# Comb Map pose, which is valid When status is > 0.
float32 posX
float32 posY
float32 posZ
float32 roll
float32 pitch
float32 yaw