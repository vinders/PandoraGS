/*******************************************************************************
PANDORAGS project - PS1 GPU driver
------------------------------------------------------------------------
Author  :     Romain Vinders
License :     GPLv2
------------------------------------------------------------------------
File name :   config_pmanager_qtview.h
Description : configuration dialog page - profiles manager - view
*******************************************************************************/
#ifndef _CONFIG_PMANAGER_QTVIEW_H
#define _CONFIG_PMANAGER_QTVIEW_H
#include "globals.h"
#if _DIALOGAPI == DIALOGAPI_QT

#include "config_page_qtview.h"


// Configuration dialog page - profiles manager - view
class ConfigPageManagerView : public ConfigPageView
{
public:
    /// <summary>Create page view container</summary>
    /// <param name="pController">Controller reference</param>
    ConfigPageManagerView(ConfigPage* pController);
    /// <summary>Destroy dialog view container</summary>
    ~ConfigPageManagerView();

    /// <summary>Create new dialog page</summary>
    /// <param name="pController">Controller reference</param>
    /// <returns>Page created</returns>
    /// <exception cref="std::exception">Creation failure</exception>
    static ConfigPageManagerView* createPage(ConfigPageManager* pController);
};

#endif
#endif
