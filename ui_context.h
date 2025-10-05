#pragma once

#include "raylib.h"
#include <functional>
#include <vector>

struct UIElement;

class UIContext {
private:
    float nativeWidth;
    float nativeHeight;
    float currentWidth;
    float currentHeight;
    float scale;
    float aspectRatio;
    float minWidth;
    float maxWidth;

    bool draggingResize;
    bool draggingWindow;
    Vector2 dragOffset;
    Vector2 resizeHandlePos;
    float resizeHandleRadius;
    Rectangle topBarRect;

    Vector2 mousePos;
    bool mousePressed;
    bool mouseReleased;
    bool mouseDown;

    struct UIElement {
        Rectangle bounds;  
        std::function<void()> onClick;
        bool enabled;
        int zIndex;  
    };
    
    std::vector<UIElement> elements;
    int hoveredElement;
    
public:
    UIContext(float nativeW, float nativeH, float minW, float maxW);
    
    void Update();
    
    float S(float value) const { return value * scale; }
    float X(float x) const { return x * scale; }
    float Y(float y) const { return y * scale; }
    
    Vector2 Vec(float x, float y) const { return {x * scale, y * scale}; }
    Vector2 Vec(Vector2 v) const { return {v.x * scale, v.y * scale}; }
    
    Rectangle Rect(float x, float y, float w, float h) const { 
        return {x * scale, y * scale, w * scale, h * scale}; 
    }
    Rectangle Rect(Rectangle r) const { 
        return {r.x * scale, r.y * scale, r.width * scale, r.height * scale}; 
    }
    
    // register a clickable element with native coordinates
    int AddElement(float x, float y, float w, float h, std::function<void()> callback, int zIndex = 0);
    int AddElement(Rectangle bounds, std::function<void()> callback, int zIndex = 0);

    void RemoveElement(int elementId);
    void ClearElements();
    
    // enable/disable element
    void SetElementEnabled(int elementId, bool enabled);
    
    // inputs:
    Vector2 GetMousePos() const { return mousePos; }
    bool IsMousePressed() const { return mousePressed; }
    bool IsMouseReleased() const { return mouseReleased; }
    bool IsMouseDown() const { return mouseDown; }
    bool IsMouseOver(float x, float y, float w, float h) const;
    bool IsMouseOver(Rectangle bounds) const;
    bool IsClicked(float x, float y, float w, float h) const;
    bool IsClicked(Rectangle bounds) const;
    
    // window properties:
    float GetScale() const { return scale; }
    float GetWidth() const { return currentWidth; }
    float GetHeight() const { return currentHeight; }
    Vector2 GetWindowSize() const { return {currentWidth, currentHeight}; }
    
    // built in:
    void SetResizeHandle(float x, float y, float radius);
    void SetTopBar(float height);
    void EnableWindowDrag(bool enable);
    void EnableWindowResize(bool enable);
    
private:
    void UpdateMouseState();
    void UpdateWindowDrag();
    void UpdateWindowResize();
    void ProcessElementClicks();
    bool CheckElementHover(const UIElement& element) const;
};