set(window_libs__FOUND ON)

if(NOT IOS AND NOT ANDROID)
    if(APPLE)
        set(_EXTERNAL_FRAMEWORKS
            Cocoa
            IOKit
            CoreFoundation
        )
        if(CWORK_EXTERN_FRAMEWORKS)
            set(CWORK_EXTERN_FRAMEWORKS ${CWORK_EXTERN_FRAMEWORKS} ${_EXTERNAL_FRAMEWORKS})
        else()
            set(CWORK_EXTERN_FRAMEWORKS ${_EXTERNAL_FRAMEWORKS})
        endif()
        unset(_EXTERNAL_FRAMEWORKS)

    elseif(WIN32 OR WIN64 OR _WIN32 OR _WIN64 OR CMAKE_SYSTEM_NAME STREQUAL "Windows")
        set(window_libs__LINKED kernel32 user32 comdlg32 ole32 oleaut32 gdi32)
        if(_DRAG_DROP_SUPPORT)
            set(window_libs__LINKED ${window_libs__LINKED} shell32)
        endif()
        
    else()
        set(window_libs__LINKED X11)
    endif()

    if(CWORK_EXTERN_FRAMEWORKS AND NOT CWORK_EXTERN_FRAMEWORKS_SCOPE)
        if(CWORK_EXTERN_LIBS_SCOPE)
            set(CWORK_EXTERN_FRAMEWORKS_SCOPE ${CWORK_EXTERN_LIBS_SCOPE})
        else()
            set(CWORK_EXTERN_FRAMEWORKS_SCOPE PUBLIC)
        endif()
    endif()
endif()
