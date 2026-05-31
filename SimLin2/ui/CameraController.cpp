#include "CameraController.h"
#include <algorithm>

void CameraController::HandlePan(Camera& cam, bool focused, bool dragging, const ImVec2& delta) {
    if (focused && dragging)
    {
        cam.offset.x += delta.x;
        cam.offset.y += delta.y;
    }
}

void CameraController::HandleZoom(Camera& cam, bool focused, float mouseWheel, const ImVec2& origin, const ImVec2& mousePos) {
    if (!focused || mouseWheel == 0.0f) {
        return;
    }

    //World position before zoom
    ImVec2 worldBefore = cam.ScreenToWorld(mousePos, origin);

    //Apply zoom
    float zoomFactor = (mouseWheel > 0) ? 1.1f : (1.0f / 1.1f);
    cam.zoom *= zoomFactor;
    cam.zoom = std::clamp(cam.zoom, 5.0f, 200.0f);

    //World position after zoom
    ImVec2 worldAfter = cam.ScreenToWorld(mousePos, origin);

    //Adjust offset
    cam.offset.x += (worldAfter.x - worldBefore.x) * cam.zoom;
    cam.offset.y += (worldAfter.y - worldBefore.y) * cam.zoom;
}