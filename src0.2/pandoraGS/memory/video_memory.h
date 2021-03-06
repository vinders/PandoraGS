/*******************************************************************************
PANDORAGS project - PS1 GPU driver
------------------------------------------------------------------------
Author  :     Romain Vinders
License :     GPLv2
------------------------------------------------------------------------
File name :   video_memory.h
Description : video memory (vram) image
*******************************************************************************/
#ifndef _VIDEO_MEMORY_H
#define _VIDEO_MEMORY_H
#include <cstddef>
#include <cstdint>
#include <string>

// data types
enum loadmode_t : int32_t
{
    Loadmode_normal = 0,
    Loadmode_vramTransfer = 1
};

// VRAM size (kilobytes)
#define VRAM_SIZE             512
#define VRAM_SECURITY_OFFSET  512 // offset before and after vram
// dma access masks
#define THREEBYTES_MASK       0x0FFFFFFu
#define PSXVRAM_MASK          0x1FFFFCu // 2097148
#define PSXVRAM_THRESHOLD     2000000u
// data transaction codes
#define GPUDATA_INIT          0x400
#define GPUINFO_TW            0
#define GPUINFO_DRAWSTART     1
#define GPUINFO_DRAWEND       2
#define GPUINFO_DRAWOFF       3


// Video memory (VRAM) image
class VideoMemory
{
private:
    // emulated vram
    uint8_t*  m_pVramImage;      // allocated memory image
    bool      m_isDoubledBufSize;// doubled buffer size (for Zinc) or not
    size_t    m_vramBufferSize;  // single vram buffer size
    size_t    m_vramTotalSize;   // total allocated memory

    // memory access
    uint8_t*  m_pByte;   // 8 bits access mode
    uint16_t* m_pWord;   // 16 bits access mode
    uint32_t* m_pDword;  // 32 bits access mode
    uint16_t* m_pEnd;    // end of effective zone
    uint32_t  m_dmaAddresses[3]; // DMA address check


public:
    // memory access iterator
    class iterator
    {
    private:
        VideoMemory* it_pTarget; // memory target
        uint16_t* it_pWord;     // 16 bits access mode

    public:
        /// <summary>Create iterator</summary>
        iterator() : it_pTarget(NULL), it_pWord(NULL) {}
        /// <summary>Create iterator</summary>
        /// <param name="target">Target memory</summary>
        iterator(VideoMemory& target) : it_pTarget(&target), it_pWord(target.m_pWord) {}
        /// <summary>Copy constructor</summary>
        /// <param name="copy">Iterator to copy</summary>
        iterator(const VideoMemory::iterator& copy) : it_pTarget(copy.it_pTarget), it_pWord(copy.it_pWord) {}
        /// <summary>Get current position</summary>
        inline uint16_t* getPos();
        /// <summary>Get value at current position</summary>
        inline uint16_t getValue();
        /// <summary>Set value at current position</summary>
        /// <param name="val">Value</returns>
        inline void iterator::setValue(uint16_t val);
        /// <summary>Increment</summary>
        inline VideoMemory::iterator& operator++();
        inline VideoMemory::iterator operator++(int);
        /// <summary>Decrement</summary>
        inline VideoMemory::iterator& operator--();
        inline VideoMemory::iterator operator--(int);
        /// <summary>Add offset</summary>
        inline VideoMemory::iterator operator+(const int off);
        inline VideoMemory::iterator& operator+=(const int off);
        /// <summary>Remove offset</summary>
        inline VideoMemory::iterator operator-(const int off);
        inline VideoMemory::iterator& operator-=(const int off);
    };


public:
    // -- MEMORY ALLOCATION -- -------------------------------------------------

    /// <summary>Initialize null pointers</summary>
    VideoMemory();
    /// <summary>Release memory allocations</summary>
    ~VideoMemory();

    /// <summary>Memory allocation and initialization</summary>
    /// <param name="isDoubledBufSize">Use doubled buffer size (for Zinc emu)</exception>
    /// <exception cref="std::exception">Memory allocation failure</exception>
    void init(bool isDoubledBufSize = false);
    /// <summary>Release memory allocations</summary>
    void close();


    // -- MEMORY ITERATION -- --------------------------------------------------

    /// <summary>Create iterator at the beginning of the effective memory zone</summary>
    /// <returns>New iterator</returns>
    /// <exception cref="std::exception">Uninitialized memory</exception>
    inline VideoMemory::iterator begin()
    {
        if (m_pVramImage == NULL)
            throw std::exception("VideoMemory.begin: can't get iterator from uninitialized memory");
        return VideoMemory::iterator(*this);
    }
    /// <summary>Get pointer after the end of the effective memory zone</summary>
    /// <returns>End of memory</returns>
    inline uint16_t* end()
    {
        return m_pEnd;
    }
    /// <summary>Get pointer to the beginning of the effective memory zone</summary>
    /// <returns>Start of memory</returns>
    inline uint16_t* rend()
    {
        return m_pWord;
    }
    /// <summary>Get pointer to the beginning of the effective memory zone</summary>
    /// <returns>Start of memory</returns>
    inline uint8_t* get()
    {
        return m_pByte;
    }
    /// <summary>Get the size of a single memory buffer</summary>
    /// <returns>New iterator</returns>
    inline size_t size()
    {
        return m_vramBufferSize;
    }
    /// <summary>Check if doubled size is used</summary>
    /// <returns>Size doubled or not</returns>
    inline bool isDoubledSize()
    {
        return m_isDoubledBufSize;
    }


    // -- MEMORY IO -- -------------------------------------------------------------

    /// <summary>Initialize check values for DMA chains</summary>
    inline void resetDmaCheck()
    {
        m_dmaAddresses[0] = THREEBYTES_MASK;
        m_dmaAddresses[1] = THREEBYTES_MASK;
        m_dmaAddresses[2] = THREEBYTES_MASK;
    }
    /// <summary>Check DMA chain for endless loop (Pete's fix)</summary>
    /// <param name="addr">Memory address to check</param>
    inline bool checkDmaEndlessChain(unsigned long addr)
    {
        if (addr == m_dmaAddresses[1] || addr == m_dmaAddresses[2])
            return true;
        if (addr < m_dmaAddresses[0])
            m_dmaAddresses[1] = addr;
        else
            m_dmaAddresses[2] = addr;
        m_dmaAddresses[0] = addr;
        return false;
    }
};

#include "video_memory_iterator.hpp" // inline iterator definitions
#endif
