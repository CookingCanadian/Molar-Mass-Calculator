#include "raylib.h"
#include "text_align.h"
#include "window_utils.h"
#include "ui_context.h"
#include "resources/ROBOTO_REGULAR.h"

#define NATIVE_WIDTH 1600
#define NATIVE_HEIGHT 800
#define MIN_WIDTH 400
#define MAX_WIDTH 1600

Font ROBOTO_REGULAR;

void loadFonts();

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT | FLAG_WINDOW_UNDECORATED);
    InitWindow(NATIVE_WIDTH, NATIVE_HEIGHT, "Molar Mass Calculator");
    SetTargetFPS(60);

    SetWindowRoundedCorners("Molar Mass Calculator");
    
    UIContext ui(NATIVE_WIDTH, NATIVE_HEIGHT, MIN_WIDTH, MAX_WIDTH);
    
    ui.SetResizeHandle(NATIVE_WIDTH - 20, NATIVE_HEIGHT - 20, 8.0f);
    ui.SetTopBar(60);
    
    loadFonts();
    
    while (!WindowShouldClose()) {
        ui.Update();        
        
        BeginDrawing();
            ClearBackground((Color){41, 44, 49, 255});

            DrawRectangle(0, 0, (int)ui.GetWidth(), (int)ui.S(60), (Color){31, 34, 39, 255}); // top bar

            DrawRectangleGradientV(0, (int)ui.GetHeight() - ui.Y(600), (int)ui.GetWidth(), (int)ui.Y(600), (Color){0, 255, 197, 0}, (Color){0, 255, 197, 20}); // background gradient

            DrawCircle((int)ui.GetWidth() - ui.X(20), (int)ui.GetHeight() - ui.Y(20), ui.S(8.0f), (Color){31, 43, 41, 255}); // resize circle

            DrawRectangle(0, (int)ui.Y(60), (int)ui.S(400), (int)ui.GetHeight() - ui.Y(60), (Color){39, 42, 47, 200}); // history backing
            DrawLine((int)ui.X(400), (int)ui.Y(60), (int)ui.X(400), (int)ui.GetHeight(), (Color){58, 62, 66, 255}); // history divider line

            DrawRectangleRounded((Rectangle){ui.X(600), ui.Y(350), ui.S(800), ui.S(100)}, 0.24f, 8, (Color){151, 172, 169, 255}); // search box background
            DrawRectangleRounded((Rectangle){ui.X(1360), ui.Y(410), ui.S(30), ui.S(30)}, 0.4f, 6, (Color){48, 177, 146, 255}); // calculate icon
            DrawTextAligned(ROBOTO_REGULAR, "=", (Rectangle){ui.X(1360), ui.Y(410), ui.S(30), ui.S(30)}, 24.0f, 0.0f, WHITE, HorizontalAlign::Center, VerticalAlign::Middle); // replace w/ NHG_REGULAR sometime in the future
            
            
            DrawTextAlignedAt(ROBOTO_REGULAR, "Molar Mass Calculator", ui.X(20), ui.Y(30), ui.S(30), 0.0f, GRAY, HorizontalAlign::Left, VerticalAlign::Middle);
            
        EndDrawing();
    }
    
    UnloadFont(ROBOTO_REGULAR);
    CloseWindow();
    return 0;
}

void loadFonts() {
    ROBOTO_REGULAR = LoadFontFromMemory(".ttf", font_data_regular, sizeof(font_data_regular), 40, nullptr, 0);


    SetTextureFilter(ROBOTO_REGULAR.texture, TEXTURE_FILTER_BILINEAR);


    GenTextureMipmaps(&ROBOTO_REGULAR.texture);
}