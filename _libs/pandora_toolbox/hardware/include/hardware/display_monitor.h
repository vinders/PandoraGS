/*******************************************************************************
Author  :     Romain Vinders
License :     MIT
*******************************************************************************/
#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace pandora { 
  namespace hardware {
    /// @brief Display position/area ("virtual desktop" coordinates)
    struct DisplayArea final {
      int32_t x;       ///< Horizontal position of a screen/context/window. Can be negative if it's not the primary display.
      int32_t y;       ///< Vertical position of a screen/context/window. Can be negative if it's not the primary display.
      uint32_t width;  ///< Horizontal size of a screen/context/window.
      uint32_t height; ///< Vertical size of a screen/context/window.
    };
    /// @brief Display mode settings for a monitor
    struct DisplayMode final {
      uint32_t width;       ///< Horizontal size (pixels).
      uint32_t height;      ///< Vertical size (pixels).
      uint32_t bitDepth;    ///< Color depth (bits per pixel). Usually 32.
      uint32_t refreshRate; ///< Display rate (hertz)
    };
    constexpr inline uint32_t undefinedRefreshRate() noexcept { return 0; } ///< Unknown refresh rate (when used to set a display mode, the refresh rate isn't changed)

    // ---

    /// @class DisplayMonitor
    /// @brief Display monitor description (handle and attributes) + discovery utilities.
    class DisplayMonitor final {
    public:
#     ifdef _WINDOWS
        using DeviceId = std::wstring;
        using Handle = void*;
        using WindowHandle = void*;
        using String = std::wstring;
#     else
        using DeviceId = std::string;
        using Handle = int32_t;
        using WindowHandle = int32_t;
        using String = std::string;
#     endif

      /// @brief Attributes of a display monitor on a system
      /// @warning Measures in 'screenArea' and 'workArea' only use physical pixels if the process is DPI aware.
      struct Attributes final {
        DeviceId id;            ///< Unique identifier on the system.
        String description;     ///< Description string of the monitor (usually a type of monitor or a brand).
        String adapter;         ///< Description string of the associated adapter (usually the brand of the GPU).
        DisplayArea screenArea; ///< Display position/area of the entire screen in "virtual desktop". Can contain negative values if not a primary monitor.
        DisplayArea workArea;   ///< Max work area in "virtual desktop": screen area excluding taskbar/toolbars. Can contain negative values if not a primary monitor.
        bool isPrimary;         ///< Primary or secondary display monitor.
      };

      // -- monitor description --

      /// @brief Get primary monitor description
      DisplayMonitor() noexcept;
      /// @brief Get monitor description from handle
      /// @warning Throws invalid_argument if handle is invalid or can't be read
      DisplayMonitor(Handle monitorHandle, bool usePrimaryAsDefault);
      /// @brief Get monitor description by identifier (or primary if not found and usePrimaryAsDefault==true)
      /// @warning Throws invalid_argument if ID not found and usePrimaryAsDefault==false
      DisplayMonitor(const DisplayMonitor::DeviceId& id, bool usePrimaryAsDefault);
      /// @brief Get monitor description by index (or primary if not found and usePrimaryAsDefault==true)
      /// @warning Throws invalid_argument if index not found and usePrimaryAsDefault==false
      DisplayMonitor(uint32_t index, bool usePrimaryAsDefault);

      DisplayMonitor(const DisplayMonitor&) = delete;
      DisplayMonitor(DisplayMonitor&&) noexcept = default;
      DisplayMonitor& operator=(const DisplayMonitor&) = delete;
      DisplayMonitor& operator=(DisplayMonitor&&) noexcept = default;
      ~DisplayMonitor() = default;

      /// @brief Get list of monitors available for display
      static std::vector<DisplayMonitor> listAvailableMonitors() noexcept;

      // -- accessors --

      /// @brief Get native monitor handle
      inline Handle handle() noexcept { return this->_handle; }
      /// @brief Get display attributes of monitor
      inline const Attributes& attributes() const noexcept { return this->_attributes; }

      // -- display mode --

      /// @brief Read current display mode of a monitor
      DisplayMode DisplayMonitor::getDisplayMode() noexcept;
      /// @brief Change display mode of a monitor (for fullscreen apps)
      /// @remarks To keep the original attribute values in object (for later use), set refreshAttributes to false
      bool setDisplayMode(const DisplayMode& mode, bool refreshAttributes = true) noexcept;
      /// @brief Reset monitor to its default display mode
      /// @remarks To keep the original attribute values in object (for later use), set refreshAttributes to false
      bool setDefaultDisplayMode(bool refreshAttributes = true) noexcept;

      /// @brief Read available display modes for a monitor
      std::vector<DisplayMode> listAvailableDisplayModes() noexcept;

      // -- DPI awareness --

      /// @brief Set DPI awareness for current process (if supported on current system)
      /// @warning - on Windows, it is recommended to use a manifest instead of this call.
      ///          - this function should be called once at the beginning of the process to avoid weird behaviors.
      /// @returns success
      static bool setDpiAwareness(bool isEnabled) noexcept;

      /// @brief Read per-monitor DPI (if Win10+ and valid window handle) or system DPI value
      /// @warning The process must be DPI aware (on Windows: requires manifest or calling setDpiAwareness(true))
      uint32_t getMonitorDpi(WindowHandle windowHandle) const noexcept;
      /// @brief Read monitor content scale (based on system DPI)
      /// @warning The process must be DPI aware (on Windows: requires manifest or calling setDpiAwareness(true))
      void getMonitorContentScale(float& factorX, float& factorY) const noexcept;


      //TODO abstraction:
      //getMonitorDpi -> avoir la valeur de r�f�rence -> m�thode static constexpr baseMonitorDpi
      //getMonitorDpi -> avoir version syst�me -> static int getSystemDpi ???
      //getMonitorContentScale -> am�liorer (double? rationnel?) + version avec hwnd ?

      //TODO metrics: 
      // adjust window rect <-> client area (+ version manuelle calcul -> metrics taille bordures+caption+scrollbars+menu)
      // taille pour cr�ation fen�tre = taille_baseX * dpiX / 96.f

    private:
      Handle _handle = (Handle)0;
      Attributes _attributes{ DeviceId{}, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, false };
    };
  }
}
