# pragma once

#include <string>
#include <vector>
#include "raylib.h"
#include "text_align.h"

struct FormattedChar {
    char character;
    bool subscript;
    
    FormattedChar(char c = ' ', bool sub = false) 
        : character(c), subscript(sub) {}
};

class TextBox {
private:
    std::string rawText;                 
    std::vector<FormattedChar> formatted; 
    Rectangle bounds;
    bool focused;
    int cursorPos;
    float cursorBlinkTimer;
    
    Color bgColor;
    Color borderColor;
    Color textColor;
    Color placeholderColor;
    
    std::string placeholder;
    Font regularFont;
    Font subscriptFont; 
    float fontSize;
    
    bool autoSubscript; 
    
    void updateFormatting();
    void insertChar(char c);
    void deleteChar();
    void backspace();
    void toggleSubscript(bool makeSubscript);
    
public:
    TextBox(Rectangle rect, Font regular, Font subscript, float size, bool autoSub = true);
    
    void Update();
    void Draw();
    
    std::string GetText() const { return rawText; }
    std::vector<FormattedChar> GetFormatted() const { return formatted; }
    bool IsFocused() const { return focused; }
    
    void SetText(const std::string& text);
    void SetPlaceholder(const std::string& text) { placeholder = text; }
    void SetColors(Color text, Color placeholderCol);
    void SetFocus(bool focus) { focused = focus; }
    void Clear();

    bool Contains(Vector2 point) const;
};
