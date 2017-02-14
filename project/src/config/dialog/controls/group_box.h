/*******************************************************************************
PANDORAGS project - PS1 GPU driver
------------------------------------------------------------------------
Author  :     Romain Vinders
License :     GPLv2
------------------------------------------------------------------------
Description : custom group box
*******************************************************************************/
#pragma once

#include "../../../globals.h"
#include <cstdint>
#include "common.h"

/// @namespace config
/// Configuration management
namespace config
{
    /// @namespace config.dialog
    /// Configuration dialog management
    namespace dialog
    {
        /// @namespace config.dialog.controls
        /// Dialog controls
        namespace controls
        {
            /// @class GroupBox
            /// @brief Custom group box
            class GroupBox
            {
            public:
                /// @brief Set group box text
                /// @param[in] hWindow     Parent window handle
                /// @param[in] resourceId  Group box resource identifier
                /// @param[in] text        Text value
                static inline void setText(window_handle_t hWindow, const int32_t resourceId, const std::wstring text) noexcept
                {
                    #if _DIALOGAPI == DIALOGAPI_WIN32
                    SetDlgItemText(reinterpret_cast<HWND>(hWindow), resourceId, (LPCWSTR)text.c_str());
                    #else
                    //...
                    #endif
                }
            };
        }
    }
}