/********************************************************************************
 *   File Name:
 *
 *
 *   Description:
 *
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_THREADING_INTERFACE_HPP
#define CHIMERA_THREADING_INTERFACE_HPP

/* Chimera Includes */
#include <Chimera/types/event_types.hpp>

#ifdef USING_FREERTOS

/* FreeRTOS Includes */
#ifdef __cplusplus
extern "C"
{
#include "FreeRTOS.h"
#include "semphr.h"
}
#endif /* __cplusplus */
#endif /* USING_FREERTOS */

namespace Chimera::Threading
{
  
  /**
   *
   */
  class AsyncIOBaseInterface
  {
  public:

    /**
     *  Asynchronously waits for the given event to occur before the function 
     *  will return. The is accomplished by blocking the current thread.
     *
     *  @param[in]  event       The event upon which to be triggered
     *  @return void
     */
    virtual void await( const Chimera::Event::Trigger event ) = 0;

    /**
     *  A more explicit version of await that allows selecting the threading
     *  primitive on which to block. This could be useful if multiple owners 
     *  might unblock a process or if the event generator is nested several
     *  calls deep in the stack.
     *
     *  @param[in]  event       The event upon which to be triggered
     *  @param[in]  notifier    Semaphore to be given to upon the event occurrance
     *  @return void
     */
    virtual void await( const Chimera::Event::Trigger event, SemaphoreHandle_t notifier ) = 0;
  };

}  // namespace Chimera::Threading

#endif /* !CHIMERA_THREADING_INTERFACE_HPP */