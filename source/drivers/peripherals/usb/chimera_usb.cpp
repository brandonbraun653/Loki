/********************************************************************************
 *  File Name:
 *    chimera_usb.cpp
 *
 *  Description:
 *    Peripheral driver for USB
 *
 *  2020-2021 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/usb>

namespace Chimera::USB
{
  /*-------------------------------------------------------------------------------
  Static Data
  -------------------------------------------------------------------------------*/
  static Backend::DriverConfig s_backend_driver;

  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  namespace Backend
  {
    Chimera::Status_t __attribute__( ( weak ) ) registerDriver( Chimera::USB::Backend::DriverConfig &registry )
    {
      registry.isSupported = false;
      return Chimera::Status::NOT_SUPPORTED;
    }
  }  // namespace Backend

  namespace Peripheral
  {
    Chimera::Status_t initialize()
    {
      memset( &s_backend_driver, 0, sizeof( s_backend_driver ) );

      /*------------------------------------------------
      Register the backend interface with Chimera
      ------------------------------------------------*/
      auto result = Backend::registerDriver( s_backend_driver );
      if ( result != Chimera::Status::OK )
      {
        return result;
      }

      /*------------------------------------------------
      Try and invoke the registered init sequence
      ------------------------------------------------*/
      if ( s_backend_driver.isSupported && s_backend_driver.initialize )
      {
        return s_backend_driver.initialize();
      }
      else
      {
        return Chimera::Status::NOT_SUPPORTED;
      }

      return result;
    }


    Chimera::Status_t reset()
    {
      if ( s_backend_driver.isSupported && s_backend_driver.reset )
      {
        return s_backend_driver.reset();
      }
      else
      {
        return Chimera::Status::NOT_SUPPORTED;
      }
    }


    Driver_rPtr getDriver( const Chimera::USB::Channel channel )
    {
      if ( s_backend_driver.isSupported && s_backend_driver.getDriver )
      {
        return s_backend_driver.getDriver( channel );
      }
      else
      {
        return nullptr;
      }
    }
  }  // namespace Peripheral
}  // namespace Chimera::USB
