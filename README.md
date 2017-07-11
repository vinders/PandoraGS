# PandoraGS - GPU driver for emulator
![GPLv2](https://img.shields.io/badge/license-GPL2-blue.svg)
![v0.3.1](https://img.shields.io/badge/latest-v0.3.1-blue.svg)
![Status](https://img.shields.io/badge/status-not_ready-red.svg)
<br/>
![C++14](https://img.shields.io/badge/language-C++14-lightgrey.svg)
![OpenGL4.1](https://img.shields.io/badge/api-OpenGL_4.1-lightgrey.svg)

PandoraGS is a GPU driver plugin for PlayStation emulators (PSEmu compatible).

    Author       : Romain Vinders
    Languages    : English, French, Spanish, German (or external file)
    Build        : Visual Studio 2015+ / gcc 5.2+
    Requirements : Windows NT 6.1+ (7, 8, 10+) / Debian, Ubuntu, Mint 
                   Visual C++ Runtimes 2015 (x86) / glibc 2.23+
                   OpenGL 4.1+ compatible drivers
                   256MB+ VRAM

                   
### Current work (2017):
* heavy refactoring
* creation of a reusable framework with all the necessary utilities + algorithms
* new unit testing system + win32/win64/linux32/linux64 builds
* test tools for upscaling, filters, shaders, forms, ...
* C++11/C++14 usage + advanced optimization + intrinsics


### Done:
* driver init/open/close/shutdown, low-level commands, read/write access
* frame rate management - FPS counter, frame rate limit, frame skipping
* user input - change profile, toggle config elements, goodies (fast forward, slow motion, ...)
* configuration management - general settings, profile system, game/profile association
* configuration dialog - user-friendly UI with tabs, profile manager, settings, import/export
* multilanguage support and error log system
* debug mode - debug terminal + vram data export
* test utilities - unit testing, primitive testing, rendering demo, dialog launcher

### To do (late 2017 - 2018):
* processing and rendering of primitives and attributes
* display management - screen resolution, internal resolution, ratio, stretching, cropping, mirroring, ...
* textures/sprites/screen filtering and upscaling
* shader effects (AA, motion blur, bump mapping, color filters, noise, HDR, ...)
* configuration dialog - final settings
* goodies (visual vibrations, snapshots, cursor, scanlines, ...)

### To do when complete:
* fix game-specific bugs
* full compatibility with ePSXe, PCSX-R and Zinc
* gnu-linux (debian/ubuntu) port
* mac os (10.9+) port
* support for PGXP
