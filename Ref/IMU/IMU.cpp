// ======================================================================
// \title  IMU.cpp
// \author cfeldewerth
// \brief  cpp file for IMU component implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================


#include <Ref/IMU/IMU.hpp>
#include "Fw/Types/BasicTypes.hpp"

namespace Ref {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  IMU ::
    IMU(
        const char *const compName
    ) : IMUComponentBase(compName)
  {

  }

  void IMU ::
    init(
        const NATIVE_INT_TYPE queueDepth,
        const NATIVE_INT_TYPE instance
    )
  {
    IMUComponentBase::init(queueDepth, instance);
  }

  IMU ::
    ~IMU()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  Drv::I2cStatus IMU ::
    read_handler(
        const NATIVE_INT_TYPE portNum,
        U32 addr,
        Fw::Buffer &serBuffer
    )
  {
    // TODO return
  }

  void IMU ::
    run_handler(
        const NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    // TODO
  }

  Drv::I2cStatus IMU ::
    write_handler(
        const NATIVE_INT_TYPE portNum,
        U32 addr,
        Fw::Buffer &serBuffer
    )
  {
    // TODO return
  }

  Drv::I2cStatus IMU ::
    writeRead_handler(
        const NATIVE_INT_TYPE portNum,
        U32 addr,
        Fw::Buffer &writeBuffer,
        Fw::Buffer &readBuffer
    )
  {
    // TODO return
  }

  // ----------------------------------------------------------------------
  // Command handler implementations
  // ----------------------------------------------------------------------

  void IMU ::
    CLEAR_EVENT_THROTTLE_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq
    )
  {
    // TODO
    this->cmdResponse_out(opCode,cmdSeq,Fw::CmdResponse::OK);
  }

  void IMU ::
    IMU_SEND_I2C_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        const Fw::CmdStringArg& data
    )
  {
    // TODO
    this->cmdResponse_out(opCode,cmdSeq,Fw::CmdResponse::OK);
  }

} // end namespace Ref
