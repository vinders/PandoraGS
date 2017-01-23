/*******************************************************************************
PANDORAGS project - PS1 GPU driver
------------------------------------------------------------------------
Author  :     Romain Vinders
License :     GPLv2
------------------------------------------------------------------------
Description : drawing polygon primitive (triangle / quad)
*******************************************************************************/
#pragma once

#include "i_primitive.h"

/// @namespace command
/// GPU commands management
namespace command
{
    /// @namespace command.primitive
    /// Drawing primitive management
    namespace primitive
    {
        /// @class PolyPrimitive
        /// @brief Drawing polygon primitive (triangle / quad)
        class PolyPrimitive : public IPrimitive
        {
        public:
            /// @brief Create primitive
            /// @param pData Raw primitive data
            /// @param frameSettings Current frame buffer settings
            PolyPrimitive(unsigned long* pData, FrameBufferSettings& frameSettings);
            
            /// @brief Process primitive
            /// @param pOutSlots Next drawing primitive slots
            virtual void process(const float* pOutSlots);
        };
    }
}
