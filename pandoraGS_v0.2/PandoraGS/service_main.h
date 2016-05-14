/*******************************************************************************
PANDORAGS project - PS1 GPU driver
------------------------------------------------------------------------
Author  :     Romain Vinders
License :     GPLv2
------------------------------------------------------------------------
File name :   service_main.h
Description : driver service interface (PSEmu)
*******************************************************************************/
#ifndef _SERVICE_MAIN_H
#define _SERVICE_MAIN_H
#include "globals.h"


// -- DRIVER INIT INTERFACE -- -------------------------------------------------

/// <summary>Driver init (called once)</summary>
/// <returns>Success indicator</returns>
long CALLBACK GPUinit();
/// <summary>Driver shutdown (called once)</summary>
/// <returns>Success indicator</returns>
long CALLBACK GPUshutdown();

#ifdef _WINDOWS
#define GPUopen_PARAM_ GPUopen(HWND hWindow)
#else
#define GPUopen_PARAM_ GPUopen(unsigned long* displayId,char* caption,char* configFile)
#endif
/// <summary>Driver opening/reopening (game started)</summary>
/// <param name="hWindow">Emulator main window handle</param>
/// <returns>Success indicator</returns>
long CALLBACK GPUopen_PARAM_;
/// <summary>Driver closed (game stopped)</summary>
/// <returns>Success indicator</returns>
long CALLBACK GPUclose();

/// <summary>Activity update (called every vsync)</summary>
void CALLBACK GPUupdateLace();


// -- PLUGIN DIALOG INTERFACE -- -----------------------------------------------

/// <summary>Plugin - Open the configuration dialog box</summary>
/// <returns>Success indicator</returns>
long CALLBACK GPUconfigure();
/// <summary>Plugin - Open the 'about' dialog box</summary>
void CALLBACK GPUabout();
/// <summary>Plugin - Load test</summary>
/// <returns>Success indicator (0 = ok, 1 = warning or -1 = error)</returns>
long CALLBACK GPUtest();


// -- PLUGIN LIBRARY INFO -- ---------------------------------------------------

/// <summary>Get PSemu library type</summary>
/// <returns>Library type (GPU)</returns>
unsigned long CALLBACK PSEgetLibType();
/// <summary>Get library title</summary>
/// <returns>Library name</returns>
char* CALLBACK PSEgetLibName();
/// <summary>Get library version</summary>
/// <returns>Full library version identifier</returns>
unsigned long CALLBACK PSEgetLibVersion();


// -- GETTERS - SETTERS -- -----------------------------------------------------

/// <summary>Set game executable ID (for config profiles associations)</summary>
/// <param name="pGameId">Newly started game identifier</param>
void CALLBACK GPUsetExeName(char* pGameId);
/// <summary>Enable/disable frame limit from emulator</summary>
/// <param name="option">Status (1 = limit / 0 = none)</param>
void CALLBACK GPUsetframelimit(unsigned long option);

/// <summary>Get PSemu transfer mode</summary>
/// <returns>Image transfer mode</returns>
long CALLBACK GPUgetMode();
/// <summary>Set PSemu transfer mode (deprecated)</summary>
/// <param name="gdataMode">Image transfer mode</param>
void CALLBACK GPUsetMode(unsigned long gdataMode);
/// <summary>Set special display flags</summary>
/// <param name="dwFlags">Display flags</param>
void CALLBACK GPUdisplayFlags(unsigned long dwFlags);
/// <summary>Set custom fixes from emulator</summary>
/// <param name="fixBits">Fixes (bits)</param>
void CALLBACK GPUsetfix(unsigned long fixBits);


// -- STATUS REGISTER CONTROL -- -----------------------------------------------

/// <summary>Read data from GPU status register</summary>
/// <returns>GPU status register data</returns>
unsigned long CALLBACK GPUreadStatus();
/// <summary>Process data sent to GPU status register</summary>
/// <param name="gdata">Status register command</param>
void CALLBACK GPUwriteStatus(unsigned long gdata);


// -- DATA TRANSFER INTERFACE -- -----------------------------------------------

/// <summary>Read data from video memory (vram)</summary>
/// <returns>Raw GPU data</returns>
unsigned long CALLBACK GPUreadData();
/// <summary>Process and send data to video data register</summary>
/// <param name="gdata">Written data</param>
void CALLBACK GPUwriteData(unsigned long gdata);

/// <summary>Give a direct core memory access chain to GPU driver</summary>
/// <param name="pDwBaseAddress">Pointer to memory chain</param>
/// <param name="offset">Memory offset</param>
/// <returns>Success indicator</returns>
long CALLBACK GPUdmaChain(unsigned long* pDwBaseAddress, unsigned long offset);


// -- SAVE-STATES - SNAPSHOTS -- -----------------------------------------------

#define GPUFREEZE_SUCCESS   1
#define GPUFREEZE_ERR       0

// Save-state structure
typedef struct GPUFREEZETAG
{
    unsigned long freezeVersion;    // system version = 1 (set by emulator)
    unsigned long status;           // current virtual GPU status
    unsigned long pControlReg[256]; // latest control register values
    unsigned char pPsxVram[1024*1024 * 2]; // current video memory image
} GPUFreeze_t;

/// <summary>Set custom fixes from emulator</summary>
/// <param name="dataMode">Transaction type (0 = setter / 1 = getter / 2 = slot selection)</param>
/// <param name="pMem">Save-state structure pointer (to read or write)</param>
/// <returns>Success/compatibility indicator</returns>
long CALLBACK GPUfreeze(unsigned long dataMode, GPUFreeze_t* pMem);

/// <summary>Request snapshot (on next display)</summary>
void CALLBACK GPUmakeSnapshot();
/// <summary>Get screen picture</summary>
/// <param name="pMem">allocated screen picture container 128x96 px (24b/px: 8-8-8 bit BGR, no header)</param>
void CALLBACK GPUgetScreenPic(unsigned char* pMem);
/// <summary>Store and display screen picture</summary>
/// <param name="pMem">screen picture data 128x96 px (24b/px: 8-8-8 bit BGR, no header)</param>
void CALLBACK GPUshowScreenPic(unsigned char* pmem);


// -- MISCELLANEOUS -- ---------------------------------------------------------

/// <summary>Display debug text</summary>
/// <param name="pText">Text string</param>
void CALLBACK GPUdisplayText(char* pText);
/// <summary>Set gun cursor display and position</summary>
/// <param name="player">Player with current cursor (0-7)</param>
/// <param name="x">Cursor horizontal position (0-511)</param>
/// <param name="y">Cursor vertical position (0-255)</param>
void CALLBACK GPUcursor(int player, int x, int y);
/// <summary>Trigger screen vibration</summary>
/// <param name="smallVbr">Small rumble value</param>
/// <param name="bigVbr">Big rumble value (if != 0, smallVbr will be ignored)</param>
void CALLBACK GPUvisualVibration(unsigned long smallVbr, unsigned long bigVbr);

#endif
