#pragma once
#ifndef CHIMERA_TYPES_HPP
#define CHIMERA_TYPES_HPP

#include <stdint.h>

#include <Chimera/preprocessor.hpp>

/** @namespace Chimera */
namespace Chimera
{
  typedef void ( *void_func_void )( void );
  typedef void ( *void_func_void_ptr )( void * );
  typedef void ( *void_func_uint32_t )( uint32_t );

  using Status_t = uint16_t;

  class CommonStatusCodes
  {
  public:
    static constexpr Status_t UNKNOWN_ERROR      = 0;  /**< Don't know what went wrong, but need to report an error */
    static constexpr Status_t OK                 = 1;  /**< Everything is just fine. No errors. */
    static constexpr Status_t NOT_INITIALIZED    = 2;  /**< The system has not been initialized yet. */
    static constexpr Status_t LOCKED             = 3;  /**< The system has been locked */
    static constexpr Status_t EXCEPTION          = 4;  /**< An exception was thrown */
    static constexpr Status_t TIMEOUT            = 5;  /**< The system timed-out on an operation */
    static constexpr Status_t NOT_SUPPORTED      = 10; /**< Some system functionality is not enabled/supported */
    static constexpr Status_t NOT_READY          = 11; /**< The system is not ready to go yet */
    static constexpr Status_t TX_IN_PROGRESS     = 12; /**< A transmission is in progress */
    static constexpr Status_t RX_IN_PROGRESS     = 13; /**< A reception is in progress */
    static constexpr Status_t BUSY               = 14; /**< The system can't be bothered to respond right now. It's busy. */
    static constexpr Status_t INVAL_FUNC_PARAM = 15; /**< A parameter passed into a function was invalid */

    static constexpr Status_t FAIL           = 30; /**< Generic error to signify a non-explicit failure type */
    static constexpr Status_t FAILED_INIT    = 31; /**< Somehow failed an initialization sequence */
    static constexpr Status_t FAILED_LOCK    = 32; /**< Could not lock a resource */
    static constexpr Status_t FAILED_RELEASE = 33; /**< Could not release a locked resource */

    /*------------------------------------------------
    Offsets to identify particular Chimera modules 
    ------------------------------------------------*/
    static constexpr Status_t status_offset_module_memory_flash = 1000;

  };


  /** @namespace Chimera::GPIO */
  namespace GPIO
  {
    class Status : public CommonStatusCodes
    {
    public:
      static constexpr Status_t codeOffset = 100;

      // Add status codes as needed. Currently all CommonStatusCodes suffice.
    };

    enum class Drive : uint8_t
    {
      INPUT,
      OUTPUT_PUSH_PULL,
      OUTPUT_OPEN_DRAIN,
      ALTERNATE_PUSH_PULL,
      ALTERNATE_OPEN_DRAIN,
      ANALOG,
      HIZ,

      NUM_DRIVES,
      UNKNOWN_DRIVE
    };

    enum class State : bool
    {
      HIGH = true,
      HI   = true,
      ON   = true,
      LOW  = false,
      LO   = false,
      OFF  = false
    };

    enum class Pull : uint8_t
    {
      NO_PULL,
      PULL_UP,
      PULL_DN,

      NUM_PULL_OPTIONS,
      UNKNOWN_PULL
    };

    enum class Port : uint8_t
    {
      PORTA,
      PORTB,
      PORTC,
      PORTD,
      PORTE,
      PORTF,
      PORTG,
      PORTH,
      PORTI,
      PORTJ,
      PORTK,
      PORTL,

      NUM_PORTS,
      UNKNOWN_PORT
    };

    struct PinInit
    {
      Pull pull          = Pull::UNKNOWN_PULL; /**< Pull-up/down configuration */
      Port port          = Port::UNKNOWN_PORT; /**< Pin IO port */
      Drive mode         = Drive::INPUT;       /**< Pin IO drive type */
      State state        = State::LOW;         /**< Default logical pin state on init */
      uint16_t pin       = 0u;                 /**< Pin number on the given port */
      uint32_t alternate = 0u;                 /**< Project specific indicator of pin AF config*/
    };

  }  // namespace GPIO

  /** @namespace Chimera::SPI */
  namespace SPI
  {

    class Status : public CommonStatusCodes
    {
    public:
      static constexpr Status_t codeOffset = 200;

      static constexpr Status_t PACKET_TOO_LARGE_FOR_BUFFER = codeOffset + 1;
      static constexpr Status_t FAILED_CONVERSION           = codeOffset + 2;
      static constexpr Status_t INVALID_HARDWARE_PARAM      = codeOffset + 3;
      static constexpr Status_t FAILED_CHIP_SELECT_WRITE    = codeOffset + 4;
      static constexpr Status_t CLOCK_SET_LT                = codeOffset + 5;
      static constexpr Status_t CLOCK_SET_EQ                = OK;
    };

    enum class Mode : uint8_t
    {
      MASTER,
      SLAVE
    };

    enum class BitOrder : uint8_t
    {
      MSB_FIRST,
      LSB_FIRST
    };

    enum class ClockMode : uint8_t
    {
      MODE0, /**< CPOL=0, CPHA=0 */
      MODE1, /**< CPOL=0, CPHA=1 */
      MODE2, /**< CPOL=1, CPHA=0 */
      MODE3  /**< CPOL=1, CPHA=1 */
    };

    enum class DataSize : uint8_t
    {
      SZ_8BIT,
      SZ_9BIT,
      SZ_10BIT,
      SZ_11BIT,
      SZ_12BIT,
      SZ_13BIT,
      SZ_14BIT,
      SZ_15BIT,
      SZ_16BIT,
    };

    enum class SubPeripheral : uint8_t
    {
      TX,
      RX,
      TXRX
    };

