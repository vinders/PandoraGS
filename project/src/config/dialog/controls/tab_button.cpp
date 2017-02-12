/*******************************************************************************
PANDORAGS project - PS1 GPU driver
------------------------------------------------------------------------
Author  :     Romain Vinders
License :     GPLv2
------------------------------------------------------------------------
Description : tab button for tab control
*******************************************************************************/
#include "../../../globals.h"
#include <cstdint>
#include <string>
#include <map>
#if _DIALOGAPI == DIALOGAPI_WIN32
#include <Windows.h>
#include <windowsx.h>
#include <commctrl.h>
#endif
#include "../../../res/resource.h"
#include "tab_button.h"
using namespace config::dialog::controls;
using namespace std::literals::string_literals;

#define TAB_INTERVAL      6
#define TAB_HEIGHT        90
#define TAB_TEXT_OFFSET   64

int32_t TabButton::s_activeCount = 0; ///< Number of tab buttons loaded
std::map<uint32_t, bitmap_cache_t> TabButton::s_cache; ///< Icon bitmap cache
#if _DIALOGAPI == DIALOGAPI_WIN32
HFONT TabButton::s_tabFont = nullptr; ///< Common font for tab buttons
#endif


/// @brief Create tab button
/// @param[in] instance       Current instance handle
/// @param[in] title          Tab name
/// @param[in] resourceId     Tab button resource identifier
/// @param[in] bmpResourceId  Bitmap with icons (resource ID)
/// @param[in] icon           Icon index (0 = none)
/// @param[in] iconSize       Icon size (pixels)
TabButton::TabButton(library_instance_t instance, std::wstring& title, const uint32_t resourceId, 
                     const uint32_t bmpResourceId, const uint32_t icon, const uint32_t iconSize = 48u)
    : m_instance(instance), m_title(title), m_resourceId(resourceId), m_bitmapId(bmpResourceId), m_tabButtonControl(nullptr)
{
    m_tabNumber = s_activeCount;
    ++s_activeCount;
    m_iconIndex = icon;

    // load bitmap with icon
    if (!s_cache.count(bmpResourceId))
    {
        s_cache[bmpResourceId] = bitmap_cache_t();
        s_cache[bmpResourceId].iconSize = iconSize;
        s_cache[bmpResourceId].maxIndex = icon;
        s_cache[bmpResourceId].isAlphaChannelSet = false;
        s_cache[bmpResourceId].bitmapData = (HBITMAP)LoadBitmap(instance, MAKEINTRESOURCE(IDB_CONFIG_ICONS));
        if (s_cache[bmpResourceId].bitmapData == nullptr)
        {
            s_cache.erase(bmpResourceId);
            m_iconIndex = 0;
        }
    }
    // bitmap already loaded -> update max index
    else if (static_cast<int32_t>(icon) > s_cache[bmpResourceId].maxIndex)
    {
        s_cache[bmpResourceId].maxIndex = icon;
    }
}

/// @brief Destrot tab button
TabButton::~TabButton()
{
    // free resources
    --s_activeCount;
    #if _DIALOGAPI == DIALOGAPI_WIN32
    if (m_tabButtonControl != nullptr)
        DestroyWindow(m_tabButtonControl);
    #endif

    if (s_activeCount <= 0u)
    {
        // clear cache if no tab button left
        s_activeCount = 0u;
        for (auto it = s_cache.begin(); it != s_cache.end(); ++it)
        {
            #if _DIALOGAPI == DIALOGAPI_WIN32
            if (it->second.bitmapData != nullptr) 
                DeleteObject(it->second.bitmapData);
            #endif
        }
        s_cache.clear();

        // remove font
        #if _DIALOGAPI == DIALOGAPI_WIN32
        if (s_tabFont != nullptr)
        {
            DeleteObject(s_tabFont);
            s_tabFont = nullptr;
        }
        #endif
    }
}


