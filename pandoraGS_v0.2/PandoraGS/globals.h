/*******************************************************************************
PANDORAGS project - PS1 GPU driver
------------------------------------------------------------------------
Authors :     Romain Vinders (globals), Vision Thing (PSemu), Microsoft
License :     GPLv2
------------------------------------------------------------------------
File name :   globals.h
Description : global constants and definitions + PSEmu Pro specification
*******************************************************************************/
#ifndef _GLOBALS_H
#define _GLOBALS_H

// plugin information
#define PLUGIN_VERSIONMAJ  0uL
#define PLUGIN_VERSIONMIN  2uL
#define PLUGIN_VERSIONSTR  "0.2"
#define PLUGIN_NAME        "PandoraGS Driver"
#define PLUGIN_INFO        "PandoraGS GPU Driver 0.2\nBy Romain Vinders"
#define PLUGIN_DESCRIPTION "GPU plugin for PSEmu compatible emulator"
#define PLUGIN_AUTHOR      "Romain Vinders"
#define PLUGIN_DATE        "2016"
#define PLUGIN_LANGUAGES   "English, Spanish, French, German"
#define PLUGIN_TECH        "C++11 (runtimes 2015), "
#define PLUGIN_API_OPENGL  "OpenGL 4.1"
#define PLUGIN_API_DIRECTX "DirectX 11.0"

// other informations
#define PLUGIN_ABOUT_TITLE "About PandoraGS..."
#ifdef _WINDOWS
#define PLUGIN_THANKS      "Tapeq - for providing useful docs\r\nPete and iCatButler - for sharing public sources\
 \r\nCalb & ePSXe - for adding exe-name support\r\nAsmodean - for his awesome shaders\r\nThe OpenGL SuperBible book"
#else
#define PLUGIN_THANKS      "Tapeq - for providing useful docs\nPete and iCatButler - for sharing public sources\
 \nCalb & ePSXe - for adding exe-name support\nAsmodean - for his awesome shaders\nThe OpenGL SuperBible book"
#endif

// compilation settings
#define DIALOGAPI_WIN32    1
#define DIALOGAPI_QT       2
#ifdef _WINDOWS
#define _DIALOGAPI DIALOGAPI_WIN32
#else
#define _DIALOGAPI DIALOGAPI_QT // cross-platform
#endif
#define RENDERAPI_OPENGL   1
#define RENDERAPI_DIRECTX  2
#ifdef _WINDOWS
#define _RENDERAPI RENDERAPI_OPENGL
#else
#define _RENDERAPI RENDERAPI_OPENGL // cross-platform
#endif


// -- PSEMU SPECIFICATION -- ---------------------------------------------------
/*  PSEmu Plugin Developer Kit Header definition - (C)1998 Vision Thing
This file can be used only to develop PSEmu Plugins. Other usage is highly prohibited.
*/
#define _PPDK_HEADER_VERSION		1uL   //PSEmu 1.x

// plugin type returned by PSEgetLibType (types can be merged)
#define PSE_LT_CDR					1L
#define PSE_LT_GPU					2L
#define PSE_LT_SPU					4L
#define PSE_LT_PAD					8L

// every function in DLL if completed sucessfully should return this value
#define PSE_SUCCESS				    0L
// undefined error but fatal one, that kills all functionality
#define PSE_ERR_FATAL				-1L

// initialization went OK
#define PSE_INIT_SUCCESS		    0L
// this driver is not configured
#define PSE_INIT_ERR_NOTCONFIGURED	-2L
// this driver can not operate properly on this hardware or hardware is not detected
#define PSE_INIT_ERR_NOHARDWARE		-3L

//  GPU_Test return values
// success, everything configured, and went OK.
#define PSE_GPU_SUCCESS			    0L
// this driver is not configured
#define PSE_GPU_ERR_NOTCONFIGURED	1L
// gpuQueryS.flags
// this driver requests windowed mode,
#define PSE_GPU_FLAGS_WINDOWED		1L
// gpuQueryS.status
// this driver cannot operate in this windowed mode
#define PSE_GPU_STATUS_WINDOWWRONG	1L

//	CDR_Test return values
// success, everything configured, and went OK.
#define PSE_CDR_SUCCESS             0L
// ERRORS
#define PSE_CDR_ERR                 -40L
// this driver is not configured
#define PSE_CDR_ERR_NOTCONFIGURED	PSE_CDR_ERR - 0
// if this driver is unable to read data from medium
#define PSE_CDR_ERR_NOREAD			PSE_CDR_ERR - 1
// WARNINGS
#define PSE_CDR_WARN                40L
// if this driver emulates lame mode ie. can read only 2048 tracks and sector header is emulated
// this might happen to CDROMS that do not support RAW mode reading - surelly it will kill many games
#define PSE_CDR_WARN_LAMECD			PSE_CDR_WARN + 0


// -- SYSTEM COMPATIBILITY -- --------------------------------------------------

#ifdef _WINDOWS
// manifest
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
// exclusions
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN   // exclude rarely-used items from Windows headers
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN  // exclude rarely-used items from Windows headers
#endif
#ifndef _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE
#endif

// Windows inclusions
#include "pandoraGS_targetver.h"
#include <Windows.h>
#include <tchar.h>

// functions / macros
#define snprintf _snprintf
#define pthread_mutex_t HANDLE

// win32 resources identifiers
#include "resource.h"

#else 
// LINUX-UNIX
// types compatibility
#define FALSE 0
#define TRUE  1
#define BOOL  unsigned short
#define DWORD unsigned long

// functions / macros
#define LOWORD(l)   ((unsigned short)(l))
#define HIWORD(l)   ((unsigned short)(((unsigned long)(l) >> 16) & 0xFFFF))
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))
#endif

#ifndef CALLBACK
#define CALLBACK __stdcall
#endif


// -- COMMON INCLUSIONS -- -----------------------------------------------------

#include <stdint.h>
#include <inttypes.h>

#endif
