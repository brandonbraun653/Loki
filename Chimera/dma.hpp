/********************************************************************************
 *  File Name:
 *    dma.hpp
 *
 *  Description:
 *    Implements a common wrapper interface to the Chimera DMA peripheral.
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_DMA_HPP
#define CHIMERA_DMA_HPP

/* C/C++ Includes */
#include <memory>
#include <stdint.h>

/* Chimera Includes */
#include "chimeraPort.hpp"
#include <Chimera/interface/dma_intf.hpp>

namespace Chimera::DMA
{
#if !defined( CHIMERA_INHERITED_DMA )
  using CHIMERA_INHERITED_DMA = DMAUnsupported;
#endif

  /**
   *  A simple wrapper to provide a common DMA class type for programs built
   *  with Chimera. The runtime behavior of this class is defined by the user
   *  provided class type CHIMERA_INHERITED_DMA in chimeraPort.hpp.
   *
   *  If no user class is provided, a default disabled behavior version will be
   *  substituted in its place.
   */
  class DMAClass : public CHIMERA_INHERITED_DMA
  {
  public:
    DMAClass() : CHIMERA_INHERITED_DMA(){};
    ~DMAClass() = default;
  };

  static_assert( std::is_base_of<Interface, DMAClass>::value, "Base class implements the wrong interface" );

}  // namespace Chimera::DMA

#endif /* !CHIMERA_DMA_HPP */