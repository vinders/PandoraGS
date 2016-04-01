/*******************************************************************************
PANDORAGS project - PS1 GPU driver
------------------------------------------------------------------------
Author  :     Romain Vinders
License :     GPLv2
------------------------------------------------------------------------
File name :   config_profile.h
Description : configuration profile container
*******************************************************************************/
#ifndef _CONFIG_PROFILE_H
#define _CONFIG_PROFILE_H
#include "globals.h"

#include <string>

// presets
enum ProfilePreset
{
    ProfilePreset_Fastest = 0,
    ProfilePreset_Standard = 1,
    ProfilePreset_Enhanced2D = 2,
    ProfilePreset_Enhanced3D = 3,
};


// Driver configuration profile container
class ConfigProfile
{
public:
    // general
    unsigned int gen_profileId;     // profile unique identifier
    std::string  gen_profileName;   // profile displayed name

    // smooth/scale filters
    unsigned int scl_textureSmooth;     // textures smoothing
    unsigned int scl_textureUpscale;    // textures hi-res upscaling
    unsigned int scl_spriteSmooth;      // 2D/sprites smoothing
    unsigned int scl_spriteUpscale;     // sprites hi-res upscaling
    unsigned int scl_screenSmooth;      // global screen smoothing
    bool         scl_isMdec;            // MDEC video filter
    bool         scl_isGpuTxScaling;    // scaling textures with GPU shaders (true) or software
    bool         scl_isGpu2dScaling;    // scaling sprites/screen with GPU shaders (true) or software
    
    // shading
    unsigned int shd_antiAliasing;      // anti-aliasing

    // screen adjustment
    unsigned int dsp_internalResX;      // internal resolution [x]
    unsigned int dsp_internalResY;      // internal resolution [y]
    unsigned int dsp_screenStretch;     // screen stretching mode
    bool         dsp_isExtraRender;     // extra rendering instead of black bars
    bool         dsp_isScreenMirror;    // screen mirror (mirrored/normal)
    unsigned int dsp_borderSize;        // add black borders (0 = none)

    // custom fixes
    bool         sync_hasFixAutoLimit;  // fix - use theoretical FPS limit (25 or 30, doubled if interlaced)
    bool         sync_hasFixInterlace;  // chronocross fix - switch during status read instead of update lace
    bool         dsp_hasFixExpandScreen;// capcom fix - fix screen width to show full area


public:
    /// <summary>Create profile container (with preset values)</summary>
    /// <param name="id">Profile unique identifier</param>
    /// <param name="name">Profile name</param>
    ConfigProfile(unsigned int id, std::string name);
    /// <summary>Copy profile container</summary>
    /// <param name="copy">Profile container to copy</param>
    ConfigProfile(ConfigProfile& copy);

    /// <summary>Set profile preset values</summary>
    /// <param name="preset">Default values to use</param>
    void setPresetValues(const ProfilePreset preset);
};


// smoothing modes
enum CfgSmoothing
{
    CfgSmoothing_Nearest = 0,
    CfgSmoothing_Bilinear = 1,
    CfgSmoothing_BilinearEnhanced = 2,
    CfgSmoothing_Bicubic = 3,
    CfgSmoothing_BicubicEnhanced = 4,
    CfgSmoothing_GaussianBlur = 5
};
// upscaling modes
enum CfgUpscaling
{
    CfgUpscaling_Native = 0,
    CfgUpscaling_2xSaI = 1,
    CfgUpscaling_2xSuperEagle = 2,
    CfgUpscaling_HQ2X = 3,
    CfgUpscaling_HQ3X = 4,
    CfgUpscaling_HQ4X = 5,
    CfgUpscaling_2xBRZ = 6,
    CfgUpscaling_3xBRZ = 7,
    CfgUpscaling_4xBRZ = 8
};
// screen smoothing
enum CfgScreenSmooth
{
    CfgScreenSmooth_None = 0,
    CfgScreenSmooth_Blur = 1,
    CfgScreenSmooth_HQ3x = 2,
    CfgScreenSmooth_HQ4x = 3,
    CfgScreenSmooth_3xBRZ = 4,
    CfgScreenSmooth_4xBRZ = 5,
    CfgScreenSmooth_5xBRZ = 6,
    CfgScreenSmooth_Blur3xBRZ = 7,
    CfgScreenSmooth_Blur4xBRZ = 8
};
// screen stretching
enum CfgStretching
{
    CfgStretching_Native = 0,
    CfgStretching_FullWindow = 1,
    CfgStretching_PixelRatio = 2,
    CfgStretching_AspectRatio = 3,
    CfgStretching_AspectRatioCut = 4,
    CfgStretching_AspectSlight = 5,
    CfgStretching_AspectSlightCut = 6
};
#define CFGSTRETCHING_LAST CfgStretching_AspectSlightCut
// anti-aliasing
enum CfgAntiAliasing
{
    CfgAntiAliasing_None = 0,
    CfgAntiAliasing_FXAA = 1,
    CfgAntiAliasing_NFAA = 2,
    CfgAntiAliasing_MSAA4 = 3,
    CfgAntiAliasing_MSAA8 = 4,
    CfgAntiAliasing_SMAA4 = 5,
    CfgAntiAliasing_SMAA8 = 6
};


#endif
