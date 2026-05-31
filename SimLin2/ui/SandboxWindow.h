#pragma once

#include "imgui.h"
#include <algorithm>
#include "../NodeManager.h"

struct Camera
{
    ImVec2 offset = ImVec2(0, 0);
    float zoom = 55.0f;

    ImVec2 defaultOffset = ImVec2(0.0f, 0.0f);
    float defaultZoom = 1.0f;
};

class SandboxWindow
{
public:
    SandboxWindow(NodeManager& nodeManager);
    void Draw();
    void DrawPreSim();

private:

    Camera mCamera;
    NodeManager& mNodeManager;

    float width = 700;
    float height = 700;
};