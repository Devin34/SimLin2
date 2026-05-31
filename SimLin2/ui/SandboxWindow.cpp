#include "SandboxWindow.h"
#include "imgui.h"
#include "../NodeManager.h"
#include <algorithm>
#include <cmath>

#include "CameraController.h"
#include "RenderContext.h"
#include "GridRenderer.h"
#include "NodeRenderer.h"

SandboxWindow::SandboxWindow(NodeManager& node_manager) : node_manager(node_manager), pre_sim_viewport() {
    pre_sim_viewport.camera.offset = ImVec2(0.0f, 0.0f);
    pre_sim_viewport.camera.zoom = 55.0f;
}

void SandboxWindow::UpdateViewport(Viewport& vp, const ImVec2& canvasPos, const ImVec2& canvasSize) {
    vp.size = canvasSize;

    vp.origin = ImVec2(
        canvasPos.x + canvasSize.x * 0.5f,
        canvasPos.y + canvasSize.y * 0.5f);

    vp.focused = ImGui::IsWindowFocused();
}

void SandboxWindow::HandleViewportInput(Viewport& vp, const ImGuiIO& io) {
    CameraController::HandlePan(vp.camera, vp.focused, ImGui::IsMouseDragging(ImGuiMouseButton_Middle), io.MouseDelta);
    CameraController::HandleZoom(vp.camera, vp.focused, io.MouseWheel, vp.origin, io.MousePos);
}

void SandboxWindow::DrawPreSim() {

    ImGuiIO& io = ImGui::GetIO();

    ImVec2 canvasPos = ImGui::GetCursorScreenPos();
    ImVec2 canvasSize = ImGui::GetContentRegionAvail();

    Viewport& pre_sim_vp = pre_sim_viewport;

    UpdateViewport(pre_sim_vp, canvasPos, canvasSize);




    if (canvasSize.x < 50.0f) canvasSize.x = 50.0f;
    if (canvasSize.y < 50.0f) canvasSize.y = 50.0f;

    ImDrawList* drawList = ImGui::GetWindowDrawList();

    RenderContext ctx{
    pre_sim_viewport,
    drawList,
    canvasPos,
    canvasSize,
    io
    };

    HandleViewportInput(pre_sim_viewport, io);

    GridRenderer::Draw(ctx);
    NodeRenderer::Draw(ctx, node_manager);

}

void SandboxWindow::Draw() {
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