#include "text_align.h"

void DrawTextAligned(Font font, const char* text, Rectangle bounds, // rectangle bounding alignment
                     float fontSize, float spacing, Color color,
                     HorizontalAlign hAlign, VerticalAlign vAlign)
{
    Vector2 textSize = MeasureTextEx(font, text, fontSize, spacing);
    Vector2 pos = { bounds.x, bounds.y };

    switch (hAlign) {
        case HorizontalAlign::Center: pos.x += (bounds.width - textSize.x) / 2.0f; break;
        case HorizontalAlign::Right:  pos.x += bounds.width - textSize.x; break;
        default: break;
    }

    switch (vAlign) {
        case VerticalAlign::Middle: pos.y += (bounds.height - textSize.y) / 2.0f - fontSize * 0.1f; break;
        case VerticalAlign::Bottom: pos.y += bounds.height - textSize.y; break;
        default: break;
    }

    DrawTextEx(font, text, pos, fontSize, spacing, color);
}

void DrawTextAlignedAt(Font font, const char* text, float x, float y, // point alignment
                       float fontSize, float spacing, Color color,
                       HorizontalAlign hAlign, VerticalAlign vAlign)
{
    Vector2 textSize = MeasureTextEx(font, text, fontSize, spacing);
    Vector2 pos = { x, y };


    switch (hAlign) { 
        case HorizontalAlign::Center: pos.x -= textSize.x / 2.0f; break;
        case HorizontalAlign::Right:  pos.x -= textSize.x; break;
        default: break;
    }


    switch (vAlign) {
        case VerticalAlign::Middle: pos.y -= textSize.y / 2.0f - fontSize * 0.1f; break;
        case VerticalAlign::Bottom: pos.y -= textSize.y; break;
        default: break;
    }

    DrawTextEx(font, text, pos, fontSize, spacing, color);
}