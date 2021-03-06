/*******************************************************************************
PANDORAGS project - PS1 GPU driver
------------------------------------------------------------------------
Author  :     Romain Vinders
License :     GPLv2
------------------------------------------------------------------------
File name :   dispatcher.h
Description : GPU manager and dispatcher
*******************************************************************************/
#ifndef _DISPATCHER_H
#define _DISPATCHER_H
#include "globals.h"
#include <cstddef>
#include <cstdint>
#include <string>
#include "video_memory.h"
#include "display_state.h"
#include "status_register.h"
#include "primitive_builder.h"
#include "system_tools.h"
#include "geometry.hpp"

// data types
typedef struct MEMLOAD // memory I/O
{
    loadmode_t mode;
    tile_t range;
    short  rowsRemaining;
    short  colsRemaining;
    VideoMemory::iterator vramPos;
    MEMLOAD()
    {
        mode = Loadmode_normal;
    }
} memoryload_t;
typedef struct GPUFREEZETAG // save-state structure
{
    unsigned long freezeVersion;    // system version = 1 (set by emulator)
    unsigned long status;           // current virtual GPU status
    unsigned long pControlReg[256]; // latest control register values
    unsigned char pPsxVram[1024*1024 * 2]; // current video memory image
} GPUFreeze_t;

// save-states
#define SAVESTATE_LOAD          0u
#define SAVESTATE_SAVE          1u
#define SAVESTATE_INFO          2u
#define GPUFREEZE_SUCCESS       1
#define GPUFREEZE_ERR           0
// control register
#define CTRLREG_SIZE            256
// commands
#define CMD_RESETGPU            0x00
#define CMD_TOGGLEDISPLAY       0x03
#define CMD_SETTRANSFERMODE     0x04
#define CMD_SETDISPLAYPOSITION  0x05
#define CMD_SETDISPLAYWIDTH     0x06
#define CMD_SETDISPLAYHEIGHT    0x07
#define CMD_SETDISPLAYINFO      0x08
#define CMD_GPUREQUESTINFO      0x10


// GPU manager and dispatcher
class Dispatcher
{
public: // treat PSEmu memory functions as member methods

    // video memory management
    static VideoMemory  mem_vram;               // emulated console video memory
    static memoryload_t mem_vramReader;         // output memory load
    static memoryload_t mem_vramWriter;         // input memory load
    static unsigned long mem_dataExchangeBuffer; // data buffer read/written by emulator

    // execution and display status
    static std::string s_gameId; // game executable ID
    static DisplayState st_displayState;
    static unsigned long st_pControlReg[CTRLREG_SIZE]; // GPU status control
    static uint32_t st_displayDevFlags; // 00 -> digital, 01 -> analog, 02 -> mouse, 03 -> gun
    static bool st_isFirstOpen;      // first call to GPUopen()
    static bool st_isUploadPending;  // image needs to be uploaded to VRAM
    static long st_selectedSaveSlot;  // selected save-state slot

    static bool s_isZincEmu;   // Zinc emulation


public:
    /// <summary>Initialize memory, status and dispatcher</summary>
    static inline void init()
    {
        // initialize status
        st_isFirstOpen = true;
        StatusRegister::init();
        // initialize control data
        st_displayState.init();
        mem_dataExchangeBuffer = GPUDATA_INIT;
        memset(st_pControlReg, 0x0, CTRLREG_SIZE * sizeof(unsigned long));
        st_displayDevFlags = 0u;
        PrimitiveBuilder::init();
        // initialize VRAM
        mem_vram.init(s_isZincEmu);
        memset(&mem_vramReader, 0x0, sizeof(memoryload_t)); // mode = Loadmode_normal = 0
        memset(&mem_vramWriter, 0x0, sizeof(memoryload_t)); // mode = Loadmode_normal = 0
        
        StatusRegister::setStatus(GPUSTATUS_IDLE | GPUSTATUS_READYFORCOMMANDS);
    }

    /// <summary>Reset GPU information</summary>
    static inline void reset()
    {
        StatusRegister::init();
        mem_vramReader.mode = Loadmode_normal;
        mem_vramWriter.mode = Loadmode_normal;
        st_displayState.reset();
    }

    /// <summary>Close memory</summary>
    static inline void close()
    {
        mem_vram.close();
    }

    /// <summary>Display data summary in debug window</summary>
    static void printDebugSummary();
    /// <summary>Export full status and VRAM data</summary>
    static void exportData();


    // -- SET SYNC/TRANSFER INFORMATION -- -----------------------------------------

    /// <summary>Set VRAM data transfer mode</summary>
    /// <param name="gdata">Status command</param>
    static inline void setDataTransferMode(ubuffer_t gdata)
    {
        gdata &= 0x3u; // extract last 2 bits (LSB+1, LSB)
        mem_vramWriter.mode = (gdata == 0x2) ? Loadmode_vramTransfer : Loadmode_normal;
        mem_vramReader.mode = (gdata == 0x3) ? Loadmode_vramTransfer : Loadmode_normal;
        // set direct memory access bits
        StatusRegister::unsetStatus(GPUSTATUS_DMABITS); // clear current DMA settings
        StatusRegister::setStatus(gdata << 29); // set DMA (MSB-1, MSB-2)
    }

