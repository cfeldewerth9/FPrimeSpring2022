#-----
# events
#-----

@ Event throttle cleared
    event THROTTLE_CLEARED \
    severity activity high \
    id 0 \
    format "Event throttle cleared"

@ Message received on I2C
    event IMU_I2C_MSGIN(
        msg: string size 40 @< The message bytes as text
        ) \
    severity activity high \
    id 1 \
    format "Received msg {} on I2C"