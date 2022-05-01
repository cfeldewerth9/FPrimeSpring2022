module Ref {

  @ Component to check, read, and return IMU values
  active component IMU {

    # ----------------------------------------------------------------------
    # General ports
    # ----------------------------------------------------------------------

    @ The rate group scheduler input
    sync input port run: Svc.Sched

    @ Output Port for reading/writing I2C
    guarded input port writeRead: Drv.I2cWriteRead

    @ guarded input port for writing I2c
    guarded input port write: Drv.I2c

    @ guarded input port for reading I2c
    guarded input port read: Drv.I2c

    # ----------------------------------------------------------------------
    # Special ports
    # ----------------------------------------------------------------------

    @ command receive port
    command recv port cmdIn

    @ command registration port
    command reg port cmdRegOut

    @ command reponse port
    command resp port cmdResponseOut

    @ event port
    event port eventOut

    @ text event port
    text event port textEventOut

    @ telemetry port
    telemetry port tlmOut

    @ serial buffer port
    output port serialBufferOut: Fw.BufferSend

    @ Time get port
    time get port timeGetOut

    # ----------------------------------------------------------------------
    # Events
    # ----------------------------------------------------------------------

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

    # ----------------------------------------------------------------------
    # Commands
    # ----------------------------------------------------------------------

    @ Clear the event throttle
    async command CLEAR_EVENT_THROTTLE \
      opcode 0

    @ Sends I2c data, prints read data
    async command IMU_SEND_I2C(
                              data: string size 40 @< data to send
                            ) \
      opcode 1
    
    # ----------------------------------------------------------------------
    # Telemetry
    # ----------------------------------------------------------------------

    @ current acceleration in x, y, and z directions (in g's)
    telemetry ACC_X: F32 id 0
    telemetry ACC_Y: F32 id 1
    telemetry ACC_Z: F32 id 2

    @ current angular speed around x, y, and z axes (in degrees per second)
    telemetry GYRO_X: F32 id 3
    telemetry GYRO_Y: F32 id 4
    telemetry GYRO_Z: F32 id 5

    @ current magnetic field around x, y, and z axes (in micro Teslas)
    telemetry MAG_X: F32 id 6
    telemetry MAG_Y: F32 id 7
    telemetry MAG_Z: F32 id 8

    }

}