    /// <summary>Initialize time mode and frame rate</summary>
    static inline void initFrameRate()
    {
        Timer::setTimeMode((timemode_t)Config::sync_timeMode);
        Timer::setFrequency(Config::sync_framerateLimit, Regionsync_undefined, StatusRegister::getStatus(GPUSTATUS_INTERLACED));
    }
    /// <summary>Set frame rate with region info</summary>
    static inline void setFrameRate()
    {
        st_displayState.setFrameRate();
    }


    // - OTHER SETTERS - -----------------------------------------------------------

    /// <summary>Set game executable identifier</summary>
    /// <param name="pGameId">Game ID</param>
    static inline void setGameId(char* pGameId)
    {
        s_gameId = (pGameId != NULL) ? std::string(pGameId) : std::string("");
    }
    /// <summary>Get game executable identifier</summary>
    /// <returns>Game ID</returns>
    static inline std::string getGameId()
    {
        return s_gameId;
    }

    
    // -- COMMAND EXTRACTION -- ----------------------------------------------------

    /// <summary>Extract display command type ID from raw data</summary>
    /// <param name="raw">Display bits</param>
    /// <returns>Command identifier</returns>
    static inline ubuffer_t extractGpuCommandType(ubuffer_t raw)
    {
        return ((raw >> 24) & 0x0FFu); // extract bits 25 and 26
    }
    /// <summary>Extract display position (X) from raw data</summary>
    /// <param name="raw">Display bits</param>
    /// <returns>Coordinates</returns>
    static inline point_t extractPos(ubuffer_t raw)
    {
        uint32_t x = (raw & 0x7FFu);
        uint32_t y = ((raw >> 12) & 0xFFFu);
        return point_t((short)x, (short)y);
    }
    /// <summary>Extract small display position (Y) from raw data</summary>
    /// <param name="raw">Display bits</param>
    /// <param name="isZincSupport">Zinc emu support enabled</param>
    /// <returns>Coordinates</returns>
    static inline point_t extractSmallPos(ubuffer_t raw, bool isZincSupport)
    {
        uint32_t x = (raw & 0x3FFu);
        uint32_t y = ((raw >> 10) & 0x3FFu);
        if (isZincSupport && mem_vram.isDoubledSize() && st_displayState.version() == 2)
            y = ((raw >> 12) & 0x3FFu);
        return point_t((short)x, (short)y);
    }
};

// -- DISPLAY STATUS CONTROL -- ------------------------------------------------

/// <summary>Read data from GPU status register</summary>
/// <returns>GPU status register data</returns>
unsigned long CALLBACK GPUreadStatus();
/// <summary>Process data sent to GPU status register</summary>
/// <param name="gdata">Status register command</param>
void CALLBACK GPUwriteStatus(unsigned long gdata);

/// <summary>Set special display flags</summary>
/// <param name="dwFlags">Display flags</param>
void CALLBACK GPUdisplayFlags(unsigned long dwFlags);
/// <summary>Set game executable ID (for config profiles associations)</summary>
/// <param name="pGameId">Newly started game identifier</param>
void CALLBACK GPUsetExeName(char* pGameId);


// -- DATA TRANSFER -- ---------------------------------------------------------

/// <summary>Get data transfer mode</summary>
/// <returns>Image transfer mode</returns>
long CALLBACK GPUgetMode();
/// <summary>Set data transfer mode (deprecated)</summary>
/// <param name="gdataMode">Image transfer mode</param>
void CALLBACK GPUsetMode(unsigned long gdataMode);

/// <summary>Read data from video memory (vram)</summary>
/// <returns>Raw GPU data</returns>
unsigned long CALLBACK GPUreadData();
/// <summary>Process and send data to video data register</summary>
/// <param name="gdata">Written data</param>
void CALLBACK GPUwriteData(unsigned long gdata);

/// <summary>Read entire chunk of data from video memory (vram)</summary>
/// <param name="pDwMem">Pointer to chunk of data (destination)</param>
/// <param name="size">Memory chunk size</param>
void CALLBACK GPUreadDataMem(unsigned long* pDwMem, int size);
/// <summary>Process and send chunk of data to video data register</summary>
/// <param name="pDwMem">Pointer to chunk of data (source)</param>
/// <param name="size">Memory chunk size</param>
void CALLBACK GPUwriteDataMem(unsigned long* pDwMem, int size);
/// <summary>Direct core memory chain transfer to GPU driver</summary>
/// <param name="pDwBaseAddress">Pointer to memory chain</param>
/// <param name="offset">Memory offset</param>
/// <returns>Success indicator</returns>
long CALLBACK GPUdmaChain(unsigned long* pDwBaseAddress, unsigned long offset);


// -- LOAD/SAVE MEMORY STATE -- ------------------------------------------------

/// <summary>Save/load current state</summary>
/// <param name="dataMode">Transaction type (0 = setter / 1 = getter / 2 = slot selection)</param>
/// <param name="pMem">Save-state structure pointer (to read or write)</param>
/// <returns>Success/compatibility indicator</returns>
long CALLBACK GPUfreeze(unsigned long dataMode, GPUFreeze_t* pMem);

#endif
