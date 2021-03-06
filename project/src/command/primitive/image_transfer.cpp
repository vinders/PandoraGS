/*******************************************************************************
PANDORAGS project - PS1 GPU driver
------------------------------------------------------------------------
Author  :     Romain Vinders
License :     GPLv2
------------------------------------------------------------------------
Description : image transfer command
*******************************************************************************/
#include "../../globals.h"
#include "primitive_facade.h"
#include "image_transfer.h"
using namespace command::primitive;
#pragma pack(push, 4)


// -- attribute types - data transfer settings -- --------------------------

/// @brief Clear texture cache
/// @param[in] pData  Raw attribute data pointer
void cache_clear_t::process(command::cmd_block_t* pData)
{
    //...
}

/// @brief Load image (cpu to vram)
/// @param[in] pData  Raw attribute data pointer
void img_load_t::process(command::cmd_block_t* pData)
{
    img_load_t* pAttr = (img_load_t*)pData;

    //...
}

/// @brief Store image (vram to central memory)
/// @param[in] pData  Raw attribute data pointer
void img_store_t::process(command::cmd_block_t* pData)
{
    img_store_t* pAttr = (img_store_t*)pData;

    //...
}

/// @brief Frame buffer rectangle copy (vram to vram)
/// @param[in] pData  Raw attribute data pointer
void img_move_t::process(command::cmd_block_t* pData)
{
    img_move_t* pAttr = (img_move_t*)pData;

    //...
}

#pragma pack(pop)
