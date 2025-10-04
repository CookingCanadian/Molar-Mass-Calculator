#include "raylib.h"
#include "textAlign.h"
#include "resources/NHG_LIGHT.h"
#include "resources/NHG_MEDIUM.h"

#ifdef _WIN32 // Windows API for rounded corners
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
#endif // _WIN32

#define NATIVE_WIDTH 1600
#define NATIVE_HEIGHT 800
#define MIN_WIDTH 200

#define BACKGROUND_COLOR (Color){41, 44, 49, 255}
Font NHG_LIGHT;
Font NHG_MEDIUM;

void loadFonts();

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT | FLAG_WINDOW_UNDECORATED);
    InitWindow(NATIVE_WIDTH, NATIVE_HEIGHT, "Molar Mass Calculator");
    SetTargetFPS(60);

    #ifdef _WIN32
    HWND hwnd = FindWindowA(nullptr, "Molar Mass Calculator");
    if (hwnd) {
        DWM_WINDOW_CORNER_PREFERENCE preference = DWMWCP_ROUND;
        DwmSetWindowAttribute(hwnd, DWMWA_WINDOW_CORNER_PREFERENCE, &preference, sizeof(preference));
    }
    #endif

    loadFonts();

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(BACKGROUND_COLOR);

            DrawRectangle(0, 0, NATIVE_WIDTH, 50, (Color){31, 34, 39, 255}); // top bar
            DrawRectangleGradientV(0, 0, NATIVE_WIDTH, NATIVE_HEIGHT, (Color){0, 255, 197, 0}, (Color){0, 255, 197, 20}); // background gradient

            DrawTextAlignedAt(NHG_MEDIUM, "Molar Mass Calculator", 20, 25, 24, 0.0f, GRAY, HorizontalAlign::Left, VerticalAlign::Middle);
        EndDrawing();
    }
    
    UnloadFont(NHG_LIGHT);
    UnloadFont(NHG_MEDIUM);
    CloseWindow();
    return 0;
}

void loadFonts() {
    NHG_LIGHT = LoadFontFromMemory(".ttf", font_data_light, sizeof(font_data_light), 24, nullptr, 0);
    NHG_MEDIUM = LoadFontFromMemory(".ttf", font_data_medium, sizeof(font_data_medium), 24, nullptr, 0);

    SetTextureFilter(NHG_LIGHT.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(NHG_MEDIUM.texture, TEXTURE_FILTER_BILINEAR);

    GenTextureMipmaps(&NHG_LIGHT.texture);
    GenTextureMipmaps(&NHG_MEDIUM.texture);
}