/********************************************************************************
 *  File Name:
 *    types.hpp
 *
 *  Description:
 *    FreeRTOS thread primitive native types
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_THREADING_FREERTOS_TYPES_HPP
#define CHIMERA_THREADING_FREERTOS_TYPES_HPP

#if defined( USING_FREERTOS_THREADS )

/* FreeRTOS Includes */
#include <FreeRTOS/FreeRTOS.h>
#include <FreeRTOS/semphr.h>
#include <FreeRTOS/task.h>
#include <FreeRTOS/queue.h>
#include "portmacro.h"

namespace Chimera::Threading::detail
{
  /*-------------------------------------------------------------------------------
  Mutex Types
  -------------------------------------------------------------------------------*/
  using native_mutex                 = SemaphoreHandle_t;
  using native_recursive_mutex       = SemaphoreHandle_t;
  using native_timed_mutex           = SemaphoreHandle_t;
  using native_recursive_timed_mutex = SemaphoreHandle_t;

  /*-------------------------------------------------------------------------------
  Semaphore Types
  -------------------------------------------------------------------------------*/
  using native_binary_semaphore   = SemaphoreHandle_t;
  using native_counting_semaphore = SemaphoreHandle_t;

  /*-------------------------------------------------------------------------------
  Thread Types
  -------------------------------------------------------------------------------*/
  using native_thread             = TaskHandle_t;
  using native_thread_handle_type = TaskHandle_t;

  /*-------------------------------------------------------------------------------
  Queue Types
  -------------------------------------------------------------------------------*/
  using native_queue           = QueueHandle_t;
  using native_queue_structure = StaticQueue_t;

}  // namespace Chimera::Threading::detail

#endif /* USING_FREERTOS_THREADS */
#endif /* !CHIMERA_THREADING_FREERTOS_TYPES_HPP */