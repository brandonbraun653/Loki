/********************************************************************************
 *   File Name:
 *    callback_types.hpp
 *
 *   Description:
 *    Various types of callback signatures used in Chimera. This is surprisingly
 *    difficult to keep track of the naming, so an incredibly generic format is 
 *    used. Whether or not this is a good idea is up for debate.
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_CALLBACK_TYPES_HPP
#define CHIMERA_CALLBACK_TYPES_HPP

/* C++ Includes */
#include <cstdint>
#include <functional>

namespace Chimera::Callback
{

  /**
   *  A generic callback type that can be used across a wide variety of
   *  peripheral ISR events. Sometimes the callbacks need data and sometimes
   *  they do not, so a 'handle' like function signature was used to give the
   *  most flexibility. The const data specifier was intentionally left removed
   *  so that in a unique scenario the callbacks can talk directly with the handle.
   *
   *  @param[in]  handle    Some data structure that the ISR wants to give to callbacks
   *  @param[in]  size      The size of the handle structure in bytes
   *  @return void
   */
  using ISRCallbackFunction = std::function<void( void *const handle, const size_t size )>;

}  // namespace Chimera::Callback

#endif /* !CHIMERA_CALLBACK_TYPES_HPP */