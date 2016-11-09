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
enum ProfilePreset : uint32_t
{
    ProfilePreset_Fastest = 0,
    ProfilePreset_Standard = 1,
    ProfilePreset_Enhanced2D = 2,
    ProfilePreset_Enhanced3D = 3,
};

// custom fixes
#define CFG_FIX_FRAMEBUFFER_FF7        0x1 // FF7 battle cursor fix - adjusted framebuffer access
#define CFG_FIX_FRAMEBUFFER_DIRECT     0x2 // speed frame upload fix - direct framebuffer updates
#define CFG_FIX_IGNORE_BLACK           0x4 // black screens & Lunar fix - ignore black brightness
#define CFG_FIX_SWAP_FRONTBACK         0x8 // Speed Freaks & Killer Loop fix - swap front/back detection
#define CFG_FIX_NO_COORD_CHECK        0x10 // coord compatibility mode
#define CFG_FIX_REMOVE_BLUE           0x20 // Legacy of Dragoon fix - remove blue glitches (needs alpha multipass)
#define CFG_FIX_REACTIVE_FPSLIMIT     0x40 // frame displayed before frame time waiting - feels more reactive but less steady
#define CFG_FIX_PC_FPSLIMIT           0x80 // MGS fix - special FPS limit (theoretical values, no wait if skipped)
#define CFG_FIX_HALF_SKIPPING        0x100 // skip one frame out of two
#define CFG_FIX_POLYGON_CACHE_FF9    0x200 // FF9 battle rect fix - G4 polygon cache
#define CFG_FIX_IGNORE_SMALL_MOVE    0x400 //+ FF7 & 8 smooth fix - ignore small framebuffer moves
#define CFG_FIX_LAZY_UPLOAD          0x800 // Dynasty Warriors 7 fix - lazy upload detection
#define CFG_FIX_STATUS_INTERLACE    0x1000 // Chronocross fix - switch during status read instead of update lace
#define CFG_FIX_EXPAND_SCREEN       0x2000 // Capcom fix - fix screen width to show full area
#define CFG_FIX_FILTERING_BLACK     0x4000 // old filtering fix - removes black areas
#define CFG_FIX_SPECIAL_UPLOAD      0x8000 // splash screens fix - special upload detection
#define CFG_FIX_FAKE_LOWCOMP_READ  0x10000 //+ LOD & RPGmaker fix - fake low compatibility frame read
#define CFG_FIX_FAKE_GPU_BUSY      0x20000 // fake busy emulation hack
//#define CFG_FIX_19               0x40000
//#define CFG_FIX_20               0x80000


// Driver configuration profile container
class ConfigProfile
{
public:
    // general
    uint32_t gen_profileId;       // profile unique identifier
    std::wstring  gen_profileName; // profile displayed name

    // smooth/scale filters
    uint32_t scl_texSmooth;       // textures smoothing
    uint32_t scl_texUpscaleVal;   // textures upscaling - 1-5x
    uint32_t scl_texUpscaleType;  // textures upscaling - type (algorithm)
    uint32_t scl_sprSmooth;       // 2D/sprites smoothing
    uint32_t scl_sprUpscaleVal;   // 2D/sprites upscaling - 1-5x
    uint32_t scl_sprUpscaleType;  // 2D/sprites upscaling - type (algorithm)
    uint32_t scl_screenSmooth;// global screen smoothing - type (blur, algorithm, ..)
    uint32_t scl_screenUpscaleVal; // global screen upscaling - 1-8x
    uint32_t scl_screenUpscaleType;// global screen upscaling - type (blur, algorithm, ..)
    bool     scl_isShaderUpscale; // upscaling algorithm using shader (faster, less precision)
    uint32_t scl_mdecFilter;      // MDEC video filter
    //smooth texture transitions ('texture splatting' avec textures de polygones voisins)
                //-> essayer de ne le faire qu'une fois par association de textures (tableau avec ID textures ?)
                //-> param�tre �tendue : l�ger d�grad� / moyen d�grad� / moyen distribu� / fort d�grad� / fort distribu�
                //-> distribution pattern (au lieu de d�grad�, baser transition sur masque 
                                        //(selon une image, ou selon nuages de plus en plus petits/transparents)
                //-> �tendue d�pend de taille des polygones (petits polys = petites transitions) -> �vite fond flou 
                                        //(! pr�voir �tendue max assez petite, pour �viter skyboxes compl�tement floues)
    
