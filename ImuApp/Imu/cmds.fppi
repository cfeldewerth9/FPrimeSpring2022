#-----
# commands
#-----

@ Clear the event throttle
    async command CLEAR_EVENT_THROTTLE \
    opcode 0

@ Sends I2c data, prints read data
    async command IMU_SEND_I2C(
        data: string size 40 @< data to send
        ) \
    opcode 1