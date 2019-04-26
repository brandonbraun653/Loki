/********************************************************************************
 *   File Name:
 *    spi_intf.hpp
 *
 *   Description:
 *    Models the Chimera SPI interface 
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once 
#ifndef CHIMERA_SPI_INTERFACE_HPP
#define CHIMERA_SPI_INTERFACE_HPP

/* C++ Includes */
#include <cstdint>

/* Chimera Includes */
#include <Chimera/types/common_types.hpp>
#include <Chimera/types/spi_types.hpp>
#include <Chimera/threading.hpp>

namespace Chimera
{
  namespace SPI
  {
    class Interface : public Threading::Lockable
    {
    public:
      virtual ~Interface() = default;

      /**
       *  Initializes the SPI hardware according to the setup struct
       *
       *  @param[in]  setupStruct     Contains information on how to initialize SPI
       *  @return Chimera::Status_t
       *
       *  |   Return Value   |                        Explanation                        |
       *  |:----------------:|:---------------------------------------------------------:|
       *  |               OK | The operation completed successfully                      |
       *  |             FAIL | The operation failed                                      |
       *  | INVAL_FUNC_PARAM | One or more of the initialization parameters were invalid |
       */
      virtual Chimera::Status_t init( const Chimera::SPI::Setup &setupStruct ) = 0;

      /**
       *	Destroys all previous hardware setup (virtually or physically), which requires
       *	re-initialization of the object in order to be used again.
       *
       *	@return Chimera::Status_t
       *
       *	| Return Value |              Explanation             |
       *  |:------------:|:------------------------------------:|
       *  |           OK | The operation completed successfully |
       *  |         FAIL | The operation failed                 |
       */
      virtual Chimera::Status_t deInit() = 0;

      /**
       *  Sets the chip select GPIO to a logical state
       *
       *  @param[in]  value           The state to set the chip select to
       *  @return Chimera::Status_t
       *
       *  |   Return Value  |                  Explanation                 |
       *  |:---------------:|:--------------------------------------------:|
       *  |              OK | The operation completed successfully         |
       *  |            FAIL | The operation failed                         |
       *  |   NOT_SUPPORTED | This behavior is not supported on the driver |
       *  | NOT_INITIALIZED | The class object has not been initialized    |
       */
      virtual Chimera::Status_t setChipSelect( const Chimera::GPIO::State &value ) = 0;

      /**
       *  Instruct the chip select to behave in a specific manner
       *
       *  @param[in]  mode            The desired mode for the chip select to operate in
       *  @return Chimera::Status_t
       *
       *  |   Return Value  |                  Explanation                 |
       *  |:---------------:|:--------------------------------------------:|
       *  |              OK | The operation completed successfully         |
       *  | NOT_INITIALIZED | The class object has not been initialized    |
       */
      virtual Chimera::Status_t setChipSelectControlMode( const Chimera::SPI::ChipSelectMode &mode ) = 0;

      /**
       *  Writes data onto the SPI bus. The number of bytes actually written will be returned
       *  via onWriteCompleteCallback().
       *
       *  @param[in]  txBuffer        Data buffer to be sent
       *  @param[in]  length          Number of bytes to be sent (should not be larger than txBuffer)
       *  @param[in]  timeoutMS       How long to wait for SPI hardware to become available
       *  @return Chimera::Status_t
       *
       *  |   Return Value  |                  Explanation                 |
       *  |:---------------:|:--------------------------------------------:|
       *  |              OK | The operation completed successfully         |
       *  |            FAIL | The operation failed                         |
       *  | NOT_INITIALIZED | The class object has not been initialized    |
       */
      virtual Chimera::Status_t writeBytes( const uint8_t *const txBuffer, const size_t length, const uint32_t timeoutMS ) = 0;

      /**
       *  Reads data from the SPI bus. The number of bytes actually read will be returned
       *  via onReadCompleteCallback().
       *
       *  @param[out] rxBuffer        Data buffer to read into
       *  @param[in]  length          Number of bytes to read (must not be larger than rxBuffer size)
       *  @param[in]  timeoutMS       How long to wait for SPI hardware to become available
       *  @return Chimera::Status_t
       *
       *  |   Return Value  |                  Explanation                 |
       *  |:---------------:|:--------------------------------------------:|
       *  |              OK | The operation completed successfully         |
       *  |            FAIL | The operation failed                         |
       *  | NOT_INITIALIZED | The class object has not been initialized    |
       */
      virtual Chimera::Status_t readBytes( uint8_t *const rxBuffer, const size_t length, const uint32_t timeoutMS ) = 0;

