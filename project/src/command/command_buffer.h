/*******************************************************************************
PANDORAGS project - PS1 GPU driver
------------------------------------------------------------------------
Author  :     Romain Vinders
License :     GPLv2
------------------------------------------------------------------------
Description : fifo GPU command buffer
*******************************************************************************/
#pragma once

#include <cstdint>
#include <thread>
#include "memory/vertex_buffer.h"
#include "frame_buffer_settings.h"

/// @namespace command
/// GPU commands management
namespace command
{
    /// @class CommandBuffer
    /// @brief FIFO GPU command buffer
    class CommandBuffer
    {
    private:
        FrameBufferSettings m_drawSettings;
        memory::VertexBuffer* m_pLineBuffer;
        memory::VertexBuffer* m_pPolyBuffer;
        uint32_t m_currentPrimitiveCount;

        bool m_isBusy;


    private:
        void lock()
        {
            while (m_isBusy)
                std::this_thread::yield();
            m_isBusy = true;
        }

        void unlock()
        {
            m_isBusy = false;
        }

    public:
        CommandBuffer()
        {
            // PS1 GPU limits :
            //  - theoretically 360000 flat-shaded polygons per second -> 12000/frame
            //  - theoretically 180000 textured/gouraud-shaded polygons per second -> 6000/frame
            m_pLineBuffer = new memory::VertexBuffer(12000 * 2, false);
            m_pPolyBuffer = new memory::VertexBuffer(10000 * 3, false);

            //! ne pas vider contenu de frame avant de dessiner -> garder "restes" de la frame pr�c�dente -> effets de tornade de certains jeux (crash 3, ff 7, ...)
            //! ajout option pour vider frame avant dessin
        }
    };
}
