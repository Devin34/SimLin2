#pragma once
#include "Camera.h"
#include "imgui.h"

class CameraController {
    public:
        static void HandlePan(Camera& cam, bool focused, bool dragging, const ImVec2& delta);
        static void HandleZoom(Camera& cam, bool focused, float mouseWheel, const ImVec2& origin, const ImVec2& mousePos);
};