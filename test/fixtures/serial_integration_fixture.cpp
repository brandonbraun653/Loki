/********************************************************************************
 *  File Name:
 *    serial_integration_fixture.cpp
 *
 *  Description:
 *    Implements the Serial test fixture for the Chimera backend driver
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* Chimera Includes */
#include <Chimera/gpio.hpp>

/* Testing Includes */
#include <Chimera/test/fixtures/serial_integration_fixture.hpp>

namespace Chimera
{
  namespace Serial
  {
    void SerialIntegrationTestFixture::SetUp()
    {
      serial = new CHIMERA_INHERITED_SERIAL();
    }

    void SerialIntegrationTestFixture::TearDown()
    {
      delete serial;
    }

    void SerialIntegrationTestFixture::defaultInit()
    {
    }

  }  // namespace Serial
}  // namespace Chimera