/// @brief Create control in dialog
/// @param[in] window  Window handle
/// @param[in] offset      Vertical offset for first tab button
/// @param[in] width       Tab button width
/// @returns Success
bool TabButton::create(window_handle_t window, const uint32_t offset, const uint32_t width)
#if _DIALOGAPI == DIALOGAPI_WIN32
{
    // create tab button
    std::wstring controlName = L"Tab "s + std::to_wstring(m_tabNumber);
    m_tabButtonControl = CreateWindow(L"button", controlName.c_str(), WS_CHILD | WS_VISIBLE | BS_TEXT | BS_BITMAP | BS_BOTTOM | BS_OWNERDRAW,
                                      4, offset + (m_tabNumber * (TAB_HEIGHT + TAB_INTERVAL)), width - 4, TAB_HEIGHT,
                                      static_cast<HWND>(window), (HMENU)m_resourceId, (HINSTANCE)m_instance, NULL);
    if (!m_tabButtonControl)
        return false;

    HDC hDC = GetDC(static_cast<HWND>(window));
    if (hDC)
    {
        // set icon bitmap transparency
        if (m_iconIndex > 0u && s_cache[m_bitmapId].isAlphaChannelSet == false)
        {
            s_cache[m_bitmapId].isAlphaChannelSet = true;
            setBitmapAlphaChannel(hDC, s_cache[m_bitmapId].bitmapData, s_cache[m_bitmapId].maxIndex *s_cache[m_bitmapId].iconSize, s_cache[m_bitmapId].iconSize);
        }

        // load font
        if (s_tabFont == nullptr)
        {
            LOGFONT logFont = { 0 };
            logFont.lfHeight = -MulDiv(10, GetDeviceCaps(hDC, LOGPIXELSY), 72);
            logFont.lfWeight = FW_NORMAL;
            //logFont.lfQuality = CLEARTYPE_QUALITY;
            wcscpy_s(logFont.lfFaceName, L"Tahoma");
            s_tabFont = CreateFontIndirect(&logFont);
        }
        // set font
        ReleaseDC(static_cast<HWND>(window), hDC);
        SendMessage(m_tabButtonControl, WM_SETFONT, (WPARAM)s_tabFont, (LPARAM)MAKELONG(TRUE, 0));
    }
    return true;
}
#else
{
    //...
    return true;
}
#endif


/// @brief Trigger control drawing
/// @param[in] args  Event arguments
void TabButton::paint(paint_control_event_args_t& args)
#if _DIALOGAPI == DIALOGAPI_WIN32
{
    // draw tab background with icon
    if (m_iconIndex > 0u)
    {
        //...
    }

    // draw tab text
    //...
}
#else
{
    //...
}
#endif



#if _DIALOGAPI == DIALOGAPI_WIN32
/// @brief Prepare bitmap with alpha channel (set transparency)
/// @param hDC     Device context handle
/// @param img     Bitmap handle
/// @param width   Bitmap width (pixels)
/// @param height  Bitmap height (pixels)
void TabButton::setBitmapAlphaChannel(HDC hDC, HBITMAP img, LONG width, LONG height)
{
    if (img)
    {
        // set bitmap meta-data
        BITMAPINFO bmi;
        ZeroMemory(&bmi, sizeof(BITMAPINFO));
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = width;
        bmi.bmiHeader.biHeight = height;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;
        bmi.bmiHeader.biSizeImage = bmi.bmiHeader.biWidth * bmi.bmiHeader.biHeight * 4;

        // multiply alpha channel
        BITMAP bm = { 0 };
        GetObject(img, sizeof(bm), &bm);
        GetDIBits(hDC, img, 0, bm.bmHeight, NULL, &bmi, DIB_RGB_COLORS); // add real dimensions to bmi
        LPBYTE pBitData = (LPBYTE)LocalAlloc(LPTR, bm.bmWidth * bm.bmHeight * sizeof(DWORD));
        if (pBitData != NULL)
        {
            LPBYTE pData = pBitData;
            GetDIBits(hDC, img, 0, bm.bmHeight, pData, &bmi, DIB_RGB_COLORS); // copy image to array
            for (int y = 0; y < bm.bmHeight; y++)
            {
                for (int x = 0; x < bm.bmWidth; x++)
                {
                    pData[0] = (BYTE)((DWORD)pData[0] * pData[3] / 255); // fix for AlphaBlend
                    pData[1] = (BYTE)((DWORD)pData[1] * pData[3] / 255);
                    pData[2] = (BYTE)((DWORD)pData[2] * pData[3] / 255);
                    pData += 4;
                }
            }
            SetDIBits(hDC, img, 0, bm.bmHeight, pBitData, &bmi, DIB_RGB_COLORS); // copy array to image
            LocalFree(pBitData);
        }
    }
}
#endif
