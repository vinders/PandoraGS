/*******************************************************************************
PANDORAGS project - PS1 GPU driver
------------------------------------------------------------------------
Author  :     Romain Vinders
License :     GPLv2
------------------------------------------------------------------------
Description : rendering engine - graphics API management
*******************************************************************************/
#include "../globals.h"
#ifdef _WINDOWS
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif
#include "../vendor/opengl.h" // openGL includes
#include "../config/config.h"
#include "utils/display_window.h"
#include "shader.h"
#include "engine.h"
using namespace display;


// engine status
bool Engine::s_isInitialized = false; ///< Rendering engine initialization status
GLuint Engine::s_programId = 0;       ///< Rendering pipeline program identifier

// window management
utils::DisplayWindow* Engine::s_pWindowManager = nullptr; ///< Main window
device_handle_t Engine::s_windowDeviceContext = 0;        ///< Window device context
#ifdef _WINDOWS
HGLRC Engine::s_openGlRenderContext; ///< API rendering context
#else
GLFWwindow* Engine::s_openGlRenderContext = nullptr; ///< API window context
#endif


/// @brief Create and initialize display window
/// @param[in] window  Parent window handle
/// @throws runtime_error  Window creation failure
void Engine::createDisplayWindow(window_handle_t window)
{
    // create display window
    utils::DisplayWindow* pWindowManager = nullptr;
    try
    {
        pWindowManager = new utils::DisplayWindow(window);
        pWindowManager->show(config::Config::display.windowRes.x, config::Config::display.windowRes.y,
            static_cast<int32_t>(config::Config::display.windowMode));
        if (config::Config::display.windowMode == display::utils::window_mode_t::fullscreen)
            pWindowManager->setPixelFormat(config::Config::display.fullscreenRes, config::Config::display.colorDepth);
        else
            pWindowManager->setPixelFormat(config::Config::display.windowRes, config::Config::display.colorDepth);
    }
    catch (...)
    {
        if (pWindowManager != nullptr)
            delete pWindowManager;
        throw;
    }

    // disable screen-saver
    if (config::Config::events.isNoScreenSaver)
        utils::DisplayWindow::setScreenSaver(false);

    s_pWindowManager = pWindowManager; // unlock render()
}

/// @brief Close display window and restore menu
void Engine::closeDisplayWindow()
{
    utils::DisplayWindow* pWindowManager = s_pWindowManager;
    s_pWindowManager = nullptr; // lock render()

    // close API and window
    closeGL();
    pWindowManager->hide();
    delete pWindowManager;

    // re-enable screensaver (if disabled)
    if (config::Config::events.isNoScreenSaver)
        utils::DisplayWindow::setScreenSaver(true);
}

/// @brief Render current frame
void Engine::render()
{
    if (s_isInitialized == false)
    {
        if (s_pWindowManager != nullptr)
            initGL();
        return;
    }
    //...
}


// -- rendering API management -- --------------------------------------

/// @brief Initialize rendering API
/// @throws runtime_error API init failure
void Engine::initGL()
{
    if (s_isInitialized == true)
        return;

    //...

    loadPipeline();
    s_isInitialized = true;
}

/// @brief Cleanup and close rendering API
void Engine::closeGL()
{
    if (s_isInitialized == false)
        return;
    s_isInitialized = false;

    //...
}

/// @brief Load/reload rendering pipeline
void Engine::loadPipeline()
{

}
