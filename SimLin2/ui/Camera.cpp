#include "Camera.h"
#include <algorithm>

ImVec2 Camera::WorldToScreen(const ImVec2& p, const ImVec2& origin) const {
    return {
        origin.x + offset.x + p.x * zoom,
        origin.y + offset.y - p.y * zoom
    };
}

ImVec2 Camera::ScreenToWorld(const ImVec2& p, const ImVec2& origin) const {
    return {
        (p.x - origin.x - offset.x) / zoom,
        -(p.y - origin.y - offset.y) / zoom
    };
}