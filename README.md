#PandoraGS - GPU driver for emulator
PandoraGS is a GPU driver plugin for PlayStation emulators (PSEmu Pro specification).
#########################################################################

    Author            : Romain Vinders
    Prog. languages   : C++, GLSL, HLSL
    Supported systems : currently Windows NT (Vista+) (later Debian/OS X?)
    License           : GPLv2

#########################################################################

##Done:
* driver init/open/close/shutdown, low-level commands, read/write access
* frame rate management - FPS counter, frame rate limit, frame skipping
* user input - change profile, toggle config elements, goodies (fast forward, ...)
* configuration management - general settings, profile system, game/profile association
* multilanguage support and error log system

##Partly done:
* configuration dialog - user-friendly UI with tabs

##To do:
* psx primitives management
* graphics rendering - rendering pipeline and shaders (OpenGL, later DirectX)
* display management - screen resolution, internal resolution, ratio, stretching, mirroring, ...
* filtering/upscaling, shader effects (AA, motion blur, bump mapping, HDR, color filters, ...)
* goodies (visual vibrations, snapshots, cursor, scanlines, ...)
