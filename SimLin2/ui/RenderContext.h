#pragma once


#include "imgui.h"
#include "Viewport.h"


struct RenderContext
{
    Viewport& viewport;

    ImDrawList* drawList;

    ImVec2 canvasPos;
    ImVec2 canvasSize;

    ImGuiIO& io;
};