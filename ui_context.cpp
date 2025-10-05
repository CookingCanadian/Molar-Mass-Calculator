#include "ui_context.h"
#include <algorithm>

UIContext::UIContext(float nativeW, float nativeH, float minW, float maxW)
    : nativeWidth(nativeW), nativeHeight(nativeH),
      currentWidth(nativeW), currentHeight(nativeH),
      scale(1.0f),
      aspectRatio(nativeW / nativeH),
      minWidth(minW), maxWidth(maxW),
      draggingResize(false), draggingWindow(false),
      dragOffset{0, 0},
      resizeHandlePos{nativeW - 20.0f, nativeH - 20.0f},
      resizeHandleRadius(20.0f),
      topBarRect{0, 0, nativeW, 60.0f},
      mousePos{0, 0},
      mousePressed(false), mouseReleased(false), mouseDown(false),
      hoveredElement(-1) {}

void UIContext::Update() {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    currentWidth = (float)screenWidth;
    currentHeight = (float)screenHeight;
    scale = currentWidth / nativeWidth;
    
    UpdateMouseState();
    UpdateWindowDrag();
    UpdateWindowResize();

    ProcessElementClicks();
}

void UIContext::UpdateMouseState() {
    mousePos = GetMousePosition();
    mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    mouseReleased = IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
    mouseDown = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
}

void UIContext::UpdateWindowDrag() {
    Rectangle scaledTopBar = Rect(topBarRect);
    
    if (mousePressed) {
        if (CheckCollisionPointRec(mousePos, scaledTopBar) && !draggingResize) {
            draggingWindow = true;
            dragOffset = mousePos;
        }
    }
    
    if (mouseReleased) {
        draggingWindow = false;
    }
    
    if (draggingWindow && mouseDown) {
        Vector2 currentMousePos = GetMousePosition();
        Vector2 mouseDelta = {
            currentMousePos.x - dragOffset.x,
            currentMousePos.y - dragOffset.y
        };
        
        Vector2 windowPos = GetWindowPosition();
        int newX = (int)(windowPos.x + mouseDelta.x);
        int newY = (int)(windowPos.y + mouseDelta.y);
        
        SetWindowPosition(newX, newY);
    }
}

void UIContext::UpdateWindowResize() {
    Vector2 scaledHandlePos = Vec(resizeHandlePos);
    
    if (mousePressed) {
        if (CheckCollisionPointCircle(mousePos, scaledHandlePos, resizeHandleRadius)) {
            draggingResize = true;
        }
    }
    
    if (mouseReleased) {
        draggingResize = false;
    }
    
    if (draggingResize && mouseDown) {
        float newWidth = mousePos.x + 20.0f;
        newWidth = std::max(minWidth, std::min(newWidth, maxWidth));
        float newHeight = newWidth / aspectRatio;
        
        SetWindowSize((int)newWidth, (int)newHeight);
    }
}

void UIContext::ProcessElementClicks() {
    hoveredElement = -1;
    
    if (!mousePressed) return;
    

    std::vector<int> sortedIndices; // sort by z-index
    for (int i = 0; i < (int)elements.size(); i++) {
        sortedIndices.push_back(i);
    }
    
    std::sort(sortedIndices.begin(), sortedIndices.end(), 
        [this](int a, int b) { return elements[a].zIndex > elements[b].zIndex; });
    
    for (int idx : sortedIndices) {
        const UIElement& elem = elements[idx];
        if (elem.enabled && CheckElementHover(elem)) {
            hoveredElement = idx;
            if (elem.onClick) {
                elem.onClick();
            }
            break; // topmost element
        }
    }
}

bool UIContext::CheckElementHover(const UIElement& element) const {
    return CheckCollisionPointRec(mousePos, element.bounds);
}

int UIContext::AddElement(float x, float y, float w, float h, std::function<void()> callback, int zIndex) {
    return AddElement({x, y, w, h}, callback, zIndex);
}

int UIContext::AddElement(Rectangle bounds, std::function<void()> callback, int zIndex) {
    UIElement elem;
    elem.bounds = Rect(bounds);
    elem.onClick = callback;
    elem.enabled = true;
    elem.zIndex = zIndex;
    
    elements.push_back(elem);
    return (int)elements.size() - 1;
}

void UIContext::RemoveElement(int elementId) {
    if (elementId >= 0 && elementId < (int)elements.size()) {
        elements.erase(elements.begin() + elementId);
    }
}

void UIContext::ClearElements() {
    elements.clear();
    hoveredElement = -1;
}

void UIContext::SetElementEnabled(int elementId, bool enabled) {
    if (elementId >= 0 && elementId < (int)elements.size()) {
        elements[elementId].enabled = enabled;
    }
}

bool UIContext::IsMouseOver(float x, float y, float w, float h) const {
    return IsMouseOver({x, y, w, h});
}

bool UIContext::IsMouseOver(Rectangle bounds) const {
    Rectangle scaledBounds = Rect(bounds);
    return CheckCollisionPointRec(mousePos, scaledBounds);
}

bool UIContext::IsClicked(float x, float y, float w, float h) const {
    return IsClicked({x, y, w, h});
}

bool UIContext::IsClicked(Rectangle bounds) const {
    Rectangle scaledBounds = Rect(bounds);
    return mousePressed && CheckCollisionPointRec(mousePos, scaledBounds);
}

void UIContext::SetResizeHandle(float x, float y, float radius) {
    resizeHandlePos = {x, y};
    resizeHandleRadius = radius;
}

void UIContext::SetTopBar(float height) {
    topBarRect = {0, 0, nativeWidth, height};
}

void UIContext::EnableWindowDrag(bool enable) {
    if (!enable) {
        draggingWindow = false;
    }
}

void UIContext::EnableWindowResize(bool enable) {
    if (!enable) {
        draggingResize = false;
    }
}