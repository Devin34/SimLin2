#pragma once
#include "imgui.h"
#include "Camera.h"


struct Viewport {
    Camera camera;

    ImVec2 origin{ 0.0f, 0.0f };
    ImVec2 size{ 0.0f, 0.0f };

    bool focused = false;
    bool hovered = false;
};