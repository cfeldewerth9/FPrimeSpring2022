// ======================================================================
// \title  IMU.hpp
// \author cfeldewerth
// \brief  hpp file for IMU component implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================

#ifndef IMU_HPP
#define IMU_HPP

#include "Ref/IMU/IMUComponentAc.hpp"

namespace Ref {

  class IMU :
    public IMUComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object IMU
      //!
      IMU(
          const char *const compName /*!< The component name*/
      );

      //! Initialize object IMU
      //!
      void init(
          const NATIVE_INT_TYPE queueDepth, /*!< The queue depth*/
          const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
      );

      //! Destroy object IMU
      //!
      ~IMU();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for read
      //!
      Drv::I2cStatus read_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          U32 addr, /*!< 
      I2C slave device address
      */
          Fw::Buffer &serBuffer /*!< 
      Buffer with data to read/write to/from
      */
      );

      //! Handler implementation for run
      //!
      void run_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          NATIVE_UINT_TYPE context /*!< 
      The call order
      */
      );

      //! Handler implementation for write
      //!
      Drv::I2cStatus write_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          U32 addr, /*!< 
      I2C slave device address
      */
          Fw::Buffer &serBuffer /*!< 
      Buffer with data to read/write to/from
      */
      );

      //! Handler implementation for writeRead
      //!
      Drv::I2cStatus writeRead_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          U32 addr, /*!< 
      I2C slave device address
      */
          Fw::Buffer &writeBuffer, /*!< 
      Buffer to write data to the i2c device
      */
          Fw::Buffer &readBuffer /*!< 
      Buffer to read back data from the i2c device, must set size when passing in read buffer
      */
      );

    PRIVATE:

      // ----------------------------------------------------------------------
      // Command handler implementations
      // ----------------------------------------------------------------------

      //! Implementation for CLEAR_EVENT_THROTTLE command handler
      //! Clear the event throttle
      void CLEAR_EVENT_THROTTLE_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq /*!< The command sequence number*/
      );

      //! Implementation for IMU_SEND_I2C command handler
      //! Sends I2c data, prints read data
      void IMU_SEND_I2C_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq, /*!< The command sequence number*/
          const Fw::CmdStringArg& data /*!< 
          data to send
          */
      );


    };

} // end namespace Ref

#endif

// -------------------------------------------------------------------------------------------

// line 457 @ https://github.com/adafruit/Adafruit_LSM6DS/blob/master/Adafruit_LSM6DS.cpp#L457

// void Adafruit_LSM6DS::_read(void) {
//   // get raw readings
//   Adafruit_BusIO_Register data_reg = Adafruit_BusIO_Register(
//       i2c_dev, spi_dev, ADDRBIT8_HIGH_TOREAD, LSM6DS_OUT_TEMP_L, 14);

//   uint8_t buffer[14];
//   data_reg.read(buffer, 14);

//   rawTemp = buffer[1] << 8 | buffer[0];
//   temperature = (rawTemp / temperature_sensitivity) + 25.0;

//   rawAccX = buffer[9] << 8 | buffer[8];
//   rawAccY = buffer[11] << 8 | buffer[10];
//   rawAccZ = buffer[13] << 8 | buffer[12];

//   float accel_scale = 1; // range is in milli-g per bit!
//   switch (accelRangeBuffered) {
//   case LSM6DS_ACCEL_RANGE_16_G:
//     accel_scale = 0.488;
//     break;
//   case LSM6DS_ACCEL_RANGE_8_G:
//     accel_scale = 0.244;
//     break;
//   case LSM6DS_ACCEL_RANGE_4_G:
//     accel_scale = 0.122;
//     break;
//   case LSM6DS_ACCEL_RANGE_2_G:
//     accel_scale = 0.061;
//     break;
//   }

//   accX = rawAccX * accel_scale * SENSORS_GRAVITY_STANDARD / 1000;
//   accY = rawAccY * accel_scale * SENSORS_GRAVITY_STANDARD / 1000;
//   accZ = rawAccZ * accel_scale * SENSORS_GRAVITY_STANDARD / 1000;
// }

// -------------------------------------------------------------------------------------------

// line 833 @ https://github.com/adafruit/Adafruit_LSM6DS/blob/master/Adafruit_LSM6DS.cpp#L457

// int Adafruit_LSM6DS::readAcceleration(float &x, float &y, float &z) {
//   int16_t data[3];

//   Adafruit_BusIO_Register accel_data = Adafruit_BusIO_Register(
//       i2c_dev, spi_dev, ADDRBIT8_HIGH_TOREAD, LSM6DS_OUTX_L_A, 6);

//   if (!accel_data.read((uint8_t *)data, sizeof(data))) {
//     x = y = z = NAN;
//     return 0;
//   }

//   // scale to range of -4 – 4
//   x = data[0] * 4.0 / 32768.0;
//   y = data[1] * 4.0 / 32768.0;
//   z = data[2] * 4.0 / 32768.0;

//   return 1;
// }