/*******************************************************************************
PANDORAGS project - PS1 GPU driver
------------------------------------------------------------------------
Author  :     Romain Vinders
License :     GPLv2
------------------------------------------------------------------------
Description : tab page - general settings
*******************************************************************************/
#pragma once

#include "controls/common.h"
#include "controls/tab_page.h"
#include "controls/tab_control.h"

/// @namespace config
/// Configuration management
namespace config
{
    /// @namespace config.dialog
    /// Configuration dialog management
    namespace dialog
    {
        /// @class GeneralPage
        /// @brief Tab page - general settings
        /// @see TabPage
        class GeneralPage : public controls::TabPage
        {
        private:
        
        public:
            /// @brief Create tab page - general settings
            /// @param[in] instance       Current instance handle
            /// @param[in] pParentDialog  Parent dialog reference
            GeneralPage(controls::library_instance_t instance, controls::Dialog* pParentDialog)
                : controls::TabPage(instance, pParentDialog, IDD_GENERAL_PAGE) {}


            // -- specialized handlers -- --------------------------------------

            /// @brief Language change event
            /// @param[in] isRecursive    Also translate controls in child pages or not
            virtual void onLanguageChange(const bool isRecursive);
        };
    }
}