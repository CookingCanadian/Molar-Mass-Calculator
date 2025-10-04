#include "raylib.h"

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Molar Mass Calculator");

    SetTargetFPS(60);

    while (!WindowShouldClose()) { 
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Molar Mass Calculator", 20, 20, 24, DARKGRAY);
        DrawText("Press ESC or close window to exit.", 20, 52, 12, GRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
