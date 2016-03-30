/*******************************************************************************
PANDORAGS project - PS1 GPU driver
------------------------------------------------------------------------
Author  :     Romain Vinders
License :     GPLv2
------------------------------------------------------------------------
File name :   psx_core_memory.h
Description : playstation virtual video memory unit
*******************************************************************************/
#ifndef _PSX_CORE_MEMORY
#define _PSX_CORE_MEMORY
#include "globals.h"

// -- GENERAL INFORMATION DATA -- ----------------------------------------------

// localization
enum LocalizationMode
{
    LocalizationMode_Ntsc = 0,
    LocalizationMode_Pal = 1
};

// geometry point (dword)
typedef struct LPOINTTAG
{
    long x;
    long y;
} Point_t;
// geometry point (word)
typedef struct HPOINTTAG
{
    short x;
    short y;
} WPoint_t;
// geometry rectangle (word)
typedef struct PSXRECTTAG
{
    short top; // X start
    short btm; // X end
    short lft; // Y start
    short rgt; // Y end
} WRect_t;


// -- PSX MEMORY DATA -- -------------------------------------------------------

// PSX VRAM memory image
typedef struct PSX_VRAM
{
    unsigned char*  pData;    // allocated memory image
    unsigned char*  pByte;    // 8 bits mode access pointer
    unsigned short* pWord;    // 16 bits mode access pointer
    unsigned long*  pDword;   // 32 bits mode access pointer
    unsigned short* pEOM;     // end of memory

    unsigned long   bufferSize;// single vram buffer size
    unsigned long   length;   // total memory length
    int             oddFrame; // current frame has 'odd' lines
} PsxVram_t;
// PSX VRAM load indicator
typedef struct VRAM_LOADTAG
{
    short x;
    short y;
    short width;
    short height;
    short rowsRemaining;
    short colsRemaining;
    unsigned short *pVramImage;
} VramLoad_t;
// VRAM size (kilobytes)
#define PSXVRAM_SIZE          512
#define ZINCVRAM_SIZE         1024
#define PSXVRAM_SECURE_EXTRA  1024
#define PSXVRAM_SECURE_OFFSET 512
// array size
#define STATUSCTRL_SIZE       256
#define DRAWINFO_SIZE         16
// byte access masks
#define DUAL_BYTE_MASK        0xFFFF
#define TRI_BYTE_MASK         0xFFFFFF
#define PSXVRAM_MASK          0x1FFFFC // 2097148

// transfer modes
#define DR_NORMAL                   0
#define DR_VRAMTRANSFER             1
// status bits masks
#define GPUSTATUS_INIT              0x14802000 // initial values
#define GPUSTATUS_ODDLINES          0x80000000
#define GPUSTATUS_DMABITS           0x60000000 // 2 bits
#define GPUSTATUS_READYFORCOMMANDS  0x10000000
#define GPUSTATUS_READYFORVRAM      0x08000000
#define GPUSTATUS_IDLE              0x04000000
#define GPUSTATUS_DISPLAYDISABLED   0x00800000
#define GPUSTATUS_INTERLACED        0x00400000
#define GPUSTATUS_RGB24             0x00200000
#define GPUSTATUS_PAL               0x00100000
#define GPUSTATUS_DOUBLEHEIGHT      0x00080000
#define GPUSTATUS_WIDTHBITS         0x00070000 // 3 bits
#define GPUSTATUS_MASKENABLED       0x00001000
#define GPUSTATUS_MASKDRAWN         0x00000800
#define GPUSTATUS_DRAWINGALLOWED    0x00000400
#define GPUSTATUS_DITHER            0x00000200


// -- PSX CORE MEMORY CLASS -- -------------------------------------------------

// Playstation virtual video memory unit
class PsxCoreMemory
{
public:
    #ifdef _WINDOWS
    HWND gen_hWindow; // main window handle
    #endif
    // frame sync config
    LocalizationMode sync_localize;       // localization (NTSC/PAL)
    bool             sync_isInterlaced;   // interlacing (on/off)

    // psx emulated memory
    PsxVram_t     mem_vramImage;          // PSX VRAM memory image
    VramLoad_t    mem_vramRead;           // PSX VRAM frame reader
    int           mem_vramReadMode;       // read transfer mode
    VramLoad_t    mem_vramWrite;          // PSX VRAM frame writer
    int           mem_vramWriteMode;      // write transfer mode
    long          mem_gpuDataTransaction; // GPU data read/written by emulator

    // gpu emulated status and information
    long          st_statusReg;           // GPU status register
    unsigned long st_pStatusControl[STATUSCTRL_SIZE]; // GPU status control
    unsigned long st_pGpuDrawInfo[DRAWINFO_SIZE];     // GPU draw information


public:
    /// <summary>Create empty instance</summary>
    PsxCoreMemory();
    /// <summary>Destroy memory instance</summary>
    ~PsxCoreMemory();

    /// <summary>Initialize memory instance values</summary>
    void initMemory();

