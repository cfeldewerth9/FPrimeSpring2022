module ImuApp {

  @ Component to check, read, and return IMU values
  active component Imu {

    include "cmds.fppi"
    include "events.fppi"
    include "tlm.fppi"

    @ command receive port
    command recv port cmdIn

    @ command registration port
    command reg port cmdRegOut

    @ command response port
    command resp port cmdResponseOut

    @ event port
    event port eventOut

    @ text event port
    text event port textEventOut

    @ time get port
    time get port timeGetOut

    @ telemetry port
    telemetry port tlmOut

    @ Output Port for reading/writing I2c
    guarded input port writeRead: Drv.I2cWriteRead

    @ guarded input port for writing I2c
    guarded input port write: Drv.I2c

    @ guarded input port for reading I2c
    guarded input port read: Drv.I2c
  }

}