#include "SetupWindow.h"
#include "imgui.h"
#include "AddNewNodeWindow.h"
#include <cstdio>

void SetupWindow::Draw()
{
    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Setup", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
    {
        ImGui::End();
        return;
    }

    if (ImGui::BeginTabBar("MyTabBar")) {
        if (ImGui::BeginTabItem("Node List")) {
            
            ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;

            if (ImGui::BeginTable("button_header_table", 3, flags))
            {
                // 1. Setup columns as usual (do NOT call TableHeadersRow)
                ImGui::TableSetupColumn("Node", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("X", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("Y", ImGuiTableColumnFlags_WidthStretch);

                // 2. Start a custom header row using specific header flags
                ImGui::TableNextRow(ImGuiTableRowFlags_Headers);

                // COLUMN 1: The Button Header
                ImGui::TableNextColumn();
                // Use the native header text color, background, and alignment styles
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_TableHeaderBg));
                if (ImGui::Button("Node (+)"))
                {
                    //show_add_node_window = true;
                    ImGui::OpenPopup("MyStandardPopup");

                }
                if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                {
                    ImGui::SetItemTooltip("Click to add a new node.");
                }
 

                //add_new_node_window.Draw(show_add_node_window);
                if (ImGui::BeginPopup("MyStandardPopup")) {
                    static float val1 = 0.0f;
                    static float val2 = 0.0f;

                    // Create a 2-column table with no borders
                    if (ImGui::BeginTable("AlignedInputs", 2, ImGuiTableFlags_SizingFixedFit)) {

                        ImGui::TableSetupColumn("Labels", ImGuiTableColumnFlags_WidthFixed, 60.0f);
                        ImGui::TableSetupColumn("Inputs", ImGuiTableColumnFlags_WidthFixed, 130.0f);

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

                    ImGui::EndPopup();
                }

                ImGui::PopStyleColor();

                // COLUMN 2: Regular text header
                ImGui::TableNextColumn();
                ImGui::TableHeader("X"); // TableHeader() applies default header formatting

                // COLUMN 3: Regular text header
                ImGui::TableNextColumn();
                ImGui::TableHeader("Y");

                // 3. Populate standard table rows
                for (int i = 0; i < 3; i++)
                {
                    ImGui::TableNextRow();

                    ImGui::TableNextColumn();
                    ImGui::Text("%d", i + 1);

                    ImGui::TableNextColumn();
                    ImGui::Text("Item Name %d", i);

                    ImGui::TableNextColumn();
                    ImGui::Text("Active");
                }
                ImGui::EndTable();
            }
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Element List")) {
            //ImGui::Text("Content for Tab 2");
            // Table flags for adding borders and alternating row background colors
            ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable;

            if (ImGui::BeginTable("advanced_table", 3, flags))
            {
                // Define headers and how columns behave (e.g., fixed size or stretching)
                ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed, 40.0f);
                ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("Status", ImGuiTableColumnFlags_WidthFixed, 100.0f);

                // Render the header row automatically
                ImGui::TableHeadersRow();

                // Populate data
                for (int i = 0; i < 3; i++)
                {
                    ImGui::TableNextRow();

                    // Cell 1: ID
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", i + 1);

                    // Cell 2: Name
                    ImGui::TableNextColumn();
                    ImGui::Text("Item Name %d", i);

                    // Cell 3: Interactive widget inside a cell
                    ImGui::TableNextColumn();
                    char buf[32];
                    sprintf_s(buf, "Active##%d", i); // Ensure unique ID for widgets
                    static bool checked[3] = { true, false, true };
                    ImGui::Checkbox(buf, &checked[i]);
                }
                ImGui::EndTable();
            }

            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }


   ImGui::End();
}