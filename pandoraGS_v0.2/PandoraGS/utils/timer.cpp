/*******************************************************************************
PANDORAGS project - PS1 GPU driver
------------------------------------------------------------------------
Author  :     Romain Vinders
License :     GPLv2
------------------------------------------------------------------------
File name :   timer.cpp
Description : frame time management
*******************************************************************************/
#ifdef _WINDOWS
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#endif
#include <ctime>
using namespace std;
#include "timer.h"

#define MAX_LACE_NUMBER 16
#define MAX_SUCCESSIVE_SKIPPING 4

// timing mode
#ifdef _WINDOWS
LARGE_INTEGER Timer::s_cpuFreq; // CPU ticks frequency
ticks_t Timer::s_periodHiRes;   // high res period duration
#endif
ticks_t Timer::s_period;            // CPU ticks per period
ticks_t Timer::s_periodTimeout;     // max waited time before skipping (7/8 period time)
float Timer::s_targetFreq = 0.0f;   // configured frequency
timemode_t Timer::s_timeMode;       // timing mode
bool Timer::s_isInterlaced = false; // interlaced periods

// timer execution
bool Timer::s_isResetPending = true;  // time reference reset query
float Timer::s_measuredFreq = 0.0f;   // current measured frequency
int Timer::s_periodsSinceMeasure = 0; // number of periods since last check
int Timer::s_periodsToSkip = 0;       // number of periods to skip (to stay in sync)
ticks_t Timer::s_lateTicks = 0uL;     // current lateness (compared to time reference)


// -- TIMER SETTINGS -- ----------------------------------------------------

/// <summary>Set time management mode</summary>
void Timer::setTimeMode(timemode_t type)
{
    s_isInterlaced = false;
    s_periodsToSkip = 0;

    #ifdef _WINDOWS
    if (type == Timemode_highResCounter && QueryPerformanceFrequency(&s_cpuFreq) == false)
        s_timeMode = Timemode_multimediaClock;
    else
        s_timeMode = type;
    #else
    s_timeMode = Timemode_multimediaClock;
    #endif
}

/// <summary>Set timer frequency</summary>
/// <param name="freqLimit">Frequency max limit</param>
/// <param name="regMode">Regional sync mode</param>
/// <param name="isInterlaced">Interlacing</param>
void Timer::setFrequency(float freqLimit, regionsync_t regMode, bool isInterlaced)
{
    s_isInterlaced = isInterlaced;
    
    // set fixed framerate limit
    s_targetFreq = 59.94f;
    if (freqLimit > 0.02f) // != 0 (+ float error offset) -> enabled
    {
        s_targetFreq = freqLimit;
    }
    // set auto framerate limit
    else if (regMode != Regionsync_undefined)
    {
        switch (regMode)
        {
            case Regionsync_ntsc: 
                if (s_isInterlaced)
                    s_targetFreq = 33868800.0f / 565031.25f; // 59.94146
                else
                    s_targetFreq = 33868800.0f / 566107.50f; // 59.82750
                break;
            case Regionsync_pal: 
                if (s_isInterlaced)
                    s_targetFreq = 33868800.0f / 677343.75f; // 50.00238
                else
                    s_targetFreq = 33868800.0f / 680595.00f; // 49.76351
                break;
            case Regionsync_ntsc_pcfix: 
                s_targetFreq = (s_isInterlaced) ? 60.0f : 30.0f;
                break;
            case Regionsync_pal_pcfix: 
                s_targetFreq = (s_isInterlaced) ? 50.0f : 25.0f;
                break;
        }
    }

    // set period duration + timeout
    double duration = 1000.0 / (double)s_targetFreq;
    s_period = (ticks_t)duration;
    #ifdef _WINDOWS
    if (s_timeMode == Timemode_highResCounter)
    {
        double durationHiRes = (double)(s_cpuFreq.LowPart) / (double)s_targetFreq;
        s_periodHiRes = (ticks_t)durationHiRes;
        s_periodTimeout = ((7 * s_periodHiRes) >> 3);
    }
    else
        s_periodTimeout = ((7 * s_period) >> 3);
    #else
    s_periodTimeout = ((7 * s_period) >> 3);
    #endif

    // reset frame skipping
    s_periodsToSkip = 0;
    s_lateTicks = 0uL;
    // reset frame management values
    s_measuredFreq = 0.0f;
    s_periodsSinceMeasure = 0;
    s_isResetPending = true;
}


// -- TIME OPERATIONS -- ---------------------------------------------------

/// <summary>Wait for one period (after previous time reference) + skipping</summary>
/// <param name="speedChange">Speed modifier (normal/slow/fast)</param>
/// <param name="isOddPeriod">Odd period (if interlaced)</param>
void Timer::wait(speed_t speedChange, bool isOddPeriod)
{

}

/// <summary>Calculate current effective frequency</summary>
void Timer::calcFrequency()
{

}
