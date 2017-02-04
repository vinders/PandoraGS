/*******************************************************************************
PANDORAGS project - PS1 GPU driver
------------------------------------------------------------------------
Author  :     Romain Vinders
License :     GPLv2
------------------------------------------------------------------------
Description : vertex data buffer
*******************************************************************************/
#include "../../globals.h"
#include <cstdlib>
#include <cstdint>
#include "vertex_buffer.h"
using namespace command;


/// @brief Create default vertex buffer
memory::VertexBuffer::VertexBuffer()
{
    m_pVertexCoords = new std::vector<float>();
    m_pVertexColors = new std::vector<uint32_t>();
    m_pTextureInfos = nullptr;
}

/// @brief Create and configure vertex buffer
/// @param[in] baseCapacity  Base capacity (pre-allocated number of vertices)
/// @param[in] isTextured    Use texture data
memory::VertexBuffer::VertexBuffer(const uint32_t baseCapacity, const bool isTextured)
{
    m_pVertexCoords = new std::vector<float>();
    m_pVertexColors = new std::vector<uint32_t>();
    m_pTextureInfos = nullptr;
    init(baseCapacity, isTextured);
}

/// @brief Destroy vertex buffer
memory::VertexBuffer::~VertexBuffer()
{
    delete m_pVertexCoords;
    delete m_pVertexColors;
    if (m_pTextureInfos != nullptr)
        delete m_pTextureInfos;
}


/// @brief Configure vertex buffer
/// @param[in] baseCapacity  Base capacity (pre-allocated number of vertices)
/// @param[in] isTextured    Use texture data
void memory::VertexBuffer::init(const uint32_t baseCapacity, const bool isTextured)
{
    // coords (X, Y, Z)
    m_pVertexCoords->reserve(baseCapacity * 3u);
    // colors
    m_pVertexColors->reserve(baseCapacity);

    // texture coords
    if (isTextured)
    {
        if (m_pTextureInfos == nullptr)
            m_pTextureInfos = new std::vector<uint32_t>();
        m_pTextureInfos->reserve(baseCapacity);
    }
    else if (m_pTextureInfos != nullptr)
    {
        delete m_pTextureInfos;
        m_pTextureInfos = nullptr;
    }
}
