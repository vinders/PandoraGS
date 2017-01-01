/*******************************************************************************
PANDORAGS project - PS1 GPU driver
------------------------------------------------------------------------
Author  :     Romain Vinders
License :     GPLv2
------------------------------------------------------------------------
File name :   system_tools.h
Description : system management toolbox
*******************************************************************************/
#ifndef _SYSTEM_TOOLS_H
#define _SYSTEM_TOOLS_H
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <string>
#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif

// data types
typedef int32_t buffer_t;
typedef uint32_t ubuffer_t;
enum change_t : int32_t // pending change
{
    Change_none = 0,
    Change_disable = 1,
    Change_enable = 2
};


// System management toolbox
namespace SystemTools
{
    /// <summary>Create a new output console window</summary>
    void createOutputWindow();
    /// <summary>Close current output console window</summary>
    void closeOutputWindow();
    /// <summary>Set cursor position in output console window</summary>
    /// <param name="line">Line number</param>
    void setConsoleCursorPos(int line);

#ifdef _WINDOWS
    /// <summary>Create a new display window</summary>
    /// <param name="hWindow">Main window handle</param>
    /// <param name="isFullscreen">Fullscreen or not</param>
    /// <param name="isResizable">Resizable window (if window mode) or not</param>
    /// <param name="winResX">Window width (if window mode)</param>
    /// <param name="winResY">Window height (if window mode)</param>
    void createDisplayWindow(HWND hWindow, bool isFullscreen, bool isResizable, uint32_t winResX, uint32_t winResY);
    /// <summary>Close current display window</summary>
    /// <param name="hWindow">Main window handle</param>
    void closeDisplayWindow(HWND hWindow);
    /// <summary>Fill display window</summary>
    /// <param name="hDC">Main display context</param>
    /// <param name="lx">Width</param>
    /// <param name="ly">Height</param>
    void fillDisplayWindow(HDC hDC, uint32_t lx, uint32_t ly);
    /// <summary>Get dimensions of display window</summary>
    /// <param name="hWindow">Display window handle</param>
    /// <param name="outX">Width destination</param>
    /// <param name="outY">Height destination</param>
    bool getDisplayWindowSize(HWND hWindow, long& outX, long& outY);
    /// <summary>Enable or disable screen-saver</summary>
    /// <param name="isEnabled">Enabled/disabled</param>
    void setScreensaver(bool isEnabled);
#else
    /// <summary>Create a new output console window</summary>
    void createDisplayWindow();
    /// <summary>Close current output console window</summary>
    void closeDisplayWindow();
#endif

    /// <summary>Get file path with write access (same as plugin or home path)</summary>
    /// <returns>File path</returns>
    std::string getWritableFilePath();
}

#endif
