#ifndef TEXTALIGN_H
#define TEXTALIGN_H

#include "raylib.h"

enum class VerticalAlign {
    Top,
    Middle,
    Bottom
};

enum class HorizontalAlign {
    Left,
    Center,
    Right
};

void DrawTextAligned(Font font, const char* text, Rectangle bounds, // in bounding rectangle
                     float fontSize, float spacing, Color color,
                     HorizontalAlign hAlign, VerticalAlign vAlign);

void DrawTextAlignedAt(Font font, const char* text, float x, float y, // via x, y
                       float fontSize, float spacing, Color color,
                       HorizontalAlign hAlign, VerticalAlign vAlign);

#endif // TEXTALIGN_H
