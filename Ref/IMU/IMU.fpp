module Ref {

  @ Component to check, read, and return IMU values
  active component IMU {

    # ----------------------------------------------------------------------
    # General ports
    # ----------------------------------------------------------------------

    @ The rate group scheduler input
    sync input port schedIn: Svc.Sched

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

    @ Output Port for reading/writing I2C
    output port I2cReadWrite: Drv.I2cWriteRead

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
      id 2 \
      format "Event throttle cleared"

    # ----------------------------------------------------------------------
    # Commands
    # ----------------------------------------------------------------------

    @ Clear the event throttle
    async command CLEAR_EVENT_THROTTLE \
      opcode 0
    
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