/*******************************************************************************
PANDORAGS project - PS1 GPU driver
------------------------------------------------------------------------
Author  :     Romain Vinders
License :     GPLv2
------------------------------------------------------------------------
File name :   config_pmanager_winview.h
Description : configuration dialog page - profiles manager - view
*******************************************************************************/
#ifndef _CONFIG_PMANAGER_WINVIEW_H
#define _CONFIG_PMANAGER_WINVIEW_H
#include "globals.h"
#if _DIALOGAPI == DIALOGAPI_WIN32

#include "config_page_winview.h"


// Configuration dialog page - profiles manager - view
class ConfigPageManagerView : public ConfigPageView
{
private:
    HANDLE res_iconAdd;
    HANDLE res_iconEdit;
    HANDLE res_iconDel;
    HANDLE res_iconIn;
    HANDLE res_iconOut;

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

    /// <summary>Refresh language-dependent page content</summary>
    virtual void resetLanguage();
    /// <summary>Copy UI settings to global configuration</summary>
    virtual void updateConfig();

    /// <summary>Load page content</summary>
    /// <param name="hWindow">Window handle</param>
    /// <param name="phInstance">Instance handle reference</param>
    /// <param name="pPageSize">Page limits (rectangle)</param>
    /// <param name="isVisible">Visibility (show/hide)</param>
    /// <exception cref="std::exception">Creation failure</exception>
    virtual void loadPage(HWND hWindow, HINSTANCE* phInstance, RECT* pPageSize, bool isVisible);

    /// <summary>Page event handler</summary>
    /// <param name="hWindow">Window handle</param>
    /// <param name="msg">Event message</param>
    /// <param name="wParam">Command</param>
    /// <param name="lParam">Informations</param>
    /// <returns>Action code</returns>
    static INT_PTR CALLBACK eventHandler(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);
};

#endif
#endif
