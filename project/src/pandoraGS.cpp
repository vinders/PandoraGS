/*******************************************************************************
PANDORAGS project - PS1 GPU driver
------------------------------------------------------------------------
Author  :     Romain Vinders
License :     GPLv2
------------------------------------------------------------------------
Description : GPU library entry point
*******************************************************************************/
#ifdef _WINDOWS
#include "globals.h"
#include <cstdlib>
#include <Windows.h>
#include <tchar.h>
#include "pandoraGS.h"
using namespace std;

HINSTANCE PandoraGS::s_hInstance = NULL; ///< Executed instance handle


/// @brief Main library entry point
/// @param[in] hModule    Module instance
/// @param[in] reason     Library call flag
/// @param[in] pReserved  Initialization and cleanup settings
/// @returns   Success indicator
BOOL APIENTRY DllMain(HANDLE hModule, DWORD reason, LPVOID pReserved)
{
	switch (reason)
	{
		case DLL_PROCESS_DETACH: // detach
		{
			HANDLE hEmptyModule = nullptr;
			PandoraGS::setInstance(&hEmptyModule);
		}
		default: // attach
		{
			PandoraGS::setInstance(&hModule);
			break;
		}
	}
    return TRUE;
}

/// @brief Initialize instance handle
/// @param[in] hModule  Module instance handle
void PandoraGS::setInstance(const HANDLE* hModule) noexcept
{
    s_hInstance = static_cast<HINSTANCE>(*hModule);
}
#endif
