#include "SetupWindow.h"
#include "imgui.h"
#include "AddNewNodeWindow.h"
#include <cstdio>

void SetupWindow::Draw() {
    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Setup", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize)) {
        ImGui::End();
        return;
    }

    if (ImGui::BeginTabBar("MyTabBar")) {
        if (ImGui::BeginTabItem("Node List")) {
            ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
            if (ImGui::BeginTable("button_header_table", 3, flags)) {

                ImGui::TableSetupColumn("Node", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("X", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("Y", ImGuiTableColumnFlags_WidthStretch);

                ImGui::TableNextRow(ImGuiTableRowFlags_Headers);

                // COLUMN 1: The Button Header
                ImGui::TableNextColumn();

                // PUSH COLOR RIGHT BEFORE WIDGET
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_TableHeaderBg));
                if (ImGui::Button("Node (+)")) {
                    ImGui::OpenPopup("MyStandardPopup");
                }
                ImGui::PopStyleColor(); // POP IT IMMEDIATELY TO KEEP THE STACK SAFE

                if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
                    ImGui::SetItemTooltip("Click to add a new node.");
                }

                // Clean window resetting logic when the popup appears
                if (ImGui::BeginPopup("MyStandardPopup")) {
                    static float val1 = 0.0f;
                    static float val2 = 0.0f;

                    if (ImGui::BeginTable("AlignedInputs", 2, ImGuiTableFlags_SizingFixedFit)) {
                        ImGui::TableSetupColumn("Labels", ImGuiTableColumnFlags_WidthFixed, 60.0f);
                        ImGui::TableSetupColumn("Inputs", ImGuiTableColumnFlags_WidthFixed, 130.0f);

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0); ImGui::Text("x-pos:");
                        ImGui::TableSetColumnIndex(1); ImGui::SetNextItemWidth(-FLT_MIN);
                        ImGui::InputFloat("##val1", &val1);

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0); ImGui::Text("y-pos:");
                        ImGui::TableSetColumnIndex(1); ImGui::SetNextItemWidth(-FLT_MIN);
                        ImGui::InputFloat("##val2", &val2);

                        ImGui::EndTable();
                    }

                    bool isCurrentlyDuplicate = false;
                    for (const auto& node : nodes) {
                        if (node.x == val1 && node.y == val2) {
                            isCurrentlyDuplicate = true;
                            break;
                        }
                    }

                    if (isCurrentlyDuplicate) {
                        ImGui::BeginDisabled();
                    }
                    if (ImGui::Button("Add Node")) {
                        NodeData newNode;
                        newNode.name = "Node " + std::to_string(nodeCounter++);
                        newNode.x = val1;
                        newNode.y = val2;
                        nodes.push_back(newNode);
                        ImGui::CloseCurrentPopup();
                    }
                    if (isCurrentlyDuplicate) {
                        ImGui::EndDisabled();
                    }

                    if (isCurrentlyDuplicate) {
                        ImGui::Spacing();
                        ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "Error: Node already exists at this position!");
                    }
                    ImGui::EndPopup();
                }

                // COLUMN 2: Regular text header
                ImGui::TableNextColumn();
                ImGui::TableHeader("X");

                // COLUMN 3: Regular text header
                ImGui::TableNextColumn();
                ImGui::TableHeader("Y");

                // --- DYNAMIC RENDERING FROM VECTOR ---
                for (size_t i = 0; i < nodes.size(); i++) {
                    ImGui::TableNextRow();

                    ImGui::TableNextColumn();
                    ImGui::Text("%s", nodes[i].name.c_str());

                    ImGui::TableNextColumn();
                    ImGui::Text("%.3f", nodes[i].x);

                    ImGui::TableNextColumn();
                    ImGui::Text("%.3f", nodes[i].y);
                }
                ImGui::EndTable();
            }
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Element List")) {
            ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable;
            if (ImGui::BeginTable("advanced_table", 3, flags)) {
                ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed, 40.0f);
                ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("Status", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                ImGui::TableHeadersRow();

                for (int i = 0; i < 3; i++) {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", i + 1);
                    ImGui::TableNextColumn();
                    ImGui::Text("Item Name %d", i);
                    ImGui::TableNextColumn();

                    char buf[32];
                    sprintf_s(buf, "Active##%d", i);
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
