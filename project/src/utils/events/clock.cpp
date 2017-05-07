/*******************************************************************************
Author  :     Romain Vinders
License :     GPLv2
------------------------------------------------------------------------
Description : high-resolution clock
*******************************************************************************/
#include <cstdlib>
#include <cstdint>
#include <cstddef>
#include <ctime>
#include <chrono>
#ifdef _WINDOWS
#   define VC_EXTRALEAN
#   define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#   include <windowsx.h>
#   include <mmsystem.h>
#endif
#include "time_point.hpp"
#include "clock.h"
using namespace utils::events;


// -- Getters / Setters --

/// @brief Set time mode and tick rate
/// @param[in] preferedMode  Prefered time mode (if available)
void Clock::setTimeMode(const Clock::timemode_t preferedMode) noexcept
{
    #ifdef _WINDOWS
    LARGE_INTEGER cpuFreq;
    if (preferedMode == Clock::timemode_t::highResCounter && QueryPerformanceFrequency(&cpuFreq))
    {
        m_tickRate = reinterpret_cast<uint64_t>(cpuFreq.QuadPart);
        m_timeMode = Clock::timemode_t::highResCounter;
    }
    else
    {
        m_tickRate = 1000uLL;
        m_timeMode = Clock::timemode_t::multimediaClock;
    }
    #else
    m_timeMode = preferedMode;
    #endif
}

/// @brief Set clock frequency
/// @param[in] freqNumerator     Clock frequency (desired periods per second) - numerator (e.g.: 60000)
/// @param[in] freqDenominator   Clock frequency (desired periods per second) - denominator (e.g.: 1001)
void Clock::setFrequency(const uint32_t freqNumerator, const uint32_t freqDenominator) noexcept
{
    if (freqNumerator > 0u)
    {
        m_periodDuration = (static_cast<uint64_t>(freqDenominator) * 1000000000uLL) / static_cast<uint64_t>(freqNumerator);
        
        double subDuration = ((static_cast<double>(freqDenominator) * (1000000000.0 / static_cast<double>(freqNumerator)))) - static_cast<double>(m_periodDuration);
        m_periodSubDuration = (subDuration > 0.0 && subDuration < 1.0) ? static_cast<float>(subDuration) : 0.0f;
    }
    else
    {
        m_periodDuration = 0uLL;
    }
    m_droppedSubDurations = 0.0f;
}



// -- Time reference creation --

/// @brief Create new time reference
/// @returns Time reference (current time)
TimePoint Clock::now() noexcept
{
    #ifdef _WINDOWS
    if (m_timeMode == Clock::timemode_t::highResCounter)
    {
        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);
        return TimePoint::fromTicks(reinterpret_cast<events::ticks_t>(currentTime.QuadPart), m_tickRate);
    }
    else
    {
        return TimePoint::fromTicks(static_cast<events::ticks_t>(timeGetTime()), 1000uLL);
    }
    #else
    auto time = (m_timeMode == Clock::timemode_t::highResCounter) ? std::chrono::high_resolution_clock::now() : std::chrono::system_clock::now();
    return TimePoint(std::chrono::duration_cast<std::chrono::nanoseconds>(time).count());
    #endif
}

/// @brief Create new time reference (auxiliary)
/// @returns Low-resolution time reference (current time)
TimePoint Clock::nowAuxMode() noexcept
{
    #ifdef _WINDOWS
    if (m_timeMode == Clock::timemode_t::highResCounter)
    {
        return TimePoint::fromTicks(static_cast<events::ticks_t>(timeGetTime()), 1000uLL);
    }
    else
    {
        auto time = std::chrono::system_clock::now();
        return TimePoint(std::chrono::duration_cast<std::chrono::nanoseconds>(time).count());
    }
    #else
    auto time = (m_timeMode == Clock::timemode_t::highResCounter) ? std::chrono::system_clock::now() : std::chrono::steady_clock::now();
    return TimePoint(std::chrono::duration_cast<std::chrono::nanoseconds>(time).count());
    #endif
}



// -- Utilities --

/// @brief Reset time reference
void Clock::reset() noexcept
{
    m_timeReference = Clock::now();
    m_auxTimeReference = Clock::nowAuxMode();
    m_runtimeDuration = m_periodDuration;
    m_droppedSubDurations = 0.0f;
    
    m_freqCalcReference = m_timeReference;
    m_auxFreqCalcReference = m_auxTimeReference;
    m_periodCount = 1u;
}

/// @brief Wait for one period after time reference
/// @returns Number of nanoseconds that occured too late (after desired time reference)
uint64_t Clock::wait() noexcept
{
    TimePoint currentReference, currentAuxReference;
    int64_t elapsedTime;
    uint64_t lateness;
    
    // wait until end of period
    uint32_t waitLoopCount = 0u;
    do
    {
        currentReference = Clock::now();
        currentAuxReference = Clock::nowAuxMode();
        elapsedTime = currentReference - m_timeReference;
        
        // invalid time : negative difference or huge number -> use low-res timer instead
        if (elapsedTime <= 0LL || elapsedTime > 500000000LL)
        {
            elapsedTime = currentAuxReference - m_auxTimeReference;
            if (elapsedTime < 0LL) // both negative -> max integer value reached
                elapsedTime = m_runtimeDuration; // break
        }
        ++waitLoopCount;
    } while (elapsedTime < m_runtimeDuration);
    
    ++m_periodCount;
    m_timeReference = currentReference;
    m_auxTimeReference = currentAuxReference;
    
    // set period runtime duration, for next call
    if (waitLoopCount == 1u) // only one loop turn -> lateness -> wait less than period duration to compensate
    {
        lateness = elapsedTime - m_runtimeDuration;
        if (lateness > m_periodDuration) // more than an entire period too late
        {
            m_runtimeDuration = (m_periodDuration >> 1); // half a period
            m_droppedSubDurations = 0.0f;
            lateness = m_periodDuration;
        }
        else
        {
            m_runtimeDuration = m_periodDuration - lateness; // compensate
        }
    }
    else
    {
        m_runtimeDuration = m_periodDuration;
        lateness = 0u;
    }
    // restore dropped sub-durations, if sum is above 1 nanosecond
    m_droppedSubDurations += m_periodSubDuration;
    if (m_droppedSubDurations >= 1.0f) 
    {
        ++m_runtimeDuration;
        m_droppedSubDurations -= 1.0f;
    }
    
    return lateness; // number of ticks too late
}


/// @brief Calculate real number of periods per second
float Clock::checkFrequency() noexcept
{
    TimePoint currentReference, currentAuxReference;
    int64_t elapsedTime;
    
    // read current time
    currentReference = Clock::now();
    currentAuxReference = Clock::nowAuxMode();
    elapsedTime = currentReference - m_freqCalcReference;
    
    if (elapsedTime <= 0LL || elapsedTime > 500000000LL) // invalid time -> use low-res timer instead
    {
        elapsedTime = currentAuxReference - m_auxFreqCalcReference;
        if (elapsedTime <= 0LL) // both negative (max integer value reached) or zero (avoid division by zero)
        {
            elapsedTime = (elapsedTime == 0LL) 1LL : m_periodDuration;
        }
    }
    
    m_freqCalcReference = currentReference;
    m_auxFreqCalcReference = currentAuxReference;
    
    // calculate real frequency
    float frequency = static_cast<float>(m_periodCount) * (1000000000.0f / elapsedTime);
    
    m_periodCount = 0u;
    return frequency;
}
