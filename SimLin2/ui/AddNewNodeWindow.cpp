#include "AddNewNodeWindow.h"
#include "imgui.h"
#include <cstdio>

void AddNewNodeWindow::Draw(bool& is_open)
{
    if (!is_open) return;

    ImGui::SetNextWindowSize(ImVec2(width, height));

    if (!ImGui::Begin("Add New Node", &is_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
    {
        ImGui::End();
        return;
    }

    static float val1 = 0.0f;
    static float val2 = 0.0f;

    // Create a 2-column table with no borders
    if (ImGui::BeginTable("AlignedInputs", 2)) {

        // --- ROW 1 ---
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("x-pos:"); // Left column

        ImGui::TableSetColumnIndex(1);
        ImGui::SetNextItemWidth(-FLT_MIN); // Fills the remaining column width
        ImGui::InputFloat("##val1", &val1); // Right column

        // --- ROW 2 ---
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("y-pos:");

        ImGui::TableSetColumnIndex(1);
        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::InputFloat("##val2", &val2);

        ImGui::EndTable();
    }
    if (ImGui::Button("Add Node"))
    {
        
    }




    ImGui::End();
}