      /**
       *  Transmits and receives data on the SPI bus in a single operation. Returns the actual
       *  number of bytes transmitted/received via onReadWriteCompleteCallback().
       *
       *  @param[in]  txBuffer        Data buffer to write from
       *  @param[out] rxBuffer        Data buffer to read into
       *  @param[in]  length          Number of bytes to transfer (must not be larger than rxBuffer size)
       *  @param[in]  timeoutMS       How long to wait for SPI hardware to become available
       *  @return Chimera::Status_t
       *
       *  |   Return Value  |                  Explanation                 |
       *  |:---------------:|:--------------------------------------------:|
       *  |              OK | The operation completed successfully         |
       *  |            FAIL | The operation failed                         |
       *  | NOT_INITIALIZED | The class object has not been initialized    |
       */
      virtual Chimera::Status_t readWriteBytes( const uint8_t *const txBuffer, uint8_t *const rxBuffer, const size_t length,
                                                const uint32_t timeoutMS ) = 0;

      /**
       *  Set the hardware operational mode in either Blocking, Interrupt, or DMA.
       *
       *  @param[in]  periph          The peripheral to set the behavior on
       *  @param[in]  mode            Desired operational mode of the peripheral
       *  @return Chimera::Status_t
       *
       *  |   Return Value  |                  Explanation                 |
       *  |:---------------:|:--------------------------------------------:|
       *  |              OK | The operation completed successfully         |
       *  |            FAIL | The operation failed                         |
       *  | NOT_INITIALIZED | The class object has not been initialized    |
       */
      virtual Chimera::Status_t setPeripheralMode( const Chimera::SPI::SubPeripheral periph,
                                                   const Chimera::SPI::SubPeripheralMode mode ) = 0;

      /**
       *  Change the frequency of the SPI output clock
       *
       *  Should work at runtime after the SPI hardware has been configured. If the
       *  exact clock frequency cannot be met, the next lowest value will be selected
       *  up to the hardware limits.
       *
       *  For example, if a particular device supports 1MHz, 2MHz, 4MHz and 8MHz clock rates
       *  and the user requests a clock of 7.5MHz with 0% tolerance, the hardware will be
       *  initialized to 4MHz and return a status of Chimera::SPI::Status::CLOCK_SET_LT.
       *
       *  @param[in]  freq            Desired SPI clock frequency in Hz
       *  @param[in]  tolerance       Percent tolerance allowed: 0 for exact, 100 for no care
       *  @return Chimera::Status_t
       *
       *  |   Return Value  |                              Explanation                             |
       *  |:---------------:|:--------------------------------------------------------------------:|
       *  |              OK | The operation completed successfully (CLOCK_SET_EQ)                  |
       *  |            FAIL | The operation failed                                                 |
       *  |   NOT_SUPPORTED | This behavior is not supported on the driver                         |
       *  | NOT_INITIALIZED | The class object has not been initialized                            |
       *  |    CLOCK_SET_EQ | The desired clock value was achieved exactly or within tolerance     |
       *  |    CLOCK_SET_LT | The actual clock value achieved was less than the user desired value |
       */
      virtual Chimera::Status_t setClockFrequency( const uint32_t freq, const uint32_t tolerance ) = 0;

      /**
       *  Get the current SPI clock frequency.
       *
       *  If the class has not been initialized, the output variable should not be modified.
       *
       *  @param[out] freq            Reported SPI clock
       *  @return Chimera::Status_t
       *
       *  |   Return Value  |                Explanation                |
       *  |:---------------:|:-----------------------------------------:|
       *  |              OK | The operation completed successfully      |
       *  |            FAIL | The operation failed                      |
       *  | NOT_INITIALIZED | The class object has not been initialized |
       */
      virtual Chimera::Status_t getClockFrequency( uint32_t &freq ) = 0;

      /**
       *  Allows the user to assign a callback function to the write complete event.
       *  The callback function will be passed a parameter indicating how many bytes were written.
       *
       *  @param[in]  func            User callback function
       *  @return Chimera::Status_t
       *
       *  |  Return Value |                  Explanation                 |
       *  |:-------------:|:--------------------------------------------:|
       *  |            OK | The operation completed successfully         |
       *  |          FAIL | The operation failed                         |
       *  | NOT_SUPPORTED | This behavior is not supported on the driver |
       */
      virtual Chimera::Status_t onWriteCompleteCallback( const Chimera::Function::void_func_uint32_t func )
      {
        return Chimera::SPI::Status::NOT_SUPPORTED;
      }

