#ifndef _PRIMITIVE_ATTR_H
#define _PRIMITIVE_ATTR_H

#include "primitive_common.h"

namespace Primitive
{
    // -- data types - read raw data -- ----------------------------------------

    // Draw mode / Texture page change
    typedef struct DR_TEXPAGE
    {
        unsigned long raw;
        // attributes
        inline unsigned long x() { return ((raw << 6) & 0x3C0uL); }    // Texture page X base: 0, 64, ...
        inline unsigned long y() { return ((raw << 4) & 0x100uL); }    // Texture page Y base: 0 or 256
        inline bool isXFlip()    { return ((raw & 0x1000uL) != 0uL); } // Textured rectangle X-flip
        inline bool isYFlip()    { return ((raw & 0x2000uL) != 0uL); } // Textured rectangle Y-flip
        inline bool isDithered() { return ((raw & 0x200uL) != 0uL); }  // Dither 24bit to 15bit (0 = off (strip LSBs) ; 1 = enabled)
        inline bool isDrawingAllowed()  { return ((raw & 0x400uL) != 0uL); }  // Drawing to display area (0 = forbidden ; 1 = allowed)
        inline bool isTextureDisabled() { return ((raw & 0x800uL) != 0uL); }  // Texture mode - normal (0) or disable (1) if texture disabling allowed (GP1(09h).bit0 == 1)
        inline colordepth_t colorDepth() // Color depth mode (4-bit, 8-bit, 15-bit)
        {
            unsigned long val = ((unsigned long)(raw >> 7) & 0x3uL);
            return (colordepth_t)val;
        }
        inline stp_t semiTransparency()  // Semi-transparency mode
        {
            unsigned long val = ((unsigned long)(raw >> 5) & 0x3uL);
            return (stp_t)val;
        }
        // length
        static inline long size() { return 1; }
    } attr_texpage_t;

    // Texture window change
    typedef struct DR_TEXWIN
    {
        unsigned long raw;
        // attributes
        inline unsigned long maskX()   { return (raw & 0x1FuL); }         // Mask X (8 pixel steps) = manipulated bits
        inline unsigned long maskY()   { return ((raw >> 5) & 0x1FuL); }  // Mask Y (8 pixel steps)
        inline unsigned long offsetX() { return ((raw >> 10) & 0x1FuL); } // Offset X (8 pixel steps) = value for these bits
        inline unsigned long offsetY() { return ((raw >> 15) & 0x1FuL); } // Offset Y (8 pixel steps)
        // texcoord = (texcoord AND (NOT (mask*8))) OR ((offset AND mask)*8)
        // Area within texture window is repeated throughout the texture page (repeats not stored, but "read" as if present)
        static inline long size() { return 1; } // length
    } attr_texwin_t;

    // Drawing area change
    typedef struct DR_AREA
    {
        unsigned long raw;
        // attributes
        inline unsigned long x() { return (raw & 0x3FFuL); }         // X coordinate
        inline unsigned long y() { return ((raw >> 10) & 0x3FFuL); } // Y coordinate (must be framebuffer height max (e.g. 512) -> check it before using it)
        // length
        static inline long size() { return 1; }
    } attr_drawarea_t;
    // Drawing offset modification
    typedef struct DR_OFFSET
    {
        unsigned long raw;
        // attributes
        inline unsigned long x() { return (raw & 0x7FFuL); }         // X coordinate
        inline unsigned long y() { return ((raw >> 11) & 0x7FFuL); } // Y coordinate
        // length
        static inline long size() { return 1; }
    } attr_drawoffset_t;

    // Semi-transparency bit change
    typedef struct DR_STPMASK
    {
        unsigned long raw;
        // attributes
        inline bool isMaskBitForced() { return ((raw & 0x1uL) != 0uL); }  // Set mask while drawing (0 = texture with bit 15 ; 1 = force bit15=1)
        // When bit0 is off, the upper bit of data written to framebuffer is equal to bit15 of the texture color 
          // (it is set for colors that are marked as "semi-transparent"). For untextured polygons, bit15 is set to zero.
        inline bool isMaskBitChecked() { return ((raw & 0x2uL) != 0uL); } // Check mask before drawing (0 = always draw ; 1 = draw if bit15==0)
        // When bit1 is on, any old pixels in the framebuffer with bit15==1 are write-protected, and cannot be overwritten by rendering commands.
        // The mask setting affects all rendering commands, as well as CPU-to-VRAM and VRAM-to-VRAM transfer commands (where it acts as for 15bit textures). 
          // However, Mask does NOT affect the Fill-VRAM command.
        static inline long size() { return 1; } // length
    } attr_stpmask_t;