    /// <summary>Activate specific status bit(s) in status register</summary>
    /// <param name="statusBits">Bits mask to set</param>
    void setStatus(long statusBits)
    {
        st_statusReg |= statusBits;
    }
    /// <summary>Remove specific status bit(s) in status register</summary>
    /// <param name="statusBits">Bits mask to remove</param>
    void unsetStatus(long statusBits)
    {
        st_statusReg &= ~statusBits;
    }
    /// <summary>Check if status bit is active in status register</summary>
    /// <param name="statusBits">Bit(s) mask (will return true if at least one is active)</param>
    bool getStatus(long statusBit)
    {
        return ((st_statusReg & statusBit) != 0);
    }
    /// <summary>Get specific status bit(s) from status register</summary>
    /// <param name="statusBits">Bits mask</param>
    long getStatusBits(long statusBits)
    {
        return (st_statusReg & statusBits);
    }


    /*
    ////////////////////////////////////////////////////////////////////////
    // memory image of the PSX vram
    ////////////////////////////////////////////////////////////////////////

                unsigned char  *psxVSecure; //mem_vramImage
                unsigned char  *psxVub;
                signed   char  *psxVsb;
                unsigned short *psxVuw;
                unsigned short *psxVuw_eom;
                signed   short *psxVsw;
                unsigned long  *psxVul;
                signed   long  *psxVsl;

    //GLfloat         gl_z = 0.0f;
    BOOL            bNeedInterlaceUpdate = FALSE;
    BOOL            bNeedRGB24Update = FALSE;
    BOOL            bChangeWinMode = FALSE;

    #ifdef _WINDOWS
    //extern HGLRC    GLCONTEXT;
    #endif

                unsigned long   ulStatusControl[256]; //mem_pStatusControl

    ////////////////////////////////////////////////////////////////////////
    // global GPU vars
    ////////////////////////////////////////////////////////////////////////

                static long     GPUdataRet;     //mem_gpuDataTransaction
                long            lGPUstatusRet;  //mem_statusReg
            char            szDispBuf[64];  //inutile, ne servait qu'� stocker les FPS sous forme de chaine

            static unsigned long gpuDataM[256]; // uniquement dans 1 seule fonction -> utiliser var statique
            static unsigned char gpuCommand = 0;// idem
            static long          gpuDataC = 0;  // idem
            static long          gpuDataP = 0;  // idem

                VRAMLoad_t      VRAMWrite;      //mem_vramWrite
                VRAMLoad_t      VRAMRead;       //mem_vramRead
                int             iDataWriteMode; //mem_vramWriteMode
                int             iDataReadMode;  //mem_vramReadMode

    long            lClearOnSwap;
    long            lClearOnSwapColor;
    BOOL            bSkipNextFrame = FALSE;
    int             iColDepth;
    BOOL            bChangeRes;
            BOOL            bWindowMode; // fait double emploi avec isFullscreen de la config
    int             iWinSize;

    // possible psx display widths
    short dispWidths[8] = { 256, 320, 512, 640, 368, 384, 512, 640 };

    PSXDisplay_t    PSXDisplay;
    PSXDisplay_t    PreviousPSXDisplay;
    TWin_t          TWin;
    short           imageX0, imageX1;
    short           imageY0, imageY1;
    BOOL            bDisplayNotSet = TRUE;
    GLuint          uiScanLine = 0;
    int             iUseScanLines = 0;
    long            lSelectedSlot = 0;
    unsigned char * pGfxCardScreen = 0;
    int             iBlurBuffer = 0;
    int             iScanBlend = 0;
    int             iRenderFVR = 0;
    int             iNoScreenSaver = 0;
                unsigned long   ulGPUInfoVals[16];
    int             iFakePrimBusy = 0;
    int             iRumbleVal = 0;
    int             iRumbleTime = 0;
    */

};




// packet information access (read commands)
#define getGpuCommand(DATA)    ((DATA>>24)&0x0FF) // get only bits 25 and 26
// commands
#define CMD_RESETGPU            0x00
#define CMD_TOGGLEDISPLAY       0x03
#define CMD_SETTRANSFERMODE     0x04
#define CMD_SETDISPLAYPOSITION  0x05
#define CMD_SETDISPLAYWIDTH     0x06
#define CMD_SETDISPLAYHEIGHT    0x07
#define CMD_SETDISPLAYINFO      0x08
#define CMD_GPUREQUEST          0x10
// request codes
#define REQ_TW                  0x02
#define REQ_DRAWSTART           0x03
#define REQ_DRAWEND             0x04
#define REQ_DRAWOFFSET1         0x05
#define REQ_DRAWOFFSET2         0x06
#define REQ_GPUTYPE             0x07
#define REQ_BIOSADDR1           0x08
#define REQ_BIOSADDR2           0x0F
#define SAVESTATE_SET           0
#define SAVESTATE_GET           1
#define SAVESTATE_INFO          2
// request replies
#define GPUBIOSADDR             0xBFC03720
#define INFO_TW                 0
#define INFO_DRAWSTART          1
#define INFO_DRAWEND            2
#define INFO_DRAWOFF            3

#endif