      /**
       *  Allows the user to assign a callback function to the read complete event.
       *  The callback function will be passed a parameter indicating how many bytes were read.
       *
       *  @param[in]  func            User callback function
       *  @return Chimera::Status_t
       *
       *  |  Return Value |                  Explanation                 |
       *  |:-------------:|:--------------------------------------------:|
       *  |            OK | The operation completed successfully         |
       *  |          FAIL | The operation failed                         |
       *  | NOT_SUPPORTED | This behavior is not supported on the driver |
       */
      virtual Chimera::Status_t onReadCompleteCallback( const Chimera::Function::void_func_uint32_t func )
      {
        return Chimera::SPI::Status::NOT_SUPPORTED;
      }

      /**
       *  Allows the user to assign a callback function to the read-write complete event.
       *  The callback function will be passed a parameter indicating how many bytes were read/written.
       *
       *  @param[in]  func            User callback function
       *  @return Chimera::Status_t
       *
       *  |  Return Value |                  Explanation                 |
       *  |:-------------:|:--------------------------------------------:|
       *  |            OK | The operation completed successfully         |
       *  |          FAIL | The operation failed                         |
       *  | NOT_SUPPORTED | This behavior is not supported on the driver |
       */
      virtual Chimera::Status_t onReadWriteCompleteCallback( const Chimera::Function::void_func_uint32_t func )
      {
        return Chimera::SPI::Status::NOT_SUPPORTED;
      }

      /**
       *  Allows the user to assign a callback function on an error event
       *
       *  The function will be passed an error code indicating what happened
       *
       *  @param[in]  func            User callback function
       *  @return Chimera::Status_t
       *
       *  |  Return Value |                  Explanation                 |
       *  |:-------------:|:--------------------------------------------:|
       *  |            OK | The operation completed successfully         |
       *  |          FAIL | The operation failed                         |
       *  | NOT_SUPPORTED | This behavior is not supported on the driver |
       */
      virtual Chimera::Status_t onErrorCallback( const Chimera::Function::void_func_uint32_t func )
      {
        return Chimera::SPI::Status::NOT_SUPPORTED;
      }
    };

    class SPIUnsupported : public Interface
    {
    public:
      SPIUnsupported()  = default;
      ~SPIUnsupported() = default;

      Chimera::Status_t init( const Chimera::SPI::Setup &setupStruct ) final override
      {
        return Chimera::SPI::Status::FAIL;
      }

      Chimera::Status_t deInit() final override
      {
        return Chimera::SPI::Status::FAIL;
      }

      Chimera::Status_t setChipSelect( const Chimera::GPIO::State &value ) final override
      {
        return Chimera::SPI::Status::NOT_SUPPORTED;
      }

      Chimera::Status_t setChipSelectControlMode( const Chimera::SPI::ChipSelectMode &mode ) final override
      {
        return Chimera::SPI::Status::NOT_INITIALIZED;
      }

      Chimera::Status_t writeBytes( const uint8_t *const txBuffer, size_t length, uint32_t timeoutMS ) final override
      {
        return Chimera::SPI::Status::FAIL;
      }

      Chimera::Status_t readBytes( uint8_t *const rxBuffer, size_t length, uint32_t timeoutMS ) final override
      {
        return Chimera::SPI::Status::FAIL;
      }

      Chimera::Status_t readWriteBytes( const uint8_t *const txBuffer, uint8_t *const rxBuffer, size_t length,
                                        uint32_t timeoutMS ) final override
      {
        return Chimera::SPI::Status::FAIL;
      }

      Chimera::Status_t setPeripheralMode( const Chimera::SPI::SubPeripheral periph,
                                           const Chimera::SPI::SubPeripheralMode mode ) final override
      {
        return Chimera::SPI::Status::FAIL;
      }

      Chimera::Status_t setClockFrequency( const uint32_t freq, const uint32_t tolerance ) final override
      {
        return Chimera::SPI::Status::FAIL;
      }

      Chimera::Status_t getClockFrequency( uint32_t &freq ) final override
      {
        return Chimera::SPI::Status::FAIL;
      }
    };

  }  // namespace SPI
}  // namespace Chimera

#endif /* !CHIMERA_SPI_INTERFACE_HPP */