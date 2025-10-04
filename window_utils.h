#pragma once

#ifdef _WIN32  // Windows API for rounded corners
    typedef int WINBOOL;
    typedef void* HWND;
    typedef long HRESULT;

    extern "C" {
        HWND FindWindowA(const char* lpClassName, const char* lpWindowName);
        HRESULT DwmSetWindowAttribute(HWND hwnd, unsigned long dwAttribute, const void* pvAttribute, unsigned long cbAttribute);
    }

    #define DWMWA_WINDOW_CORNER_PREFERENCE 33


    enum DWM_WINDOW_CORNER_PREFERENCE {
        DWMWCP_DEFAULT      = 0,
        DWMWCP_DONOTROUND   = 1,
        DWMWCP_ROUND        = 2,
        DWMWCP_ROUNDSMALL   = 3
    };

    inline void SetWindowRoundedCorners(const char* windowTitle, DWM_WINDOW_CORNER_PREFERENCE preference = DWMWCP_ROUND) {
        HWND hwnd = FindWindowA(nullptr, windowTitle);
        if (hwnd) {
            DwmSetWindowAttribute(hwnd, DWMWA_WINDOW_CORNER_PREFERENCE, &preference, sizeof(preference));
        }
    }
#else
    inline void SetWindowRoundedCorners(const char* windowTitle, int preference = 0) {
        // maybe other platform support
    }
#endif // _WIN32