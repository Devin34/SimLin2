#include "SandboxWindow.h"
#include "imgui.h"
#include "../NodeManager.h"
#include <algorithm>
#include <cmath>

SandboxWindow::SandboxWindow(NodeManager& nodeManager)
    : mNodeManager(nodeManager)
{
}

ImVec2 WorldToScreen(float x, float y, const ImVec2& origin, const Camera& cam)
{
    return ImVec2(
        origin.x + cam.offset.x + x * cam.zoom,
        origin.y + cam.offset.y - y * cam.zoom
    );
}

ImVec2 ScreenToWorld(const ImVec2& p, const ImVec2& origin, const Camera& cam)
{
    return ImVec2(
        (p.x - origin.x - cam.offset.x) / cam.zoom,
        -(p.y - origin.y - cam.offset.y) / cam.zoom
    );
}


void SandboxWindow::DrawPreSim() {

    bool windowFocused = ImGui::IsWindowFocused();
    ImGuiIO& io = ImGui::GetIO();

    ImVec2 canvasPos = ImGui::GetCursorScreenPos();
    ImVec2 canvasSize = ImGui::GetContentRegionAvail();

    if (canvasSize.x < 50.0f) canvasSize.x = 50.0f;
    if (canvasSize.y < 50.0f) canvasSize.y = 50.0f;

    ImVec2 origin(
        canvasPos.x + canvasSize.x * 0.5f,
        canvasPos.y + canvasSize.y * 0.5f
    );

    ImDrawList* drawList = ImGui::GetWindowDrawList();

    // =========================================================
    // CAMERA PAN
    // =========================================================
    if (windowFocused && ImGui::IsMouseDragging(ImGuiMouseButton_Middle))
    {
        mCamera.offset.x += io.MouseDelta.x;
        mCamera.offset.y += io.MouseDelta.y;
    }

    // =========================================================
    // CAMERA ZOOM (ANCHORED)
    // =========================================================
    if (windowFocused && io.MouseWheel != 0.0f)
    {
        float oldZoom = mCamera.zoom;

        float zoomFactor = (io.MouseWheel > 0) ? 1.1f : (1.0f / 1.1f);
        mCamera.zoom *= zoomFactor;
        mCamera.zoom = std::clamp(mCamera.zoom, 5.0f, 200.0f);

        ImVec2 mouse = io.MousePos;

        Camera camBefore = mCamera;
        camBefore.zoom = oldZoom;

        ImVec2 worldBefore = ScreenToWorld(mouse, origin, camBefore);
        ImVec2 worldAfter = ScreenToWorld(mouse, origin, mCamera);

        mCamera.offset.x += (worldAfter.x - worldBefore.x) * mCamera.zoom;
        mCamera.offset.y += (worldAfter.y - worldBefore.y) * mCamera.zoom;
    }

    // =========================================================
    // BACKGROUND
    // =========================================================
    drawList->AddRectFilled(
        canvasPos,
        ImVec2(canvasPos.x + canvasSize.x,
            canvasPos.y + canvasSize.y),
        IM_COL32(40, 40, 40, 255)
    );



    float left = (canvasPos.x - origin.x - mCamera.offset.x) / mCamera.zoom;
    float right = (canvasPos.x + canvasSize.x - origin.x - mCamera.offset.x) / mCamera.zoom;
    float top = -(canvasPos.y - origin.y - mCamera.offset.y) / mCamera.zoom;
    float bottom = -(canvasPos.y + canvasSize.y - origin.y - mCamera.offset.y) / mCamera.zoom;

    // =========================================================
    // ADAPTIVE GRID (1–2–5 RULE)
    // =========================================================
    float targetPixelSpacing = 80.0f;
    float rawSpacing = targetPixelSpacing / mCamera.zoom;

    float pow10 = std::pow(10.0f, std::floor(std::log10(rawSpacing)));
    float normalized = rawSpacing / pow10;

    float snapped;
    if (normalized < 1.5f)      snapped = 1.0f;
    else if (normalized < 3.5f) snapped = 2.0f;
    else if (normalized < 7.5f) snapped = 5.0f;
    else                        snapped = 10.0f;

    float gridSpacing = snapped * pow10;

    // =========================================================
    // WORLD BOUNDS (for stable grid)
    // =========================================================

    float startX = std::floor(left / gridSpacing) * gridSpacing;
    float startY = std::floor(bottom / gridSpacing) * gridSpacing;

    // =========================================================
    // GRID DRAW
    // =========================================================
    for (float x = startX; x <= right; x += gridSpacing)
    {
        ImVec2 a = WorldToScreen(x, top, origin, mCamera);
        ImVec2 b = WorldToScreen(x, bottom, origin, mCamera);

        drawList->AddLine(a, b, IM_COL32(60, 60, 60, 255));
    }

    for (float y = startY; y <= top; y += gridSpacing)
    {
        ImVec2 a = WorldToScreen(left, y, origin, mCamera);
        ImVec2 b = WorldToScreen(right, y, origin, mCamera);

        drawList->AddLine(a, b, IM_COL32(60, 60, 60, 255));
    }

    // =========================================================
    // AXES
    // =========================================================
    drawList->AddLine(
        WorldToScreen(left, 0, origin, mCamera),
        WorldToScreen(right, 0, origin, mCamera),
        IM_COL32(0, 255, 0, 255),
        2.0f
    );

    drawList->AddLine(
        WorldToScreen(0, top, origin, mCamera),
        WorldToScreen(0, bottom, origin, mCamera),
        IM_COL32(255, 0, 0, 255),
        2.0f
    );

    const float buttonMargin = 10.0f;

    // measure button size first
    ImVec2 buttonSize = ImGui::CalcTextSize("Reset Camera");
    buttonSize.x += ImGui::GetStyle().FramePadding.x * 2.0f;
    buttonSize.y += ImGui::GetStyle().FramePadding.y * 2.0f;

    // position in bottom-right of canvas
    ImVec2 buttonPos(
        canvasPos.x + canvasSize.x - buttonSize.x - buttonMargin,
        canvasPos.y + canvasSize.y - buttonSize.y - buttonMargin
    );

    ImGui::SetCursorScreenPos(buttonPos);

    if (ImGui::Button("Reset Camera"))
    {
        mCamera.zoom = 55.0f;
        mCamera.offset = ImVec2(0.0f, 0.0f);
    }

    //const float margin = 10.0f;

    // format zoom nicely (avoid ugly floating values)
    //char zoomText[32];
    //sprintf_s(zoomText, "Zoom: %.0f%%", ((mCamera.zoom / 55.0f) * 100.0f));

    // measure text
    //ImVec2 textSize = ImGui::CalcTextSize(zoomText);

    //ImVec2 zoomPos(
    //    canvasPos.x + canvasSize.x - textSize.x - margin,
    //    canvasPos.y + canvasSize.y - textSize.y - margin - 30.0f // offset above button
    //);

    // optional background for readability
    //ImDrawList* dl = ImGui::GetWindowDrawList();

    //ImVec2 bgMin = ImVec2(zoomPos.x - 6, zoomPos.y - 4);
    //ImVec2 bgMax = ImVec2(zoomPos.x + textSize.x + 6, zoomPos.y + textSize.y + 4);

    //dl->AddRectFilled(bgMin, bgMax, IM_COL32(20, 20, 20, 180), 6.0f);

    // draw text
    //dl->AddText(zoomPos, IM_COL32(255, 255, 255, 255), zoomText);

    char gridText[64];

    if (gridSpacing >= 1000.0f)
    {
        sprintf_s(gridText, "Grid: %.1fk", gridSpacing / 1000.0f);
    }
    else
    {
        sprintf_s(gridText, "Grid Size: %.1f units", gridSpacing);
    }

    ImVec2 gridSize = ImGui::CalcTextSize(gridText);

    ImVec2 gridPos(
        canvasPos.x + canvasSize.x - gridSize.x - 10.0f,
        canvasPos.y + canvasSize.y - gridSize.y - 40.0f
    );

    drawList->AddRectFilled(
        ImVec2(gridPos.x - 6, gridPos.y - 4),
        ImVec2(gridPos.x + gridSize.x + 6, gridPos.y + gridSize.y + 4),
        IM_COL32(20, 20, 20, 180),
        6.0f
    );

    drawList->AddText(gridPos, IM_COL32(200, 200, 200, 255), gridText);


    // =========================================================
    // CANVAS INPUT GATE
    // =========================================================
    ImGui::InvisibleButton(
        "canvas",
        canvasSize,
        ImGuiButtonFlags_MouseButtonLeft |
        ImGuiButtonFlags_MouseButtonMiddle |
        ImGuiButtonFlags_MouseButtonRight
    );



    bool canvasHovered = ImGui::IsItemHovered();

    // =========================================================
    // NODES
    // =========================================================
    for (size_t i = 0; i < mNodeManager.nodes.size(); ++i)
    {
        auto& node = mNodeManager.nodes[i];

        ImVec2 p = WorldToScreen(node.x, node.y, origin, mCamera);
        const float radius = 6.0f;

        drawList->AddCircleFilled(p, radius, IM_COL32(255, 255, 0, 255));

        ImGui::SetCursorScreenPos(ImVec2(p.x - radius, p.y - radius));

        char id[32];
        sprintf_s(id, "##node_%zu", i);

        ImGui::InvisibleButton(id, ImVec2(radius * 2, radius * 2));

        if (canvasHovered &&
            ImGui::IsItemActive() &&
            ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            ImVec2 world = ScreenToWorld(io.MousePos, origin, mCamera);
            node.x = world.x;
            node.y = world.y;
        }

        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(
                "Node %zu\nX = %.3f\nY = %.3f",
                i + 1,
                node.x,
                node.y
            );
        }
    }
}

void SandboxWindow::Draw()
{
    ImGui::SetNextWindowSize(ImVec2(width, height));

    if (!ImGui::Begin("Sandbox", nullptr,
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar))
    {
        ImGui::End();
        return;
    }


    if (ImGui::BeginTabBar("SandboxTabs"))
    {
        if (ImGui::BeginTabItem("Pre-Sim"))
        {
            DrawPreSim();   // we will move your current code here
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Displacement"))
        {
            ImGui::Text("Coming soon...");
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Von Mises"))
        {
            ImGui::Text("Coming soon...");
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }


    

    ImGui::End();
}