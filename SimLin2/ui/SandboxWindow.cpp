#include "SandboxWindow.h"
#include "imgui.h"

void SandboxWindow::Draw()
{
    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Sandbox", &show_sandbox_window, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
    {
        ImGui::End();
        return;
    }


    ImVec2 canvasPos = ImGui::GetCursorScreenPos();
    ImVec2 canvasSize = ImGui::GetContentRegionAvail();

    if (canvasSize.x < 50.0f) canvasSize.x = 50.0f;
    if (canvasSize.y < 50.0f) canvasSize.y = 50.0f;


    ImDrawList* drawList = ImGui::GetWindowDrawList();

    // Background
    drawList->AddRectFilled(
        canvasPos,
        ImVec2(canvasPos.x + canvasSize.x,
            canvasPos.y + canvasSize.y),
        IM_COL32(40, 40, 40, 255));

    // Grid
    for (float x = canvasPos.x; x < canvasPos.x + canvasSize.x; x += 25.0f)
    {
        drawList->AddLine(
            ImVec2(x, canvasPos.y),
            ImVec2(x, canvasPos.y + canvasSize.y),
            IM_COL32(60, 60, 60, 255));
    }

    for (float y = canvasPos.y; y < canvasPos.y + canvasSize.y; y += 25.0f)
    {
        drawList->AddLine(
            ImVec2(canvasPos.x, y),
            ImVec2(canvasPos.x + canvasSize.x, y),
            IM_COL32(60, 60, 60, 255));
    }

    // Reserve the space so ImGui knows the canvas exists
    ImGui::InvisibleButton("canvas", canvasSize);

    ImGui::End();
}