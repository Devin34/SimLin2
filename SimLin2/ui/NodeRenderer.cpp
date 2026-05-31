#include "NodeRenderer.h"


void NodeRenderer::Draw(const RenderContext& ctx, NodeManager& node_manager) {

    auto& vp = ctx.viewport;
	auto& cam = ctx.viewport.camera;
    auto& dl = ctx.drawList;
    auto& io = ctx.io;

    for (size_t i = 0; i < node_manager.nodes.size(); ++i)
    {
        auto& node = node_manager.nodes[i];

        ImVec2 p = cam.WorldToScreen({ node.x, node.y }, vp.origin);
        const float radius = 6.0f;

        dl->AddCircleFilled(p, radius, IM_COL32(255, 255, 0, 255));

        ImGui::SetCursorScreenPos(ImVec2(p.x - radius, p.y - radius));

        char id[32];
        sprintf_s(id, "##node_%zu", i);

        ImGui::InvisibleButton(id, ImVec2(radius * 2, radius * 2));

        if (ImGui::IsItemHovered() && ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            ImVec2 world = cam.ScreenToWorld(io.MousePos, vp.origin);
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
