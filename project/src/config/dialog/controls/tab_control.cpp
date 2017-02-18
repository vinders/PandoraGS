/*******************************************************************************
PANDORAGS project - PS1 GPU driver
------------------------------------------------------------------------
Author  :     Romain Vinders
License :     GPLv2
------------------------------------------------------------------------
Description : advanced tab control
*******************************************************************************/
#include "../../../globals.h"
#if _DIALOGAPI == DIALOGAPI_WIN32
#include <Windows.h>
#include <windowsx.h>
#include <commctrl.h>
#endif
#include "../../../res/resource.h"
#include "common.h"
#include "dialog.h"
#include "tab_button.h"
#include "tab_page.h"
#include "tab_control.h"
using namespace config::dialog::controls;

#define DIALOG_BOTTOM_BAR_HEIGHT  42
#define COLOR_BORDER  RGB(210,216,220)
const float TabControl::colorTop[3] { 235.0f, 240.0f, 245.0f };
const float TabControl::colorOffset[3] { -79.0f, -51.0f, -25.0f };
const float TabControl::colorTopBorder[3] { 227.0f, 232.0f, 236.0f };
const float TabControl::colorOffsetBorder[3] { -70.0f, -36.0f, -21.0f };


/// @brief Create tab control in dialog
/// @param[in] window  Window handle
/// @param[in] offset  Vertical offset for tab control
/// @param[in] width   Tab button width
/// @returns Success
bool TabControl::create(window_handle_t window, const uint32_t offset, const uint32_t width)
{
    if (m_activePageIndex != -1)
        close();

    // create tab pages
    m_activePageIndex = 0;
    m_offsetTabY = offset;
    m_width = width;
    for (uint32_t i = 0; i < m_pages.size(); ++i)
    {
        if (m_pages.at(i).page->create(window, width) == false)
            return false;
        m_pages.at(i).page->setVisible(i == 0);
    }

    // create tab buttons
    for (uint32_t i = 0; i < m_pages.size(); ++i)
    {
        if (m_pages.at(i).button->create(window, offset, width) == false)
            return false;
    }
    return true;
}


/// @brief Close control in dialog
void TabControl::close()
{
    if (m_activePageIndex != -1 && m_pages.size() > 0)
    {
        for (uint32_t i = 0; i < m_pages.size(); ++i)
        {
            m_pages.at(i).page->close();
            m_pages.at(i).button->close();
        }
    }
    m_activePageIndex = -1;
}



// -- event handlers -- --------------------------------------------

/// @brief Language change event
/// @returns Validity
bool TabControl::onDialogConfirm(DIALOG_EVENT_HANDLER_ARGUMENTS)
{
    // check settings validity in pages
    for (uint32_t i = 0; i < m_pages.size(); ++i)
    {
        if (m_pages.at(i).page->onDialogConfirm(DIALOG_EVENT_HANDLER_ARGUMENTS_VALUES) == false)
            return false;
    }
    return true;
}

/// @brief Language change event
/// @param[in] instance  Library instance handle
void TabControl::onLanguageChange(DIALOG_EVENT_HANDLER_ARGUMENTS)
{
    // refresh tab buttons
    //...repaint tab buttons
    //...

    // translate components in pages
    for (uint32_t i = 0; i < m_pages.size(); ++i)
    {
        m_pages.at(i).page->onLanguageChange(true);
    }
}


/// @brief Trigger control drawing
DIALOG_EVENT_RETURN TabControl::onPaint(DIALOG_EVENT_HANDLER_ARGUMENTS)
#if _DIALOGAPI == DIALOGAPI_WIN32
{
    PAINTSTRUCT paint;
    HDC hDC = nullptr;
    hDC = BeginPaint(getEventWindowHandle(), &paint);
    if (hDC == nullptr)
        return DIALOG_EVENT_RETURN_ERROR;

    RECT windowRect;
    GetClientRect(getEventWindowHandle(), &windowRect); 

    // tab control bottom border
    HBRUSH borderBrush = CreateSolidBrush(COLOR_BORDER);
    if (borderBrush)
    {
        RECT borderRect;
        borderRect.left = windowRect.left;
        borderRect.right = windowRect.right;
        borderRect.bottom = windowRect.bottom - DIALOG_BOTTOM_BAR_HEIGHT;
        borderRect.top = borderRect.bottom - 1;
        FillRect(hDC, &borderRect, HBRUSH(GetStockObject(WHITE_BRUSH)));

        borderRect.top += 1;
        borderRect.bottom += 1;
        FillRect(hDC, &borderRect, borderBrush);

        DeleteObject(borderBrush);
        borderBrush = nullptr;
    }

    // set background gradient zone
    float curPercent;
    RECT gradientLine, gardientBorder;
    HBRUSH gradientBrush;
    windowRect.top += m_offsetTabY; // skip top offset
    windowRect.bottom -= (DIALOG_BOTTOM_BAR_HEIGHT + 1); // skip bottom bar + border
    m_height = windowRect.bottom - windowRect.top;

    // draw background gradient
    gradientLine.left = windowRect.left;
    gradientLine.right = gardientBorder.left = windowRect.left + m_width - 1;
    gardientBorder.right = gardientBorder.left + 1;

    for (int px = 0; px < m_height; ++px)
    {
        curPercent = static_cast<float>(px) / static_cast<float>(m_height);

        gradientLine.top = windowRect.top + px;
        gradientLine.bottom = gradientLine.top + 1;
        gradientBrush = CreateSolidBrush(RGB(colorTop[0] + (curPercent*colorOffset[0]),
                                             colorTop[1] + (curPercent*colorOffset[1]),
                                             colorTop[2] + (curPercent*colorOffset[2])));
        FillRect(hDC, &gradientLine, gradientBrush); // background
        DeleteObject(gradientBrush);

        if (px % 2 == 1) // 2 border pixels at once
        {
            gardientBorder.top = gradientLine.top - 1;
            gardientBorder.bottom = gradientLine.bottom ;
            borderBrush = CreateSolidBrush(RGB(colorTopBorder[0] + (curPercent*colorOffsetBorder[0]),
                                               colorTopBorder[1] + (curPercent*colorOffsetBorder[1]),
                                               colorTopBorder[2] + (curPercent*colorOffsetBorder[2])));
            FillRect(hDC, &gardientBorder, borderBrush); // border
            DeleteObject(borderBrush);
        }
    }

    EndPaint(hWindow, &paint);
    ReleaseDC(hWindow, hDC);
    return DIALOG_EVENT_RETURN_VALID;
}
#else
{
    //...
    return DIALOG_EVENT_RETURN_VALID;
}
#endif
