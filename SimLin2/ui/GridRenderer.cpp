#include "GridRenderer.h"
#include <cmath>
#include <stdio.h>

void GridRenderer::Draw(const RenderContext& ctx) {

    auto& vp = ctx.viewport;
    auto& cam = ctx.viewport.camera;
    auto& dl = ctx.drawList;

    float left = (ctx.canvasPos.x - vp.origin.x - cam.offset.x) / cam.zoom;
    float right = (ctx.canvasPos.x + ctx.canvasSize.x - vp.origin.x - cam.offset.x) / cam.zoom;
    float top = -(ctx.canvasPos.y - vp.origin.y - cam.offset.y) / cam.zoom;
    float bottom = -(ctx.canvasPos.y + ctx.canvasSize.y - vp.origin.y - cam.offset.y) / cam.zoom;

    //BACKGROUND
    dl->AddRectFilled(
        ctx.canvasPos,
        ImVec2(ctx.canvasPos.x + ctx.canvasSize.x,
            ctx.canvasPos.y + ctx.canvasSize.y),
        IM_COL32(40, 40, 40, 255)
    );

    //ADAPTIVE GRID
    float targetPixelSpacing = 80.0f;
    float rawSpacing = targetPixelSpacing / cam.zoom;

    float pow10 = std::pow(10.0f, std::floor(std::log10(rawSpacing)));
    float normalized = rawSpacing / pow10;

    float snapped;
    if (normalized < 1.5f)      snapped = 1.0f;
    else if (normalized < 3.5f) snapped = 2.0f;
    else if (normalized < 7.5f) snapped = 5.0f;
    else                        snapped = 10.0f;

    float gridSpacing = snapped * pow10;

    //WORLD BOUNDS
    float startX = std::floor(left / gridSpacing) * gridSpacing;
    float startY = std::floor(bottom / gridSpacing) * gridSpacing;


    //DRAW GRID
    for (float x = startX; x <= right; x += gridSpacing)
    {
        ImVec2 a = cam.WorldToScreen({ x, top }, vp.origin);
        ImVec2 b = cam.WorldToScreen({ x, bottom }, vp.origin);

        dl->AddLine(a, b, IM_COL32(60, 60, 60, 255));
    }

    for (float y = startY; y <= top; y += gridSpacing)
    {
        ImVec2 a = cam.WorldToScreen({ left, y }, vp.origin);
        ImVec2 b = cam.WorldToScreen({ right, y }, vp.origin);

        dl->AddLine(a, b, IM_COL32(60, 60, 60, 255));
    }

    //DRAW AXIS
    dl->AddLine(
        cam.WorldToScreen({ left, 0 }, vp.origin),
        cam.WorldToScreen({ right, 0 }, vp.origin),
        IM_COL32(0, 255, 0, 255),
        2.0f
    );

    dl->AddLine(
        cam.WorldToScreen({ 0, top }, vp.origin),
        cam.WorldToScreen({ 0, bottom }, vp.origin),
        IM_COL32(255, 0, 0, 255),
        2.0f
    );

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
        ctx.canvasPos.x + ctx.canvasSize.x - gridSize.x - 10.0f,
        ctx.canvasPos.y + ctx.canvasSize.y - gridSize.y - 40.0f
    );

    dl->AddRectFilled(
        ImVec2(gridPos.x - 6, gridPos.y - 4),
        ImVec2(gridPos.x + gridSize.x + 6, gridPos.y + gridSize.y + 4),
        IM_COL32(20, 20, 20, 180),
        6.0f
    );

    dl->AddText(gridPos, IM_COL32(200, 200, 200, 255), gridText);

    ImGui::InvisibleButton(
        "canvas",
        ctx.canvasSize,
        ImGuiButtonFlags_MouseButtonLeft |
        ImGuiButtonFlags_MouseButtonMiddle |
        ImGuiButtonFlags_MouseButtonRight
    );
}