    enum class SubPeripheralMode : uint8_t
    {
      UNKOWN_MODE,
      BLOCKING,
      INTERRUPT,
      DMA
    };

    enum class ChipSelectMode : uint8_t
    {
      MANUAL,                /**< Manually control the state of the chip select line */
      AUTO_BETWEEN_TRANSFER, /**< Twiddle the chip select between transfers, disabling on completion */
      AUTO_AFTER_TRANSFER    /**< Disable the chip select only after all transfers complete */
    };

    struct Setup
    {
      GPIO::PinInit SCK;  /**< The GPIO pin settings used for SCK */
      GPIO::PinInit MOSI; /**< The GPIO pin settings used for MOSI */
      GPIO::PinInit MISO; /**< The GPIO pin settings used for MISO */
      GPIO::PinInit CS;   /**< The GPIO pin settings used for CS */

      Mode mode               = Mode::MASTER;        /**< The primary control method for the peripheral */
      DataSize dataSize       = DataSize::SZ_8BIT;   /**< How wide each transfer should minimally be */
      BitOrder bitOrder       = BitOrder::MSB_FIRST; /**< Sets LSB or MSB ordering */
      ClockMode clockMode     = ClockMode::MODE0;    /**< Sets the clock phase and polarity options */
      uint32_t clockFrequency = 1000000;             /**< The desired approximate clock frequency */
    };
  }  // namespace SPI

  /** @namespace Chimera::Serial */
  namespace Serial
  {
    class Status : public CommonStatusCodes
    {
    public:
      static constexpr Status_t codeOffset = 300;

      static constexpr Status_t TX_IN_PROGRESS              = codeOffset + 3;
      static constexpr Status_t RX_IN_PROGRESS              = codeOffset + 4;
      static constexpr Status_t RX_COMPLETE                 = codeOffset + 5;
      static constexpr Status_t NOT_READY                   = codeOffset + 6;
      static constexpr Status_t PACKET_TOO_LARGE_FOR_BUFFER = codeOffset + 7;
      static constexpr Status_t FAILED_WRITE                = codeOffset + 11;
      static constexpr Status_t FAILED_READ                 = codeOffset + 12;
      static constexpr Status_t FAILED_OPEN                 = codeOffset + 13;
      static constexpr Status_t FAILED_CONFIGURE            = codeOffset + 14;
    };

    enum class BaudRate : uint32_t
    {
      SERIAL_BAUD_110    = 100u,
      SERIAL_BAUD_150    = 150u,
      SERIAL_BAUD_300    = 300u,
      SERIAL_BAUD_1200   = 1200u,
      SERIAL_BAUD_2400   = 2400u,
      SERIAL_BAUD_4800   = 4800u,
      SERIAL_BAUD_9600   = 9600u,
      SERIAL_BAUD_19200  = 19200u,
      SERIAL_BAUD_38400  = 38400u,
      SERIAL_BAUD_57600  = 57600u,
      SERIAL_BAUD_115200 = 115200u,
      SERIAL_BAUD_230400 = 230400u,
      SERIAL_BAUD_460800 = 460800u,
      SERIAL_BAUD_921600 = 921600u
    };

    enum class CharWid : uint8_t
    {
      CW_8BIT = 8
    };

    enum class Parity : uint8_t
    {
      PAR_NONE = 0,
      PAR_ODD,
      PAR_EVEN
    };

    enum class StopBits : uint8_t
    {
      SBITS_ONE = 0,
      SBITS_ONE_POINT_FIVE,
      SBITS_TWO
    };

    enum class FlowControl : uint8_t
    {
      FCTRL_NONE,
      FCTRL_SW,
      FCTRL_HW
    };

    enum class Modes : uint8_t
    {
      MODE_UNDEFINED,
      BLOCKING,
      INTERRUPT,
      DMA
    };

    enum class SubPeripheral : uint8_t
    {
      RX,
      TX
    };

    enum class Event : uint8_t
    {
      ASYNC_READ_COMPLETE = 0,
      WRITE_COMPLETE
    };

    typedef struct
    {
      bool overrun;
      bool error;
      bool asyncReady;
    } HardwareStatus;

  }  // namespace Serial

  namespace System
  {
    enum class Status : uint8_t
    {
      OK = 0,
      NOT_SUPPORTED,

      UNKNOWN_STATUS,
      MAX_STATUS
    };

    enum class ResetType : uint8_t
    {
      BROWN_OUT,
      SOFTWARE,
      HW_WATCHDOG_TIMEOUT,
      UNKNOWN_RESET,
      MAX_RESET_TYPE
    };
  }  // namespace System

  namespace Threading
  {
  }

  namespace Watchdog
  {
    enum class Status : uint8_t
    {
      OK,
      FAIL,

      NOT_SUPPORTED,
      UNKOWN_STATUS,
      MAX_STATUS
    };
  }

  /** @namespace Chimera::FreeRTOS */
  namespace FreeRTOS
  {
    enum class SPIEvent : uint8_t
    {
      RX_COMPLETE,           /**< Receive complete */
      QUEUED_RX_COMPLETE,    /**< Queued receive completed */
      TX_COMPLETE,           /**< Transmit complete */
      QUEUED_TX_COMPLETE,    /**< Queued transmit completed */
      TXRX_COMPLETE,         /**< Transmit receive operation complete */
      QUEUED_TX_RX_COMPLETE, /**< Queued transfer completed */
      SLAVE_RX_BYTE,         /**< Slave receive buffer got a byte */
      SLAVE_RX_HALF,         /**< Slave receive buffer half full */
      SLAVE_RX_FULL,         /**< Slave receive buffer full */
    };
  }
}  // namespace Chimera

#endif
