#pragma once
#ifndef CHIMERA_SPI_HPP
#define CHIMERA_SPI_HPP

/* C/C++ Includes */
#include <memory>

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/config.hpp>

namespace Chimera
{
	namespace SPI
	{
		class SPIClass : public CHIMERA_INHERITED_SPI
		{
		public:

			SPIClass(const int& channel) : CHIMERA_INHERITED_SPI( channel ) {};
			~SPIClass() = default;
        };

		typedef std::shared_ptr<SPIClass> SPIClass_sPtr;
		typedef std::unique_ptr<SPIClass> SPIClass_uPtr;
	}
}

#endif
