# location status, refer to MAP_NODE_STATE_ENUM
int32 status

# sub map no, this is valid when >= 0, if -ing status, this is the new sub map no
int32 subMapNo

# Comb Map pose, which is valid When status is > 0.
float32 posX
float32 posY
float32 dirAngle