    // Fill blank area
    typedef struct DR_FILL
    {
        rgb24_t color;  // Primitive ID + rectangle color (RGB)
        coord16_t pos;  // top-left coordinates
        coord16_t size; // size (width / height)
        // Horizontally the filling is done in 16-pixel (32-bytes) units (steps of 10h).
        // X-size==400h works only indirectly: handled as X-size==0. However, X-size==[3F1h..3FFh] is rounded-up as X-size==400h.
        // If the Source/Dest starting points plus the width/height value exceed the framebuffer size, wrap to the opposite memory edge.
        // NOT affected by the mask settings
        static inline long size() { return 3; } // length
    } fill_area_t;

    // Load image (cpu to vram)
    typedef struct DR_LOAD
    {
        unsigned long cmd;     // Primitive ID
        coord16_t destination; // framebuffer destination: X = 0..3FFh ; Y = 0..1FFh (if more, clipped)
        coord16_t size;        // size (width / height): X = 0..400h ; Y = 0..200h (if more, clipped)
        // Size=0 is handled as Size=max
        // Transfer data through DMA
        // Parameters are clipped to 10bit (X) / 9bit (Y) range, the only special case is that Size=0 is handled as Size=max.
        // If the Source/Dest starting points plus the width/height value exceed the framebuffer size, wrap to the opposite memory edge.
        // Affected by the mask settings
        static inline long size() { return 3; } // length
    } img_load_t;

    // Store image (vram to central memory)
    typedef struct DR_STORE
    {
        unsigned long cmd;  // Primitive ID
        coord16_t source;   // framebuffer source: X = 0..3FFh ; Y = 0..1FFh (if more, clipped)
        coord16_t size;     // size (width / height): X = 0..400h ; Y = 0..200h (if more, clipped)
        // Size=0 is handled as Size=max
        // If the Source/Dest starting points plus the width/height value exceed the framebuffer size, wrap to the opposite memory edge.
        // Transfer data through DMA or gpuread port
        static inline long size() { return 3; } // length
    } img_store_t;

    // Framebuffer rectangle copy (vram to vram)
    typedef struct DR_MOVE
    {
        unsigned long cmd;     // Primitive ID
        coord16_t source;      // framebuffer source: X = 0..3FFh ; Y = 0..1FFh (if more, clipped)
        coord16_t destination; // framebuffer destination: X = 0..3FFh ; Y = 0..1FFh (if more, clipped)
        coord16_t size;        // size (width / height): X = 0..400h ; Y = 0..200h (if more, clipped)
        // Size=0 is handled as Size=max
        // If the Source/Dest starting points plus the width/height value exceed the framebuffer size, wrap to the opposite memory edge
        // Affected by the mask settings
        static inline long size() { return 4; } // length
    } img_move_t;


    // - attribute/data command functions - ------------------------------------

    void cmClearCache(unsigned char* pData); // GENERAL - clear cache
    void cmFillArea(unsigned char* pData);   // GENERAL - fill area
    void cmBufferWait(unsigned char* pData); // GENERAL - add empty statement to FIFO buffer (wait if full)
    void cmGpuIrq(unsigned char* pData);     // GENERAL - GPU interrupt request flag

    void cmImageMove(unsigned char* pData);  // IMAGE - move
    void cmImageLoad(unsigned char* pData);  // IMAGE - load
    void cmImageStore(unsigned char* pData); // IMAGE - store

    void cmTexPage(unsigned char* pData);      // ATTR - texture page
    void cmTexWindow(unsigned char* pData);    // ATTR - texture window
    void cmDrawAreaStart(unsigned char* pData);// ATTR - draw area start
    void cmDrawAreaEnd(unsigned char* pData);  // ATTR - draw area end
    void cmDrawOffset(unsigned char* pData);   // ATTR - draw offset
    void cmMaskBit(unsigned char* pData);      // ATTR - mask bit
}

#endif
