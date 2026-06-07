#include "SetupWindow.h"
#include "imgui.h"
#include <cstdio>
#include <imgui_stdlib.h>
#include <algorithm>


int number_of_nodes = 0;
float width = 1000;
float height = 600;

SetupWindow::SetupWindow(NodeManager& node_manager, MaterialManager& material_manager, ElementManager& element_manager) : mNodeManager(node_manager), mMaterialManager(material_manager), mElementManager(element_manager) {}

int SetupWindow::FindNodeIndex(const NodeData& node)
{
    for (size_t i = 0; i < mNodeManager.nodes.size(); i++)
    {
        if (mNodeManager.nodes[i].x == node.x &&
            mNodeManager.nodes[i].y == node.y)
        {
            return static_cast<int>(i);
        }
    }

    return -1;
}


void SetupWindow::MaterialTable() {
    ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
    if (ImGui::BeginTable("material_table", 3, flags)) {

        ImGui::TableSetupColumn("Material", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("E", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Actions", ImGuiTableColumnFlags_WidthFixed, 50.0f);

        ImGui::TableNextRow(ImGuiTableRowFlags_Headers);

        // COLUMN 1: The Button Header
        ImGui::TableNextColumn();

        // PUSH COLOR RIGHT BEFORE WIDGET
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_TableHeaderBg));
        if (ImGui::Button("Material (+)")) {
            ImGui::OpenPopup("AddMaterialPopup");
        }
        ImGui::PopStyleColor(); // POP IT IMMEDIATELY TO KEEP THE STACK SAFE

        if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
            ImGui::SetItemTooltip("Click to add a new material.");
        }

        // Clean window resetting logic when the popup appears
        if (ImGui::BeginPopup("AddMaterialPopup")) {
            static std::string val1 = "Material Name";
            static double val2 = 0.0;

            if (ImGui::BeginTable("AlignedInputs", 2, ImGuiTableFlags_SizingFixedFit)) {
                ImGui::TableSetupColumn("Labels", ImGuiTableColumnFlags_WidthFixed, 150.0f);
                ImGui::TableSetupColumn("Inputs", ImGuiTableColumnFlags_WidthFixed, 130.0f);

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("Material Name:");
                ImGui::TableSetColumnIndex(1); ImGui::SetNextItemWidth(-FLT_MIN);
                ImGui::InputText("##val1", &val1, ImGuiInputTextFlags_AutoSelectAll);

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("EoM:");
                ImGui::TableSetColumnIndex(1); ImGui::SetNextItemWidth(-FLT_MIN);
                ImGui::InputDouble("##val2", &val2);

                ImGui::EndTable();
            }

            bool isCurrentlyDuplicate = false;

            for (const auto& material : mMaterialManager.materials) {
                if (material.name == val1) {
                    isCurrentlyDuplicate = true;
                    break;
                }
            }




            bool isNameEmpty = val1.empty();
            bool isNameSpaces = std::all_of(val1.begin(), val1.end(), [](unsigned char c) {return std::isspace(c);});

            if (isCurrentlyDuplicate || isNameEmpty || isNameSpaces) {
                ImGui::BeginDisabled();
            }
            if (ImGui::Button("Add Material")) {
                MaterialData newMaterial;
                newMaterial.name = val1;
                newMaterial.elastic_modulus = val2;
                mMaterialManager.materials.push_back(newMaterial);
                ImGui::CloseCurrentPopup();
            }
            if (isCurrentlyDuplicate) {
                ImGui::EndDisabled();
                ImGui::Spacing();
                ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "Error: Material already exists with this name!");
            }

            if (isNameEmpty || isNameSpaces) {
                ImGui::EndDisabled();
                ImGui::Spacing();
                ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "Error: Material must have a name!");
            }

            ImGui::EndPopup();
        }

        // COLUMN 2: Regular text header
        ImGui::TableNextColumn();
        ImGui::TableHeader("MoE");

        if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
            ImGui::SetItemTooltip("Modulus of Elasticity");
        }

        ImGui::TableNextColumn();
        ImGui::TableHeader(""); // Empty header for delete buttons

        // Track if any node needs deletion this frame
        int materialToDeleteIndex = -1;

        // --- DYNAMIC RENDERING FROM VECTOR ---
        for (size_t i = 0; i < mMaterialManager.materials.size(); i++) {
            ImGui::TableNextRow();

            const auto& material = mMaterialManager.materials[i];

            // Column 1
            ImGui::TableNextColumn();
            ImGui::Text("%s", material.name.c_str());

            // Column 2
            ImGui::TableNextColumn();
            ImGui::Text("%g", material.elastic_modulus);

            // Column 3 (Actions)
            ImGui::TableNextColumn();


            bool materialInUse = false;

            for (const auto& element : mElementManager.elements)
            {
                if (element.material.name == material.name)
                {
                    materialInUse = true;
                    break;
                }
            }

            if (materialInUse)
            {
                ImGui::BeginDisabled();
            }

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.25f, 0.25f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.85f, 0.35f, 0.35f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.2f, 0.2f, 1.0f));

            char buttonId[32];
            sprintf_s(buttonId, "X##Del_%zu", i);

            if (ImGui::Button(buttonId, ImVec2(-FLT_MIN, 0.0f)))
            {
                materialToDeleteIndex = static_cast<int>(i);
            }

            ImGui::PopStyleColor(3);

            if (materialInUse)
            {
                ImGui::EndDisabled();

                if (ImGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip))
                {
                    ImGui::SetTooltip("Cannot delete: Material is assigned to one or more elements.");
                }
            }


        }

        ImGui::EndTable();


        if (materialToDeleteIndex != -1) {
            mMaterialManager.materials.erase(mMaterialManager.materials.begin() + materialToDeleteIndex);
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
            ImGui::OpenPopup("AddNodePopup");
        }
        ImGui::PopStyleColor(); // POP IT IMMEDIATELY TO KEEP THE STACK SAFE

        if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
            ImGui::SetItemTooltip("Click to add a new node.");
        }

        // Clean window resetting logic when the popup appears
        if (ImGui::BeginPopup("AddNodePopup")) {
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
            //mNodeManager.nodes[i].name = ("Node %zu", i + 1);

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
    ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
    if (ImGui::BeginTable("element_table", 4, flags)) {

        ImGui::TableSetupColumn("Element", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Nodes", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Material", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Actions", ImGuiTableColumnFlags_WidthFixed, 50.0f);

        ImGui::TableNextRow(ImGuiTableRowFlags_Headers);

        // COLUMN 1: The Button Header
        ImGui::TableNextColumn();

        // PUSH COLOR RIGHT BEFORE WIDGET
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_TableHeaderBg));
        if (ImGui::Button("Element (+)")) {
            ImGui::OpenPopup("AddElementPopup");
        }
        ImGui::PopStyleColor(); // POP IT IMMEDIATELY TO KEEP THE STACK SAFE

        if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
            ImGui::SetItemTooltip("Click to add a new element.");
        }

        // Clean window resetting logic when the popup appears
        if (ImGui::BeginPopup("AddElementPopup")) {
            static MaterialData material_data;
            static int selectedMaterial = -1;
            static NodeData node1_data;
            static NodeData node2_data;
            static int selectedNode1 = -1;
            static int selectedNode2 = -1;

            if (ImGui::BeginTable("AlignedInputs", 2, ImGuiTableFlags_SizingFixedFit)) {
                ImGui::TableSetupColumn("Labels", ImGuiTableColumnFlags_WidthFixed, 150.0f);
                ImGui::TableSetupColumn("Inputs", ImGuiTableColumnFlags_WidthFixed, 130.0f);

                //Node 1 row
                // Node 1
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Start Node:");

                 const char* node1Label =
                    (selectedNode1 >= 0)
                    ? ("Node " + std::to_string(selectedNode1 + 1)).c_str()
                    : "Select Node 1";

                if (ImGui::Button(node1Label, ImVec2(-FLT_MIN, 0.0f)))

                    ImGui::OpenPopup("Node1Popup");

                if (ImGui::BeginPopup("Node1Popup"))
                {
                    for (size_t i = 0; i < mNodeManager.nodes.size(); i++)
                    {
                        char label[32];
                        sprintf_s(label, "Node %zu", i + 1);

                        if (ImGui::Selectable(label))
                        {
                            selectedNode1 = (int)i;
                            node1_data = mNodeManager.nodes[i];
                            ImGui::CloseCurrentPopup();
                        }
                    }
                    ImGui::EndPopup();
                }

                //Node 2 row
                // Node 1
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("End Node:");


                const char* node2Label =
                    (selectedNode2 >= 0)
                    ? ("Node " + std::to_string(selectedNode2 + 1)).c_str()
                    : "Select Node 2";

                if (ImGui::Button(node2Label, ImVec2(-FLT_MIN, 0.0f)))
                    ImGui::OpenPopup("Node2Popup");

                if (ImGui::BeginPopup("Node2Popup"))
                {
                    for (size_t i = 0; i < mNodeManager.nodes.size(); i++)
                    {
                        char label[32];
                        sprintf_s(label, "Node %zu", i + 1);

                        if (ImGui::Selectable(label))
                        {
                            selectedNode2 = (int)i;
                            node2_data = mNodeManager.nodes[i];
                            ImGui::CloseCurrentPopup();
                        }
                    }
                    ImGui::EndPopup();
                }

                // Material row
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Assign Material:");

                const char* buttonText = (selectedMaterial >= 0) ? mMaterialManager.materials[selectedMaterial].name.c_str() : "Select Material";

                if (ImGui::Button(buttonText, ImVec2(-FLT_MIN, 0.0f)))
                {
                    ImGui::OpenPopup("MaterialPopup");
                }

                if (ImGui::BeginPopup("MaterialPopup"))
                {
                    for (size_t i = 0; i < mMaterialManager.materials.size(); i++)
                    {
                        bool isSelected =
                            (selectedMaterial == static_cast<int>(i));

                        if (ImGui::Selectable(
                            mMaterialManager.materials[i].name.c_str(),
                            isSelected))
                        {
                            selectedMaterial = static_cast<int>(i);
                            material_data = mMaterialManager.materials[i];
                            ImGui::CloseCurrentPopup();
                        }
                    }

                    ImGui::EndPopup();
                }

                ImGui::EndTable();
            }


            bool noMaterialSelected = (selectedMaterial < 0);
            bool noNode1Selected = (selectedNode1 < 0);
            bool noNode2Selected = (selectedNode2 < 0);
            bool sameNode = (selectedNode1 == selectedNode2);
            
            if (noMaterialSelected || noNode1Selected || noNode2Selected || sameNode) {
                ImGui::BeginDisabled();
            }
            if (ImGui::Button("Add Element")) {
                ElementData newElement;
                newElement.node1 = node1_data;
                newElement.node2 = node2_data;
                newElement.material = material_data;
                mElementManager.elements.push_back(newElement);
                ImGui::CloseCurrentPopup();
            }

            if (noMaterialSelected) {
                ImGui::EndDisabled();
                ImGui::Spacing();
                ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "Error: Element must have a material!");
            }


            ImGui::EndPopup();
        }

        // COLUMN 2: Regular text header
        ImGui::TableNextColumn();
        ImGui::TableHeader("Nodes");

        ImGui::TableNextColumn();
        ImGui::TableHeader("Material");

        ImGui::TableNextColumn();
        ImGui::TableHeader(""); // Empty header for delete buttons

        // Track if any node needs deletion this frame
        int elementToDeleteIndex = -1;

        // --- DYNAMIC RENDERING FROM VECTOR ---
        for (size_t i = 0; i < mElementManager.elements.size(); i++) {
            ImGui::TableNextRow();

            auto& element = mElementManager.elements[i];

            // Column 1: Node Name
            ImGui::TableNextColumn();
            // Inside the render loop:
            ImGui::Text("Element %zu", i + 1); // Dynamically outputs "Node 1", "Node 2", etc.
            //mElementManager.elements[i].name = ("Element %zu", i + 1);


            // Column 2: X Position
            ImGui::TableNextColumn();
            ImGui::Text("N%d - N%d",
                FindNodeIndex(element.node1) + 1,
                FindNodeIndex(element.node2) + 1);

            // Material
            ImGui::TableNextColumn();



            // isolate row ID so popup is unique per element
            ImGui::PushID((int)i);

            const char* label =
                element.material.name.empty()
                ? "Select Material"
                : element.material.name.c_str();

            if (ImGui::Selectable(label))
            {
                ImGui::OpenPopup("MaterialSelectPopup");
            }

            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
            {
                ImGui::SetTooltip("Click to change material.");
            }

            if (ImGui::BeginPopup("MaterialSelectPopup"))
            {
                for (size_t m = 0; m < mMaterialManager.materials.size(); m++)
                {
                    const auto& mat = mMaterialManager.materials[m];

                    if (ImGui::Selectable(mat.name.c_str(),
                        element.material.name == mat.name))
                    {
                        element.material = mat;
                        ImGui::CloseCurrentPopup();
                    }
                    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                    {
                        ImGui::SetTooltip("Elastic Modulus: %.3f", mat.elastic_modulus);
                    }
                }
                ImGui::EndPopup();
            }

            ImGui::PopID();

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
                elementToDeleteIndex = static_cast<int>(i); // Mark this index for deletion
            }
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
                ImGui::SetItemTooltip("Click to delete element.");
            }

            ImGui::PopStyleColor(3); // Remove all 3 button color styles cleanly
        }

        ImGui::EndTable();

        if (elementToDeleteIndex != -1) {
            mElementManager.elements.erase(mElementManager.elements.begin() + elementToDeleteIndex);
        }
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
