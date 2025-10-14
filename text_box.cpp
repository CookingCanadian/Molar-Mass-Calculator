#include "text_box.h"
#include <cctype>

TextBox::TextBox(Rectangle rect, Font regular, Font subscript, float size, bool autoSub)
    : bounds(rect), focused(false), cursorPos(0), cursorBlinkTimer(0.0f),
      regularFont(regular), subscriptFont(subscript), fontSize(size),
      autoSubscript(autoSub) {
    textColor = {225, 244, 242, 255};
    placeholderColor = {120, 140, 135, 255};
}

void TextBox::SetColors(Color text, Color placeholderCol) {
    textColor = text;
    placeholderColor = placeholderCol;
}

void TextBox::updateFormatting() {
    formatted.clear();
    
    if (!autoSubscript) {
        for (char c : rawText) {
            formatted.push_back(FormattedChar(c, false));
        }
        return;
    }
    
    for (size_t i = 0; i < rawText.length(); i++) {
        char c = rawText[i];
        
        if (std::isdigit(c) && i > 0 && std::isalpha(rawText[i - 1])) {
            formatted.push_back(FormattedChar(c, true));
        } else {
            formatted.push_back(FormattedChar(c, false));
        }
    }
}

void TextBox::insertChar(char c) {
    if (std::isalnum(c) || c == '(' || c == ')') {
        rawText.insert(cursorPos, 1, c);
        
        bool shouldBeSubscript = false;
        if (autoSubscript && std::isdigit(c) && cursorPos > 0 && std::isalpha(rawText[cursorPos - 1])) {
            shouldBeSubscript = true;
        }
        
        FormattedChar newChar(c, shouldBeSubscript);
        formatted.insert(formatted.begin() + cursorPos, newChar);
        
        cursorPos++;
    }
}

void TextBox::deleteChar() {
    if (cursorPos < (int)rawText.length()) {
        rawText.erase(cursorPos, 1);
        formatted.erase(formatted.begin() + cursorPos);
    }
}

void TextBox::backspace() {
    if (cursorPos > 0) {
        rawText.erase(cursorPos - 1, 1);
        formatted.erase(formatted.begin() + cursorPos - 1);
        cursorPos--;
    }
}

void TextBox::toggleSubscript(bool makeSubscript) {
    if (cursorPos <= 0) return;
    
    int endPos = cursorPos - 1;
    char endChar = rawText[endPos];
    
    if (!std::isdigit(endChar)) return;
    
    int startPos = endPos;
    while (startPos > 0 && std::isdigit(rawText[startPos - 1])) {
        startPos--;
    }
    
    bool hasLetterBefore = (startPos > 0 && std::isalpha(rawText[startPos - 1]));
    
    if (makeSubscript) {
        if (hasLetterBefore) {
            for (int i = startPos; i <= endPos; i++) {
                formatted[i].subscript = true;
            }
        }
    } else {
        for (int i = startPos; i <= endPos; i++) {
            formatted[i].subscript = false;
        }
    }
}

void TextBox::Update() {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        focused = Contains(GetMousePosition());
        if (focused) {
            cursorBlinkTimer = 0.0f;
        }
    }
    
    if (!focused) return;
    
    cursorBlinkTimer += GetFrameTime();
    if (cursorBlinkTimer > 1.0f) cursorBlinkTimer = 0.0f;
    
    int key = GetCharPressed();
    while (key > 0) {
        if (key >= 32 && key <= 126) {
            insertChar((char)key);
        }
        key = GetCharPressed();
    }
    
    if (IsKeyPressed(KEY_BACKSPACE)) {
        backspace();
    }
    if (IsKeyPressed(KEY_DELETE)) {
        deleteChar();
    }
    if (IsKeyPressed(KEY_LEFT) && cursorPos > 0) {
        cursorPos--;
    }
    if (IsKeyPressed(KEY_RIGHT) && cursorPos < (int)rawText.length()) {
        cursorPos++;
    }
    if (IsKeyPressed(KEY_HOME)) {
        cursorPos = 0;
    }
    if (IsKeyPressed(KEY_END)) {
        cursorPos = (int)rawText.length();
    }
    
    if (IsKeyPressed(KEY_UP)) {
        toggleSubscript(false);
    }
    if (IsKeyPressed(KEY_DOWN)) {
        toggleSubscript(true);
    }
}

void TextBox::Draw() {
    DrawRectangleRounded(bounds, 0.1f, 8, bgColor);    
    
    float x = bounds.x + 12;
    float y = bounds.y + bounds.height / 2;
    
    if (rawText.empty()) {
        Vector2 textSize = MeasureTextEx(regularFont, placeholder.c_str(), fontSize, 0.0f);
        float textY = y - textSize.y / 2;
        DrawTextEx(regularFont, placeholder.c_str(), {x, textY}, fontSize, 0.0f, placeholderColor);
        
        if (focused && cursorBlinkTimer < 0.5f) {
            DrawLine((int)x, (int)(y - fontSize/2), 
                    (int)x, (int)(y + fontSize/2), textColor);
        }
    } else {
        float cursorX = x;
        
        for (size_t i = 0; i < formatted.size(); i++) {
            FormattedChar fc = formatted[i];
            std::string charStr(1, fc.character);
            
            Font activeFont = fc.subscript ? subscriptFont : regularFont;
            float activeFontSize = fc.subscript ? fontSize * 0.6f : fontSize;
            
            Vector2 textSize = MeasureTextEx(activeFont, charStr.c_str(), activeFontSize, 0.0f);
            float charY = fc.subscript ? y + (fontSize * 0.2f) : y - textSize.y / 2;
            
            if ((int)i == cursorPos && focused && cursorBlinkTimer < 0.5f) {    
                DrawLine((int)cursorX, (int)(y - fontSize/2), 
                        (int)cursorX, (int)(y + fontSize/2), textColor);
            }
            
            DrawTextEx(activeFont, charStr.c_str(), {cursorX, charY}, activeFontSize, 0.0f, textColor);
            cursorX += textSize.x;
        }       

        if ((int)cursorPos == (int)formatted.size() && focused && cursorBlinkTimer < 0.5f) {
            DrawLine((int)cursorX, (int)(y - fontSize/2), 
                    (int)cursorX, (int)(y + fontSize/2), textColor);
        }
    }
}

bool TextBox::Contains(Vector2 point) const {
    return point.x >= bounds.x && point.x <= bounds.x + bounds.width &&
           point.y >= bounds.y && point.y <= bounds.y + bounds.height;
}

void TextBox::SetText(const std::string& text) {
    rawText = text;
    cursorPos = rawText.length();
    updateFormatting();
}

void TextBox::Clear() {
    rawText.clear();
    formatted.clear();
    cursorPos = 0;
}