#include <cstdlib>
#include <cstdio>
#include "raylib.h"
#include "rlgl.h"
#include "text_align.h"
#include "window_utils.h"
#include "ui_context.h"
#include "text_box.h"
#include "element_data.h"
#include "resources/NOTO_SYMBOLS.h"
#include "resources/ROBOTO_REGULAR.h"
#include "resources/ROBOTO_MEDIUM.h"
#include "resources/ROBOTO_BOLD.h"

#define NATIVE_WIDTH 1600
#define NATIVE_HEIGHT 800
#define MIN_WIDTH 400
#define MAX_WIDTH 1600

#define TEXT_LIGHT (Color){225, 244, 242, 255}
#define TEXT_DARK (Color){71, 83, 82, 255}
#define BUTTON_NORMAL (Color){125, 145, 142, 255}
#define BUTTON_HOVER (Color){105, 125, 122, 255}

Font NOTO_SYMBOLS;

Font ROBOTO_REGULAR;
Font ROBOTO_MEDIUM;
Font ROBOTO_BOLD;

void loadFonts();

struct CalculationHistory {
    std::string formula;
    double molarMass;
};

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT | FLAG_WINDOW_UNDECORATED);
    InitWindow(NATIVE_WIDTH, NATIVE_HEIGHT, "Molar Mass Calculator");
    SetTargetFPS(60);

    SetWindowRoundedCorners("Molar Mass Calculator");
    
    UIContext ui(NATIVE_WIDTH, NATIVE_HEIGHT, MIN_WIDTH, MAX_WIDTH);
    
    ui.SetResizeHandle(NATIVE_WIDTH - 20, NATIVE_HEIGHT - 20, 8.0f);
    ui.SetTopBar(60);

    std::vector<CalculationHistory> history;
    const int MAX_HISTORY = 10;

    loadFonts();

    TextBox formulaInput(
        Rectangle{ui.X(600), ui.Y(350), ui.S(800), ui.S(50)}, 
        ROBOTO_MEDIUM,
        ROBOTO_MEDIUM,  
        ui.S(24)
    );
    formulaInput.SetPlaceholder("Enter molecular formula...");
    formulaInput.SetColors(
        TEXT_LIGHT,                  
        (Color){120, 140, 135, 255}   
    );

    // dropdown state
    bool dropdownOpen = false;
    int selectedDecimal = 1; // 0: 0.1, 1: 0.01, 2: 0.001
    const char* decimalValues[] = {"0.1", "0.01", "0.001"};

    while (!WindowShouldClose()) {
        ui.Update();      
        formulaInput.Update();
        
        // Button coordinates
        Rectangle calculateBtn = {ui.X(1360), ui.Y(410), ui.S(30), ui.S(30)};
        Rectangle decimalBtn = {ui.X(1250), ui.Y(410), ui.S(100), ui.S(30)};
        Rectangle subscriptBtn = {ui.X(610), ui.Y(410), ui.S(30), ui.S(30)};
        Rectangle clearBtn = {ui.X(650), ui.Y(410), ui.S(70), ui.S(30)};
        
        // Dropdown menu calculations
        Rectangle dropdownMenu = {decimalBtn.x, decimalBtn.y + decimalBtn.height, decimalBtn.width, 0};
        float dropdownItemHeight = ui.S(30);
        dropdownMenu.height = dropdownItemHeight * 3; // 3 items
        
        Rectangle dropdownItems[3];
        for (int i = 0; i < 3; i++) {
            dropdownItems[i] = {decimalBtn.x, decimalBtn.y + decimalBtn.height + (i * dropdownItemHeight), decimalBtn.width, dropdownItemHeight};
        }

        // dropdown interactions
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(GetMousePosition(), decimalBtn)) {
                dropdownOpen = !dropdownOpen;
            } else if (dropdownOpen) {
                bool itemSelected = false;
                for (int i = 0; i < 3; i++) {
                    if (CheckCollisionPointRec(GetMousePosition(), dropdownItems[i])) {
                        selectedDecimal = i;
                        dropdownOpen = false;
                        itemSelected = true;
                        break;
                    }
                }
                if (!itemSelected) {
                    dropdownOpen = false;
                }
            }
        }
        
        // button bindings 
        ui.AddElement(calculateBtn, [&formulaInput, &selectedDecimal, &history]() {
            std::string formula = formulaInput.GetFormattedText();              
           
            if (formula.empty()) {              
                return;
            }           
            
            double molarMass = CalculateMolarMass(formula);  
            // add to recent queue
            history.insert(history.begin(), {formula, molarMass});
            if (history.size() > MAX_HISTORY) {
                history.pop_back();
            }
            
            // format based on decimal precision selection
            const char* formatStr[] = {"%.1f", "%.2f", "%.3f"};
        });
        ui.AddElement(subscriptBtn, [&formulaInput]() {
            formulaInput.ToggleSubscript(true);
        });
        ui.AddElement(clearBtn, [&formulaInput]() {
            formulaInput.Clear();
        });
        
        // hover bindings
        Color calculateBtnClr = (ui.IsMouseOver(calculateBtn))
            ? (Color){28, 157, 126, 255}   // hover
            : (Color){48, 177, 146, 255}; // normal
        Color decimalBtnClr = (ui.IsMouseOver(decimalBtn) || dropdownOpen)
            ? BUTTON_HOVER 
            : BUTTON_NORMAL; 
        Color subscriptBtnClr = (ui.IsMouseOver(subscriptBtn))
            ? BUTTON_HOVER   
            : BUTTON_NORMAL; 
        Color clearBtnClr = (ui.IsMouseOver(clearBtn))
            ? BUTTON_HOVER   
            : BUTTON_NORMAL; 

        BeginDrawing();
            ClearBackground((Color){41, 44, 49, 255});

            DrawRectangle(0, 0, (int)ui.GetWidth(), (int)ui.S(60), (Color){31, 34, 39, 255}); // top bar
            
            DrawRectangleGradientV(0, (int)ui.GetHeight() - ui.Y(600), (int)ui.GetWidth(), (int)ui.Y(600), (Color){0, 255, 197, 0}, (Color){0, 255, 197, 20}); // background gradient

            DrawCircle((int)ui.GetWidth() - ui.X(20), (int)ui.GetHeight() - ui.Y(20), ui.S(8.0f), (Color){31, 43, 41, 255}); // resize circle

            DrawRectangle(0, (int)ui.Y(60), (int)ui.S(400), (int)ui.GetHeight() - ui.Y(60), (Color){39, 42, 47, 200}); // history backing
            DrawLine((int)ui.X(400), (int)ui.Y(60), (int)ui.X(400), (int)ui.GetHeight(), (Color){58, 62, 66, 255}); // history divider line

            DrawRectangleRounded((Rectangle){ui.X(600), ui.Y(350), ui.S(800), ui.S(100)}, 0.24f, 8, (Color){151, 172, 169, 255}); // search box background
            DrawRectangleRounded(calculateBtn, 0.4f, 6, calculateBtnClr); // calculate background button
            DrawTextAligned(NOTO_SYMBOLS, "➤", calculateBtn, ui.S(32), 0.0f, TEXT_LIGHT, HorizontalAlign::Center, VerticalAlign::Middle); // calculate icon
            
            DrawRectangleRounded(decimalBtn, 0.4f, 6, decimalBtnClr); // decimal place adjust button
            DrawTextAligned(ROBOTO_MEDIUM, TextFormat("\t%s", decimalValues[selectedDecimal]), decimalBtn, ui.S(20), 0.0f, TEXT_DARK, HorizontalAlign::Left, VerticalAlign::Middle);
            DrawTextAligned(NOTO_SYMBOLS, "▼\t", decimalBtn, ui.S(24), 0.0f, TEXT_DARK, HorizontalAlign::Right, VerticalAlign::Bottom);

            DrawRectangleRounded(subscriptBtn, 0.4f, 6, subscriptBtnClr); // subscript button
            DrawTextAligned(ROBOTO_MEDIUM, "X₂", subscriptBtn, ui.S(20), 0.0f, TEXT_DARK, HorizontalAlign::Center, VerticalAlign::Middle);

            DrawRectangleRounded(clearBtn, 0.4f, 6, clearBtnClr); // clear button
            DrawTextAligned(ROBOTO_MEDIUM, "CLEAR", clearBtn, ui.S(20), 0.0f, TEXT_DARK, HorizontalAlign::Center, VerticalAlign::Middle);

            DrawTextAlignedAt(ROBOTO_MEDIUM, "Molecular Formla", ui.X(600), ui.Y(346), ui.S(24), 0.0f, (Color){102, 129, 127, 255}, HorizontalAlign::Left, VerticalAlign::Bottom);

            DrawTextAlignedAt(ROBOTO_BOLD, "Recent Data", ui.X(20), ui.Y(70), ui.S(24), 0.0f, (Color){81, 91, 110, 255}, HorizontalAlign::Left, VerticalAlign::Top);

            DrawTextAlignedAt(ROBOTO_BOLD, "Molar Mass Calculator", ui.X(20), ui.Y(30), ui.S(30), 0.0f, (Color){50, 56, 66, 255}, HorizontalAlign::Left, VerticalAlign::Middle);

            formulaInput.Draw();

            if (dropdownOpen) { // dropdown menu
                DrawRectangleRounded(dropdownMenu, 0.2f, 6, BUTTON_HOVER); 
                
                for (int i = 0; i < 3; i++) {
                    Color itemColor = (i == selectedDecimal)
                        ? BUTTON_NORMAL // selected
                        : BUTTON_HOVER; // normal
                    
                    // draw item background
                    DrawRectangle(
                        (int)dropdownItems[i].x, 
                        (int)dropdownItems[i].y, 
                        (int)dropdownItems[i].width, 
                        (int)dropdownItems[i].height, 
                        itemColor
                    );
                    
                    // draw hover effect
                    if (CheckCollisionPointRec(GetMousePosition(), dropdownItems[i])) {
                        DrawRectangle(
                            (int)dropdownItems[i].x, 
                            (int)dropdownItems[i].y, 
                            (int)dropdownItems[i].width, 
                            (int)dropdownItems[i].height, 
                            BUTTON_NORMAL
                        );
                    }
                    
                    // draw text
                    DrawTextAligned(ROBOTO_MEDIUM, TextFormat("\t%s", decimalValues[i]), dropdownItems[i], ui.S(18), 0.0f, TEXT_DARK, HorizontalAlign::Left, VerticalAlign::Middle);
                }
            }

            float historyY = ui.Y(100);
            for (size_t i = 0; i < history.size(); i++) {            
                DrawTextAlignedAt(ROBOTO_BOLD, history[i].formula.c_str(), ui.X(20), historyY, ui.S(18), 0.0f, TEXT_LIGHT, HorizontalAlign::Left, VerticalAlign::Top); // formula
                
                // get decimal precision for display
                const char* formatStr[] = {"%.1f", "%.2f", "%.3f"};
                const char* molarMassStr = TextFormat(formatStr[selectedDecimal], history[i].molarMass);
                
                // draw molar mass in medium below formula
                DrawTextAlignedAt(ROBOTO_MEDIUM, TextFormat("%s g/mol", molarMassStr), ui.X(20), historyY + ui.S(22), ui.S(16), 0.0f, (Color){102, 129, 127, 255}, HorizontalAlign::Left, VerticalAlign::Top);
                
                historyY += ui.S(50);
                
                DrawLine((int)ui.X(20), (int)historyY - ui.S(8), (int)ui.X(380), (int)historyY - ui.S(8), (Color){58, 62, 66, 255}); // seperator line
            }
        EndDrawing();
    }
    
    UnloadFont(NOTO_SYMBOLS);
    UnloadFont(ROBOTO_REGULAR);
    UnloadFont(ROBOTO_MEDIUM);
    UnloadFont(ROBOTO_BOLD);
    CloseWindow();
    return 0;
}

