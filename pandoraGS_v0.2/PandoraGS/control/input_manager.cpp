/*******************************************************************************
PANDORAGS project - PS1 GPU driver
------------------------------------------------------------------------
Author  :     Romain Vinders
License :     GPLv2
------------------------------------------------------------------------
File name :   input_manager.h
Description : user input manager
*******************************************************************************/
#ifdef _WINDOWS
#include <Windows.h>
#endif
using namespace std;
#include "config.h"
#include "psx_core_memory.h"
#include "log_utility.h"
#include "service_main.h"
#include "input_manager.h"

// global data
extern Config* g_pConfig;        // main configuration reference
#ifdef _WINDOWS
static WNDPROC  pEmulatorWindowHandler = 0;
static WNDPROC  pDisplayWindowHandler = 0;
#endif

bool InputManager::m_isListening = false; // state indicator (main)
bool InputManager::m_isListeningDisplay = false; // state indicator (display)

bool InputManager::m_isShowingMenu = false;
int  InputManager::m_menuIndex = 0;
bool InputManager::m_isProfileChangePending = false;

int  InputManager::m_frameSpeed = 0;
bool InputManager::m_isSizeChangePending = false;
bool InputManager::m_isStretchingChangePending = false;
bool InputManager::m_isWindowModeChangePending = false;


/// <summary>Start keyboard listener</summary>
void InputManager::initListener()
{
    if (m_isListening)
        return;
    // default values
    m_isShowingMenu = false;
    m_frameSpeed = 0;
    m_isSizeChangePending = false;
    m_isStretchingChangePending = false;
    m_isWindowModeChangePending = false;
    m_isProfileChangePending = false;

    #ifdef _WINDOWS
    // start event listener + save original listener
    if (!pEmulatorWindowHandler)
    {
        pEmulatorWindowHandler = (WNDPROC)GetWindowLong(PsxCoreMemory::gen_hWindow, GWL_WNDPROC);
        SetWindowLong(PsxCoreMemory::gen_hWindow, GWL_WNDPROC, (long)keyHandler);
    }
    #endif
    m_isListening = true;
    m_isListeningDisplay = (g_pConfig->dsp_isFullscreen == false);
}

/// <summary>Stop keyboard listener</summary>
void InputManager::stopListener()
{
    if (m_isListening)
    {
        #ifdef _WINDOWS
        // restore original event listener
        if (pEmulatorWindowHandler)
            SetWindowLong(PsxCoreMemory::gen_hWindow, GWL_WNDPROC, (long)pEmulatorWindowHandler);
        pEmulatorWindowHandler = 0;
        #endif
        m_isListening = false;
    }
    m_isListeningDisplay = false;

    // default values
    m_isShowingMenu = false;
    m_frameSpeed = 0;
    m_isStretchingChangePending = false;
    m_isWindowModeChangePending = false;
    m_isProfileChangePending = false;
}

/// <summary>Update keyboard listener, based on settings</summary>
void InputManager::updateListener()
{
    m_isListeningDisplay = (g_pConfig->dsp_isFullscreen == false);
}

#ifdef _WINDOWS
/// <summary>Enable or disable screen saver</summary>
/// <param name="isEnabled">Enabled/disabled</param>
void InputManager::setScreensaver(bool isEnabled)
{
    // dynamic thread state system call
    EXECUTION_STATE(WINAPI *D_SetThreadExecutionState)(EXECUTION_STATE esFlags);
    HINSTANCE hKernel32 = NULL;

    // load the kernel32.dll library
    hKernel32 = LoadLibrary(L"kernel32.dll");
    if (hKernel32 != NULL)
    {
        D_SetThreadExecutionState = (EXECUTION_STATE(WINAPI *)(EXECUTION_STATE))GetProcAddress(hKernel32, "SetThreadExecutionState");
        if (D_SetThreadExecutionState != NULL)
        {
            if (isEnabled)
                D_SetThreadExecutionState(ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED); // enabled
            else
                D_SetThreadExecutionState(ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED | ES_CONTINUOUS); // disabled
        }
        FreeLibrary(hKernel32);
    }
}

