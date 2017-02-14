/*******************************************************************************
PANDORAGS project - PS1 GPU driver
------------------------------------------------------------------------
Author  :     Romain Vinders
License :     GPLv2
------------------------------------------------------------------------
Description : tab page - profile manager
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
        /// @class ManagerPage
        /// @brief Tab page - profile manager
        /// @see TabPage
        class ManagerPage : public controls::TabPage
        {
        private:
        
        public:
            ManagerPage(controls::library_instance_t instance) : controls::TabPage(instance, IDD_MANAGER_PAGE) {}

            // -- specialized handlers -- --------------------------------------

            /// @brief Language change event
            /// @param[in] isRecursive  Also translate controls in child pages or not
            virtual void onLanguageChange(bool isRecursive);
        };
    }
}