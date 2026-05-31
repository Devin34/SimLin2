#pragma once
#include "imgui.h"

class Camera
{
public:
    ImVec2 WorldToScreen(const ImVec2& p, const ImVec2& origin) const;
    ImVec2 ScreenToWorld(const ImVec2& p, const ImVec2& origin) const;

public:
    ImVec2 offset;
    float zoom;
};