/// <summary>Handle keyboard in Windows</summary>
/// <param name="hWindow">Window handler</param>
/// <param name="eventType">Event type</param>
/// <param name="wpCode">Command code</param>
/// <param name="lpInfo">Additional information</param>
LRESULT CALLBACK keyHandler(HWND hWindow, UINT eventType, WPARAM wpCode, LPARAM lpInfo)
{
    switch (eventType)
    {
        // system commands
        case WM_SYSCOMMAND:
            if (wpCode == SC_SCREENSAVE && g_pConfig->misc_isScreensaverDisabled) // screensaver
                return 0L; // disable
            break;

        // key pressed
        case WM_KEYDOWN:
            if (wpCode == (WPARAM)(g_pConfig->misc_gpuKeys[(int)GpuKeys_FastForward])) // fast-forward
            {
                InputManager::m_frameSpeed = FrameSpeed_FastForward;
            }
            break;

        // key released
        case WM_SYSKEYUP:
            if (wpCode == VK_RETURN // window mode
            && InputManager::m_isWindowModeChangePending == false) // not busy
                InputManager::m_isWindowModeChangePending = true;
            break;
        case WM_KEYUP:
        {
            // speed modifications
            if (wpCode == (WPARAM)(g_pConfig->misc_gpuKeys[(int)GpuKeys_FastForward]))
            {
                InputManager::m_frameSpeed = FrameSpeed_Normal;
                break;
            }
            if (wpCode == (WPARAM)(g_pConfig->misc_gpuKeys[(int)GpuKeys_SlowMotion]))
            {
                if (InputManager::m_frameSpeed != FrameSpeed_Normal)
                    InputManager::m_frameSpeed = FrameSpeed_Normal;
                else
                    InputManager::m_frameSpeed = FrameSpeed_SlowMotion;
                break;
            }

            // profile management
            if (wpCode == (WPARAM)(g_pConfig->misc_gpuKeys[(int)GpuKeys_ProfileMenu])) // show/hide menu
            {
                if (InputManager::m_isShowingMenu == false)
                {
                    InputManager::m_menuIndex = g_pConfig->getCurrentProfileId();
                    InputManager::m_isShowingMenu = true;
                }
                else
                {
                    InputManager::m_isShowingMenu = false;
                }
                break;
            }
            if (wpCode == (WPARAM)(g_pConfig->misc_gpuKeys[(int)GpuKeys_ProfileConfirm])) // change profile
            {
                if (InputManager::m_isShowingMenu)
                {
                    InputManager::m_isProfileChangePending = (InputManager::m_menuIndex != g_pConfig->getCurrentProfileId());
                }
                break;
            }
            if (wpCode == (WPARAM)(g_pConfig->misc_gpuKeys[(int)GpuKeys_ProfileDefault]))
            {
                if (InputManager::m_isShowingMenu)
                {
                    g_pConfig->getPrevProfileId(1uL);
                    InputManager::m_menuIndex = 0;
                }
                break;
            }
            if (wpCode == (WPARAM)(g_pConfig->misc_gpuKeys[(int)GpuKeys_ProfilePrev]))
            {
                if (InputManager::m_isShowingMenu)
                {
                    unsigned int newId = g_pConfig->getPrevProfileId(InputManager::m_menuIndex);
                    InputManager::m_menuIndex = newId;
                }
                break;
            }
            if (wpCode == (WPARAM)(g_pConfig->misc_gpuKeys[(int)GpuKeys_ProfileNext]))
            {
                if (InputManager::m_isShowingMenu)
                {
                    unsigned int newId = g_pConfig->getNextProfileId(InputManager::m_menuIndex);
                    InputManager::m_menuIndex = newId;
                }
                break;
            }

            // miscellaneous
            if (wpCode == (WPARAM)(g_pConfig->misc_gpuKeys[(int)GpuKeys_Debug]))
            {
                if (g_pConfig->rnd_debugMode == DebugMode_None) // not debugging -> toggle FPS display
                {
                    g_pConfig->rnd_isFpsDisplayed = !(g_pConfig->rnd_isFpsDisplayed);
                }
                else // debug mode -> next mode
                {
                    if (g_pConfig->rnd_debugMode < DebugMode_TexturedMode)
                        g_pConfig->rnd_debugMode = (DebugMode)((int)g_pConfig->rnd_debugMode + 1);
                    else
                        g_pConfig->rnd_debugMode = DebugMode_BaseMode;
                }
                break;
            }
            if (wpCode == (WPARAM)(g_pConfig->misc_gpuKeys[(int)GpuKeys_Stretching])) // screen stretching mode
            {
                if (g_pConfig->getCurrentProfile()->dsp_screenStretch < CFGSTRETCHING_LAST)
                    g_pConfig->getCurrentProfile()->dsp_screenStretch++;
                else
                    g_pConfig->getCurrentProfile()->dsp_screenStretch = CfgStretching_Stretch;
                InputManager::m_isStretchingChangePending = true;
                InputManager::m_isWindowModeChangePending = true;
                break;
            }
        }
        // window position/size/state changed
        case WM_WINDOWPOSCHANGED:
        {
            // only in window mode (and no change pending)
            if (InputManager::m_isListeningDisplay &&
                InputManager::m_isWindowModeChangePending == false && g_pConfig->dsp_isFullscreen == false)
            {
                InputManager::m_isListeningDisplay = false;
                if (wpCode == SIZE_MAXIMIZED || wpCode == SIZE_RESTORED) // not minimized
                {
                    static WPARAM prevState = wpCode;
                    if (prevState != wpCode) // different window state
                    {
                        prevState = wpCode;
                        InputManager::m_isStretchingChangePending = true;
                        InputManager::m_isWindowModeChangePending = true;
                        return 0L;
                    }
                    else // window resized
                    {
                        WINDOWPOS* winpos = (WINDOWPOS*)lpInfo;
                        static int prevCx = winpos->cx;
                        static int prevCy = winpos->cy;
                        if (prevCx != winpos->cx || prevCy != winpos->cy) // different size
                        {
                            prevCx = winpos->cx;
                            prevCy = winpos->cy;
                            InputManager::m_isSizeChangePending = true;
                            InputManager::m_isStretchingChangePending = true;
                            InputManager::m_isWindowModeChangePending = true;
                            return 0L;
                        }
                    }
                }
            }
            break;
        }
    }
    // transmit event to emulator
    return pEmulatorWindowHandler(hWindow, eventType, wpCode, lpInfo);
}


#else
#define VK_INSERT      65379
#define VK_HOME        65360
#define VK_PRIOR       65365
#define VK_NEXT        65366
#define VK_END         65367
#define VK_DEL         65535
#define VK_F5          65474

/// <summary>Handle keyboard in Linux</summary>
/// <param name="keycode">Pressed key code</param>
void GPUkeypressed(int keycode)
{
    switch (keycode)
    {
        default: //...
            break;
    }
}
#endif
