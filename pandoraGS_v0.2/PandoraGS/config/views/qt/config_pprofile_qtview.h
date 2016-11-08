/*******************************************************************************
PANDORAGS project - PS1 GPU driver
------------------------------------------------------------------------
Author  :     Romain Vinders
License :     GPLv2
------------------------------------------------------------------------
File name :   config_pprofile_qtview.h
Description : configuration dialog page - profile settings - view
*******************************************************************************/
#ifndef _CONFIG_PPROFILE_QTVIEW_H
#define _CONFIG_PPROFILE_QTVIEW_H
#include "globals.h"
#if _DIALOGAPI == DIALOGAPI_QT

#include "config_page_qtview.h"


// Configuration dialog page - profile settings - view
class ConfigPageProfileView : public ConfigPageView
{
public:
    /// <summary>Create page view container</summary>
    /// <param name="pController">Controller reference</param>
    ConfigPageProfileView(ConfigPage* pController);
    /// <summary>Destroy dialog view container</summary>
    ~ConfigPageProfileView();

    /// <summary>Create new dialog page</summary>
    /// <param name="pController">Controller reference</param>
    /// <returns>Page created</returns>
    /// <exception cref="std::exception">Creation failure</exception>
    static ConfigPageProfileView* createPage(ConfigPageProfile* pController);
};

#endif
#endif