void loadFonts() {
    // Noto Symbols: ASCII + arrows/symbols only
    int notoAsciiCount = 95;
    int notoSymbols = 2; // ➤ and ▼
    int notoTotalCount = notoAsciiCount + notoSymbols;
    
    int *notoCodepoints = (int *)malloc(notoTotalCount * sizeof(int));
    
    // ASCII printable characters (32-126)
    for (int i = 0; i < notoAsciiCount; i++) {
        notoCodepoints[i] = 32 + i;
    }
    
    // extra symbols for Noto
    notoCodepoints[notoAsciiCount + 0] = 0x27A4; // ➤ right arrow
    notoCodepoints[notoAsciiCount + 1] = 0x25BC; // ▼ down triangle
    
    NOTO_SYMBOLS = LoadFontFromMemory(".ttf", font_data_symbol, sizeof(font_data_symbol), 40, notoCodepoints, notoTotalCount);
    
    free(notoCodepoints);
    
    int robotoAsciiCount = 95;
    int subscriptCount = 10;
    int robotoTotalCount = robotoAsciiCount + subscriptCount;
    
    int *robotoCodepoints = (int *)malloc(robotoTotalCount * sizeof(int));
    
    // ASCII printable characters (32-126)
    for (int i = 0; i < robotoAsciiCount; i++) {
        robotoCodepoints[i] = 32 + i;
    }
    
    // subscript digits (U+2080 to U+2089): ₀₁₂₃₄₅₆₇₈₉
    for (int i = 0; i < subscriptCount; i++) {
        robotoCodepoints[robotoAsciiCount + i] = 0x2080 + i;
    }
    
    ROBOTO_REGULAR = LoadFontFromMemory(".ttf", font_data_regular, sizeof(font_data_regular), 40, robotoCodepoints, robotoTotalCount);
    ROBOTO_MEDIUM = LoadFontFromMemory(".ttf", font_data_medium, sizeof(font_data_medium), 40, robotoCodepoints, robotoTotalCount);
    ROBOTO_BOLD = LoadFontFromMemory(".ttf", font_data_bold, sizeof(font_data_bold), 40, robotoCodepoints, robotoTotalCount);
    
    free(robotoCodepoints);

    SetTextureFilter(NOTO_SYMBOLS.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(ROBOTO_REGULAR.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(ROBOTO_MEDIUM.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(ROBOTO_BOLD.texture, TEXTURE_FILTER_BILINEAR);

    GenTextureMipmaps(&NOTO_SYMBOLS.texture);
    GenTextureMipmaps(&ROBOTO_REGULAR.texture);
    GenTextureMipmaps(&ROBOTO_MEDIUM.texture);
    GenTextureMipmaps(&ROBOTO_BOLD.texture);
}