    // shading
    uint32_t shd_antiAliasing;    // anti-aliasing
    //bump mapping
    //motion blur
    //HDR bloom
    //HDR luma sharpen
    //HDR divers : tonemapping pass, light attenuation, pixel vibrance, subpixel dithering
    //effets (cel-shading V1 � 4, kirsch-n�gatif, storybook, bruit, incrustations, ...) + force
    //effets colorim�trie (couleurs daltoniens, natural vision, CRT, verdatre, bleuatre, d�satur� cuivr�, d�satur� gris) + force

    // screen adjustment
    uint32_t dsp_internalResX;    // internal resolution [x]
    uint32_t dsp_internalResY;    // internal resolution [y]
    uint32_t dsp_ratioType;       // screen ratio type (aspect ratio, pixel ratio)
    uint32_t dsp_stretchRatio;    // screen "keep ratio" (0) / "full stretch" (8) - ratio (0 - 8)
    uint32_t dsp_cropStrength;    // screen "black sides" (0) / "cropping" (8) - strength (0 - 8)
    bool     dsp_isScreenMirror;  // screen mirroring (mirrored/normal)
    uint32_t dsp_borderSize;      // add black borders (0 = none)
    uint32_t dsp_screenCurved;    // emulate curved CRT screen (0 to 2 ; 0 = none)
    
    // rendering corrections
        //corrections -> gamma/contraste (+ presets PAL, NTSC, neutre, presets selon jeux) + S-curve contrast
        //zbuffer/order table ???
   
    //scanlines types :
    //      - simples lignes sombres (garder pixels originaux)
    //      - simples lignes sombres avec moyenne des valeurs des pixels
    //      - en lignes  R V B     (R et B contiennent un peu du V)
    //                    B V R   (vert + ajout beaucoup R et B)
    //      - en triangles   R    V B   (tous avec un peu plus des deux autres)
    //                      V B    R
    //scanlines: INT: couleur unie (noir) / valeur pixel -> slider (0-8)
    //scanlines: INT: sombre (noir) / color�-clair (0-16 -> deviendra 0-255 pour couleur unie)
    
    //cursor: enabled, type, color, saturation, opacity
    
    // miscellaneous
    uint32_t misc_fixBits;           // custom fixes
    uint32_t misc_offscreenDrawing;  // off-screen drawing mode
    //alpha/maskbit/psx texture window/...
    //framebuffer/...


public:
    /// <summary>Create profile container (with preset values)</summary>
    /// <param name="id">Profile unique identifier</param>
    /// <param name="name">Profile name</param>
    ConfigProfile(uint32_t id, std::wstring name);
    /// <summary>Copy profile container</summary>
    /// <param name="copy">Profile container to copy</param>
    ConfigProfile(ConfigProfile& copy);

    /// <summary>Set profile preset values</summary>
    /// <param name="preset">Default values to use</param>
    void setPresetValues(const ProfilePreset preset);

    /// <summary>Enable fix bits/summary>
    /// <param name="fixBits">Fix bits mask</param>
    inline void setFix(uint32_t fixBits)
    {
        misc_fixBits |= fixBits;
    }
    /// <summary>Disable fix bits</summary>
    /// <param name="fixBits">Fix bits mask</param>
    inline void unsetFix(uint32_t fixBits)
    {
        misc_fixBits &= ~fixBits;
    }
    /// <summary>Check if fix bit is active</summary>
    /// <param name="fixBits">Bit(s) mask (will return true if all are active)</param>
    inline bool getFix(uint32_t fixBits)
    {
        return ((misc_fixBits & fixBits) == fixBits);
    }
    /// <summary>Check if fix bit is disabled</summary>
    /// <param name="fixBits">Bit(s) mask (will return true if all are disabled)</param>
    inline bool getNotFix(uint32_t fixBits)
    {
        return ((misc_fixBits & fixBits) == 0);
    }
};


