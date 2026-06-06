#include "SetupWindow.h"
#include "imgui.h"
#include <cstdio>



int number_of_nodes = 0;
float width = 1000;
float height = 600;

SetupWindow::SetupWindow(NodeManager& node_manager) : mNodeManager(node_manager) {}


void SetupWindow::MaterialTable() {
    ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
    if (ImGui::BeginTable("button_header_table", 4, flags)) {

        ImGui::TableSetupColumn("Material", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("E", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Actions", ImGuiTableColumnFlags_WidthFixed, 50.0f);

        ImGui::TableNextRow(ImGuiTableRowFlags_Headers);

        // COLUMN 1: The Button Header
        ImGui::TableNextColumn();

        // PUSH COLOR RIGHT BEFORE WIDGET
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_TableHeaderBg));
        if (ImGui::Button("Material (+)")) {
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

                ImGui::EndTable();
            }

            bool isCurrentlyDuplicate = false;
            for (const auto& node : mNodeManager.nodes) {
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
                newNode.x = val1;
                newNode.y = val2;
                mNodeManager.nodes.push_back(newNode);
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

        ImGui::TableNextColumn();
        ImGui::TableHeader(""); // Empty header for delete buttons

        // Track if any node needs deletion this frame
        int nodeToDeleteIndex = -1;

        // --- DYNAMIC RENDERING FROM VECTOR ---
        for (size_t i = 0; i < mNodeManager.nodes.size(); i++) {
            ImGui::TableNextRow();

            // Column 1: Node Name
            ImGui::TableNextColumn();
            // Inside the render loop:
            ImGui::Text("Node %zu", i + 1); // Dynamically outputs "Node 1", "Node 2", etc.


            // Column 2: X Position
            ImGui::TableNextColumn();
            ImGui::Text("%.3f", mNodeManager.nodes[i].x);

            // Column 3: Y Position
            ImGui::TableNextColumn();
            ImGui::Text("%.3f", mNodeManager.nodes[i].y);

            // Column 4: Delete Action Button
            ImGui::TableNextColumn();

            // Use PushStyleColor to style the delete button a soft red color
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.25f, 0.25f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.85f, 0.35f, 0.35f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.2f, 0.2f, 1.0f));

            // Use the index ##i to give every row's button a strictly unique internal ImGui ID
            char buttonId[32];
            sprintf_s(buttonId, "X##Del_%zu", i);

            if (ImGui::Button(buttonId, ImVec2(-FLT_MIN, 0.0f))) {
                nodeToDeleteIndex = static_cast<int>(i); // Mark this index for deletion
            }
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
                ImGui::SetItemTooltip("Click to delete node.");
            }

            ImGui::PopStyleColor(3); // Remove all 3 button color styles cleanly
        }

        ImGui::EndTable();

        // --- SAFE DELETION POST-RENDER ---
        // We erase the item OUTSIDE the table loop context to avoid container mutation mid-frame
        if (nodeToDeleteIndex != -1) {
            mNodeManager.nodes.erase(mNodeManager.nodes.begin() + nodeToDeleteIndex);
        }
    }
}

void SetupWindow::NodeTable() {
    ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
    if (ImGui::BeginTable("node_table", 4, flags)) {

        ImGui::TableSetupColumn("Node", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("X", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Y", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Actions", ImGuiTableColumnFlags_WidthFixed, 50.0f);

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
            for (const auto& node : mNodeManager.nodes) {
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
                newNode.x = val1;
                newNode.y = val2;
                mNodeManager.nodes.push_back(newNode);
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

        ImGui::TableNextColumn();
        ImGui::TableHeader(""); // Empty header for delete buttons

        // Track if any node needs deletion this frame
        int nodeToDeleteIndex = -1;

        // --- DYNAMIC RENDERING FROM VECTOR ---
        for (size_t i = 0; i < mNodeManager.nodes.size(); i++) {
            ImGui::TableNextRow();

            // Column 1: Node Name
            ImGui::TableNextColumn();
            // Inside the render loop:
            ImGui::Text("Node %zu", i + 1); // Dynamically outputs "Node 1", "Node 2", etc.


            // Column 2: X Position
            ImGui::TableNextColumn();
            ImGui::Text("%.3f", mNodeManager.nodes[i].x);

            // Column 3: Y Position
            ImGui::TableNextColumn();
            ImGui::Text("%.3f", mNodeManager.nodes[i].y);

            // Column 4: Delete Action Button
            ImGui::TableNextColumn();

            // Use PushStyleColor to style the delete button a soft red color
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.25f, 0.25f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.85f, 0.35f, 0.35f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.2f, 0.2f, 1.0f));

            // Use the index ##i to give every row's button a strictly unique internal ImGui ID
            char buttonId[32];
            sprintf_s(buttonId, "X##Del_%zu", i);

            if (ImGui::Button(buttonId, ImVec2(-FLT_MIN, 0.0f))) {
                nodeToDeleteIndex = static_cast<int>(i); // Mark this index for deletion
            }
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
                ImGui::SetItemTooltip("Click to delete node.");
            }

            ImGui::PopStyleColor(3); // Remove all 3 button color styles cleanly
        }

        ImGui::EndTable();

        // --- SAFE DELETION POST-RENDER ---
        // We erase the item OUTSIDE the table loop context to avoid container mutation mid-frame
        if (nodeToDeleteIndex != -1) {
            mNodeManager.nodes.erase(mNodeManager.nodes.begin() + nodeToDeleteIndex);
        }
    }
}

void SetupWindow::ElementTable() {
    ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable;
    if (ImGui::BeginTable("element_table", 3, flags)) {
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
}

void SetupWindow::BCTable() {
    ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable;
    if (ImGui::BeginTable("bc_table", 3, flags)) {
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
}

void SetupWindow::ForceTable() {
    ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable;
    if (ImGui::BeginTable("force_table", 3, flags)) {
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
}

void SetupWindow::Draw() {
    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Setup", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize)) {
        ImGui::End();
        return;
    }


    if (child_windows.mat_editor_window.show_window)
    {
        ImGui::Begin("Material Editor", &child_windows.mat_editor_window.show_window);

        ImGui::Text("Material properties go here.");

        ImGui::End();
    }

    if (ImGui::BeginTabBar("SetupTabBar")) {
        if (ImGui::BeginTabItem("Materials")) {
            
            MaterialTable();

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Nodes")) {
            
            NodeTable();

            ImGui::EndTabItem();
        }


        if (ImGui::BeginTabItem("Elements")) {

            ElementTable();

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Boundary Conditions")) {

            BCTable();

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Forces")) {

            ForceTable();

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
    ImGui::End();
}
