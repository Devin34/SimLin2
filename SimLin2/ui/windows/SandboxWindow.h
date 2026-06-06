#pragma once

#include "../../NodeManager.h"
#include "../Viewport.h"

//struct Camera
//{
  //  ImVec2 offset = ImVec2(0, 0);
    //float zoom = 55.0f;

    //ImVec2 defaultOffset = ImVec2(0.0f, 0.0f);
    //float defaultZoom = 1.0f;
//};

class SandboxWindow
{
    public:
        SandboxWindow(NodeManager& node_manager);
        void UpdateViewport(Viewport& vp, const ImVec2& canvasPos, const ImVec2& canvasSize);
        void HandleViewportInput(Viewport& vp, const ImGuiIO& io);
        void Draw();
        void DrawPreSim();

    private:

        //Camera mCamera;
        Viewport pre_sim_viewport;
        NodeManager& node_manager;

        float width = 700;
        float height = 700;
};