// screen ratio modes
enum CFG_ScreenRatio : uint32_t
{
    CFG_Ratio_Aspect = 0, // 15:10 - NTSC US/J + some PAL ports / 4:3 - PAL standard
    CFG_Ratio_Pixel = 1
};
#define CFG_ScreenRatio_LENGTH 2
// screen stretching presets
#define CFG_RATIO_MAX  8
#define CFG_RATIO_HALF 4
#define CFG_RATIO_STRETCH_FullWindow  CFG_RATIO_MAX  // full window stretch - best if emulator uses widescreen hack
#define CFG_RATIO_CROP_FullWindow     0
#define CFG_RATIO_STRETCH_Orig        0              // keep ratio - best with 2D
#define CFG_RATIO_CROP_Orig           0
#define CFG_RATIO_STRETCH_OrigFill    0              // keep ratio & crop - strong top/bottom cut
#define CFG_RATIO_CROP_OrigFill       CFG_RATIO_MAX
#define CFG_RATIO_STRETCH_CloseToOrig CFG_RATIO_HALF // half stretch & slight cropping - best with 3D
#define CFG_RATIO_CROP_CloseToOrig    CFG_RATIO_MAX

// off-screen drawing modes
enum CFG_OffScreenDrawing : uint32_t
{
    //...
    CFG_OffScr_Standard = 1
    //...
};
#define CFG_OffScreenDrawing_LENGTH 3

// interpolation modes
enum CFG_Interpolation : uint32_t
{
    CFG_Intp_Nearest = 0,
    CFG_Intp_Bilinear = 1,
    CFG_Intp_Bilinear_Enhanced = 2,
    CFG_Intp_Bicubic_Enhanced = 3,
    CFG_Intp_Gaussian = 4
};
#define CFG_Interpolation_LENGTH 5
// upscaling modes
enum CFG_UpScaling : uint32_t
{
    CFG_UpSc_Native = 0,        // 1x
    CFG_UpSc_SaI = 1,               // 2x
    CFG_UpSc_xBR = 2,  //LV2        // 2x, 3x, 4x
    CFG_UpSc_xBRZ = 3,              // 2x, 3x, 4x, 5x
    CFG_UpSc_xBRZ_Smoothed = 4,     //     3x, 4x, 5x // deposterize pre-filter -> utiliser option ind�pendante du filtre ?
    CFG_UpSc_SuperxBR = 5,          // 2x,     4x,     8x
    CFG_UpSc_SuperxBR_FB = 6,       // 2x,     4x,     8x  // FB = fast-bilateral
    CFG_UpSc_NNEDI3 = 7             // 2x,     4x,     8x
};
#define CFG_UpScaling_LENGTH 8
// MDEC filters
enum CFG_MdecFiltering : uint32_t
{
    CFG_Mdec_None = 0,
    CFG_Mdec_Standard = 1,
    CFG_Mdec_SuperxBR = 2,
    CFG_Mdec_NNEDI3 = 3
};
#define CFG_MdecFiltering_LENGTH 4
// screen smoothing
enum CFG_ScreenSmoothing : uint32_t
{
    CFG_ScrSm_None = 0,
    CFG_ScrSm_SlightBlur = 1,
    CFG_ScrSm_Blur = 2,
    CFG_ScrSm_BlurNoise = 3
};
#define CFG_ScreenSmoothing_LENGTH 4

// anti-aliasing
enum CFG_AntiAliasing : uint32_t
{
    CFG_AA_None = 0,
    CFG_AA_FXAA = 1, // fast
    CFG_AA_NFAA = 2, // good with 2D
    CFG_AA_MSAA4 = 3,
    CFG_AA_MSAA8 = 4,
    CFG_AA_SMAA4 = 5,
    CFG_AA_SMAA8 = 6
};
#define CFG_AntiAliasing_LENGTH 7

#endif
