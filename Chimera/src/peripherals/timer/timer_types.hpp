/********************************************************************************
 *  File Name:
 *    timer_types.hpp
 *
 *	 Description:
 *    Types for the Chimera Timer module
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_TIMER_TYPES_HPP
#define CHIMERA_TIMER_TYPES_HPP

/* STL Includes */
#include <cstdlib>
#include <memory>

/* Chimera Includes */
#include <Chimera/common>

namespace Chimera::Timer
{
  enum class Direction : uint8_t
  {
    COUNT_UP,    /**< Counts up from min, overflows, then starts counting up again */
    COUNT_DN,    /**< Counts down from max, underflows, then starts counting down again */
    COUNT_UP_DN, /**< Alternates between counting up, down, up, etc */

    NUM_OPTIONS
  };

  enum class Function : uint8_t
  {
    INVALID,
    INPUT_CAPTURE,
    OUTPUT_COMPARE,
    PWM_OUTPUT,
    ONE_PULSE_OUTPUT,
    ENCODER,

    NUM_OPTIONS
  };

  enum class Event : uint8_t
  {
    INVALID,
    // Fill in when needed
    NUM_OPTIONS
  };

  enum class Peripheral : uint8_t
  {
    TIMER1,
    TIMER2,
    TIMER3,
    TIMER4,
    TIMER5,
    TIMER6,
    TIMER7,
    TIMER8,
    TIMER9,
    TIMER10,
    TIMER11,
    TIMER12,
    TIMER13,
    TIMER14,
    TIMER15,
    TIMER16,
    LPTIMER1,
    LPTIMER2,

    NUM_OPTIONS,
    NOT_SUPPORTED
  };

  enum class Channel : uint8_t
  {
    INVALID,
    CHANNEL_1,
    CHANNEL_2,
    CHANNEL_3,
    CHANNEL_4,
    CHANNEL_5,
    CHANNEL_6,

    NUM_OPTIONS
  };

  #if defined( VIRTUAL_FUNC )
  class ITimerBase;
  using ITimerBase_sPtr = std::shared_ptr<ITimerBase>;
  using ITimerBase_uPtr = std::unique_ptr<ITimerBase>;
  #else 
  class TimerBaseImpl;

  template<class T>
  class ITimerBase;

  using ITimerBase_sPtr = ITimerBase<TimerBaseImpl>*;
  using ITimerBase_uPtr = ITimerBase<TimerBaseImpl>*;
  #endif 

  struct DriverConfig
  {
    bool validity;  /**< Decides if the configuration settings are valid */
    bool overwrite; /**< Allows the config to update the entire timer peripheral (multiple channels share one peripheral) */
    Peripheral peripheral;    /**< Which peripheral to configure */
    Direction countDirection; /**< Which direction the free-running counter should count */
    size_t reloadValue;       /**< Value to load when the counter overflows */
  };

  #if defined( VIRTUAL_FUNC )
  class ITimerEncoder;
  using ITimerEncoder_sPtr = std::shared_ptr<ITimerEncoder>;
  using ITimerEncoder_uPtr = std::unique_ptr<ITimerEncoder>;
  #else
  class TimerEncoderImpl;

  template<class T>
  class ITimerEncoder;

  using ITimerEncoder_sPtr = ITimerEncoder<TimerEncoderImpl>*;
  using ITimerEncoder_uPtr = ITimerEncoder<TimerEncoderImpl>*;
  #endif

  namespace Encoder
  {
    struct Config
    {
      // Currently not used
    };
  }  // namespace Encoder

  #if defined( VIRTUAL_FUNC )
  class ITimerInputCapture;
  using ITimerInputCapture_sPtr = std::shared_ptr<ITimerInputCapture>;
  using ITimerInputCapture_uPtr = std::unique_ptr<ITimerInputCapture>;
  #else 
  class TimerInputCaptureImpl;

  template<class T>
  class ITimerInputCapture;

  using ITimerInputCapture_sPtr = ITimerInputCapture<TimerInputCaptureImpl>*;
  using ITimerInputCapture_uPtr = ITimerInputCapture<TimerInputCaptureImpl>*;
  #endif 

  namespace InputCapture
  {
    struct Config
    {
      // Currently not used
    };
  }  // namespace InputCapture

  #if defined( VIRTUAL_FUNC )
  class ITimerOnePulse;
  using ITimerOnePulse_sPtr = std::shared_ptr<ITimerOnePulse>;
  using ITimerOnePulse_uPtr = std::unique_ptr<ITimerOnePulse>;
  #else 
  class TimerOnePulseImpl;

  template<class T>
  class ITimerOnePulse;

  using ITimerOnePulse_sPtr = ITimerOnePulse<TimerOnePulseImpl>*;
  using ITimerOnePulse_uPtr = ITimerOnePulse<TimerOnePulseImpl>*;
  #endif 

  namespace OnePulse
  {
    struct Config
    {
      // Currently not used
    };
  }  // namespace OnePulse

  #if defined( VIRTUAL_FUNC )
  class ITimerOutputCompare;
  using ITimerOutputCompare_sPtr = std::shared_ptr<ITimerOutputCompare>;
  using ITimerOutputCompare_uPtr = std::unique_ptr<ITimerOutputCompare>;
  #else 
  class TimerOutputCompareImpl;

  template<class T>
  class ITimerOutputCompare;

  using ITimerOutputCompare_sPtr = ITimerOutputCompare<TimerOutputCompareImpl>*;
  using ITimerOutputCompare_uPtr = ITimerOutputCompare<TimerOutputCompareImpl>*;
  #endif 

  namespace OutputCompare
  {
    struct Config
    {
      // Currently not used
    };
  }  // namespace OutputCompare

  #if defined( VIRTUAL_FUNC )
  class ITimerPWM;
  using ITimerPWM_sPtr = std::shared_ptr<ITimerPWM>;
  using ITimerPWM_uPtr = std::unique_ptr<ITimerPWM>;
  #else 
  class TimerPWMImpl;

  template<class T>
  class ITimerPWM;

  using ITimerPWM_sPtr = ITimerPWM<TimerPWMImpl>*;
  using ITimerPWM_uPtr = ITimerPWM<TimerPWMImpl>*;
  #endif 

  namespace PWM
  {
    enum class Polarity
    {
      ACTIVE_HIGH,
      ACTIVE_LOW,

      NUM_OPTIONS
    };

    struct Config
    {
      Chimera::Timer::Peripheral peripheral; /**< Timer peripheral in use */
      Chimera::Timer::Channel outputChannel; /**< Channel to configure the PWM output on */
      size_t dutyCycle;                      /**< Initial duty cycle */
      size_t frequency;                      /**< Initial output frequency */
      Polarity polarity;                     /**< Idle state when signal not asserted */
      bool validity;
    };
  }  // namespace PWM

  namespace Backend
  {
    struct DriverRegistration
    {
      bool isSupported;
      Chimera::Status_t ( *initialize )( void );
      Chimera::Status_t ( *reset )( void );
      size_t ( *millis )( void );
      void ( *delayMilliseconds )( const size_t );
      void ( *delayMicroseconds )( const size_t );
    };
  }  // namespace Backend
}  // namespace Chimera::Timer

#endif /* !CHIMERA_TIMER_TYPES_HPP */
