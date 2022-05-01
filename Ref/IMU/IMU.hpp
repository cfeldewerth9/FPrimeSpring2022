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
