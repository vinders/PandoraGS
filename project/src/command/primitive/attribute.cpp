/*******************************************************************************
PANDORAGS project - PS1 GPU driver
------------------------------------------------------------------------
Author  :     Romain Vinders
License :     GPLv2
------------------------------------------------------------------------
Description : drawing attribute (area / transparency)
*******************************************************************************/
#include "../../globals.h"
#include "primitive_facade.h"
#include "attribute.h"
using namespace command::primitive;


// -- attribute types - frame buffer settings -- ---------------------------

/// @brief Process draw mode / texture page change
/// @param[in] pData  Raw attribute data pointer
void attr_texpage_t::process(command::cmd_block_t* pData)
{
    attr_texpage_t* pAttr = (attr_texpage_t*)pData;

    //...
}

/// @brief Process texture window change
/// @param[in] pData  Raw attribute data pointer
void attr_texwin_t::process(command::cmd_block_t* pData)
{
    attr_texwin_t* pAttr = (attr_texwin_t*)pData;

    //...
}

/// @brief Process drawing area change
/// @param[in] pData  Raw attribute data pointer
void attr_drawarea_t::process(command::cmd_block_t* pData)
{
    attr_drawarea_t* pAttr = (attr_drawarea_t*)pData;

    //...
}

/// @brief Process drawing offset modification
/// @param[in] pData  Raw attribute data pointer
void attr_drawoffset_t::process(command::cmd_block_t* pData)
{
    attr_drawoffset_t* pAttr = (attr_drawoffset_t*)pData;

    //...
}

/// @brief Process semi-transparency bit change
/// @param[in] pData  Raw attribute data pointer
void attr_stpmask_t::process(command::cmd_block_t* pData)
{
    attr_stpmask_t* pAttr = (attr_stpmask_t*)pData;

    //...
}

/// @brief Process GPU interrupt request flag
/// @param[in] pData  Raw attribute data pointer
void attr_irqflag_t::process(command::cmd_block_t* pData)
{
    // v�rif si IRQ1 activ�
    // si oui, positionner flag d'interruption
    //...
}
