#include "SetupWindow.h"
#include "imgui.h"
#include <cstdio>
#include <imgui_stdlib.h>
#include <algorithm>


int number_of_nodes = 0;
float width = 1000;
float height = 600;

SetupWindow::SetupWindow(NodeManager& node_manager, MaterialManager& material_manager, ElementManager& element_manager) : mNodeManager(node_manager), mMaterialManager(material_manager), mElementManager(element_manager) {}


void SetupWindow::MaterialTable()
{
    ImGui::BeginTable("material_table", 3,
        ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg);



    ImGui::TableSetupColumn("Material", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableSetupColumn("E", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableSetupColumn("Actions", ImGuiTableColumnFlags_WidthFixed, 50.0f);

    // ❌ REMOVE THIS
    // ImGui::TableHeadersRow();

    if (ImGui::BeginPopup("AddMaterialPopup"))
    {
        static std::string name = "Material Name";
        static double E = 0.0;

        ImGui::InputText("Name", &name);
        ImGui::InputDouble("E", &E);

        if (ImGui::Button("Create"))
        {
            mMaterialManager.materials.push_back({ name, E });
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    // ================= HEADER ROW =================
    ImGui::TableNextRow(ImGuiTableRowFlags_Headers);

    // Column 0
    ImGui::TableNextColumn();

    ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_TableHeaderBg));

    if (ImGui::Button("Material (+)"))
        ImGui::OpenPopup("AddMaterialPopup");

    ImGui::PopStyleColor();

    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Click to add a new material.");

    // Column 1
    ImGui::TableNextColumn();
    ImGui::TextUnformatted("MoE");

    // Column 2
    ImGui::TableNextColumn();
    ImGui::TextUnformatted("");

    // ================= DATA ROWS =================
    int deleteIndex = -1;

    for (int i = 0; i < (int)mMaterialManager.materials.size(); i++)
    {
        auto& m = mMaterialManager.materials[i];

        ImGui::TableNextRow();

        ImGui::TableNextColumn();
        ImGui::Text("%s", m.name.c_str());

        ImGui::TableNextColumn();
        ImGui::Text("%g", m.elastic_modulus);

        // Column 3 (Actions)
        ImGui::TableNextColumn();

        bool materialInUse = false;

        for (const auto& element : mElementManager.elements)
        {
            if (element.material == (int)i)
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
        sprintf_s(buttonId, "X##Del_%zu", (size_t)i);

        bool pressed = ImGui::Button(buttonId, ImVec2(-FLT_MIN, 0.0f));

        ImGui::PopStyleColor(3);

        if (materialInUse)
        {
            ImGui::EndDisabled();

            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
            {
                ImGui::SetTooltip("Cannot delete: Material is assigned to one or more elements.");
            }
        }

        if (pressed && !materialInUse)
        {
            deleteIndex = static_cast<int>(i);
        }
    }

    ImGui::EndTable();

    if (deleteIndex != -1)
        mMaterialManager.materials.erase(mMaterialManager.materials.begin() + deleteIndex);
}

void SetupWindow::NodeTable()
{
    if (!ImGui::BeginTable("node_table", 4,
        ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
        return;

    ImGui::TableSetupColumn("Node", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableSetupColumn("X", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableSetupColumn("Y", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableSetupColumn("Actions", ImGuiTableColumnFlags_WidthFixed, 50.0f);

    // ================= HEADER ROW =================
    ImGui::TableNextRow(ImGuiTableRowFlags_Headers);

    // Column 0 (Add Node button)
    ImGui::TableNextColumn();

    ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_TableHeaderBg));

    if (ImGui::Button("Node (+)"))
        ImGui::OpenPopup("AddNodePopup");

    ImGui::PopStyleColor();

    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Click to add a new node.");

    // Popup MUST stay valid outside table flow (same as MaterialTable)
    if (ImGui::BeginPopup("AddNodePopup"))
    {
        static float x = 0.0f, y = 0.0f;

        ImGui::InputFloat("X", &x);
        ImGui::InputFloat("Y", &y);

        if (ImGui::Button("Create"))
        {
            mNodeManager.nodes.push_back({ x, y });
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    // Column 1 header
    ImGui::TableNextColumn();
    ImGui::TextUnformatted("X");

    // Column 2 header
    ImGui::TableNextColumn();
    ImGui::TextUnformatted("Y");

    // Column 3 header
    ImGui::TableNextColumn();
    ImGui::TextUnformatted("");

    // ================= DATA ROWS =================
    int deleteIndex = -1;

    for (int i = 0; i < (int)mNodeManager.nodes.size(); i++)
    {
        const auto& n = mNodeManager.nodes[i];

        ImGui::TableNextRow();

        // Node label
        ImGui::TableNextColumn();
        ImGui::Text("Node %d", i + 1);

        // X
        ImGui::TableNextColumn();
        ImGui::Text("%.3f", n.x);

        // Y
        ImGui::TableNextColumn();
        ImGui::Text("%.3f", n.y);

        // Actions
        ImGui::TableNextColumn();

        ImGui::PushID(i);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.25f, 0.25f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.85f, 0.35f, 0.35f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.2f, 0.2f, 1.0f));

        bool pressed = ImGui::Button("X", ImVec2(-FLT_MIN, 0.0f));

        ImGui::PopStyleColor(3);

        if (pressed)
            deleteIndex = i;

        ImGui::PopID();
    }

    ImGui::EndTable();

    if (deleteIndex != -1)
        mNodeManager.nodes.erase(mNodeManager.nodes.begin() + deleteIndex);
}

void SetupWindow::ElementTable()
{
    if (!ImGui::BeginTable("element_table", 4,
        ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
        return;

    ImGui::TableSetupColumn("Element", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableSetupColumn("Nodes", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableSetupColumn("Material", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableSetupColumn("Actions", ImGuiTableColumnFlags_WidthFixed, 50.0f);

    // ================= HEADER ROW =================
    ImGui::TableNextRow(ImGuiTableRowFlags_Headers);

    ImGui::TableNextColumn();

    ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_TableHeaderBg));

    if (ImGui::Button("Element (+)"))
        ImGui::OpenPopup("AddElementPopup");

    ImGui::PopStyleColor();

    ImGui::TableNextColumn();
    ImGui::TextUnformatted("Nodes");

    ImGui::TableNextColumn();
    ImGui::TextUnformatted("Material");

    ImGui::TableNextColumn();
    ImGui::TextUnformatted("");

    // ================= POPUP =================
    static int node1 = -1;
    static int node2 = -1;
    static int mat = -1;

    if (ImGui::BeginPopup("AddElementPopup"))
    {
        // -------- Node 1 --------
        if (ImGui::Button(node1 >= 0 ? ("Node: " + std::to_string(node1 + 1)).c_str()
            : "Select 1st Node"))
            ImGui::OpenPopup("Node1Popup");

        if (ImGui::BeginPopup("Node1Popup"))
        {
            for (int i = 0; i < (int)mNodeManager.nodes.size(); i++)
            {
                std::string label = "Node " + std::to_string(i + 1);

                bool isSelected = (node1 == i);

                if (ImGui::Selectable(label.c_str(), isSelected))
                    node1 = i;
            }

            ImGui::EndPopup();
        }

        // -------- Node 2 --------
        if (ImGui::Button(node2 >= 0 ? ("Node: " + std::to_string(node2 + 1)).c_str()
            : "Select 2nd Node"))
            ImGui::OpenPopup("Node2Popup");

        if (ImGui::BeginPopup("Node2Popup"))
        {
            for (int i = 0; i < (int)mNodeManager.nodes.size(); i++)
            {
                std::string label = "Node " + std::to_string(i + 1);

                bool isSelected = (node2 == i);

                if (ImGui::Selectable(label.c_str(), isSelected))
                    node2 = i;
            }

            ImGui::EndPopup();
        }

        // -------- Material --------
        if (ImGui::Button(mat >= 0
            ? mMaterialManager.materials[mat].name.c_str()
            : "Select Material"))
        {
            ImGui::OpenPopup("MaterialPopup");
        }

        if (ImGui::BeginPopup("MaterialPopup"))
        {
            for (int i = 0; i < (int)mMaterialManager.materials.size(); i++)
            {
                if (ImGui::Selectable(mMaterialManager.materials[i].name.c_str()))
                    mat = i;
            }
            ImGui::EndPopup();
        }

        // -------- Validation --------
        bool invalid = (node1 < 0 || node2 < 0 || mat < 0 || node1 == node2);

        if (invalid)
            ImGui::BeginDisabled();

        if (ImGui::Button("Create Element"))
        {
            mElementManager.elements.push_back({ node1, node2, mat });
            ImGui::CloseCurrentPopup();
        }

        if (invalid)
            ImGui::EndDisabled();

        ImGui::EndPopup();
    }

    // ================= DATA ROWS =================
    int deleteIndex = -1;

    for (int i = 0; i < (int)mElementManager.elements.size(); i++)
    {
        auto& e = mElementManager.elements[i];

        ImGui::TableNextRow();

        // Element ID
        ImGui::TableNextColumn();
        ImGui::Text("Element %d", i + 1);

        // Nodes
        ImGui::TableNextColumn();
        ImGui::Text("N%d - N%d", e.node1 + 1, e.node2 + 1);

        // Material
        ImGui::TableNextColumn();
        ImGui::TextUnformatted(
            (e.material >= 0 &&
                e.material < (int)mMaterialManager.materials.size())
            ? mMaterialManager.materials[e.material].name.c_str()
            : "Invalid"
        );

        // Actions
        ImGui::TableNextColumn();

        ImGui::PushID(i);
        ImGui::PushStyleColor(ImGuiCol_Button,
            ImVec4(0.7f, 0.25f, 0.25f, 1.0f));

        if (ImGui::Button("X", ImVec2(-FLT_MIN, 0.0f)))
            deleteIndex = i;

        ImGui::PopStyleColor();
        ImGui::PopID();
    }

    ImGui::EndTable();

    if (deleteIndex != -1)
        mElementManager.elements.erase(
            mElementManager.elements.begin() + deleteIndex);
}

void SetupWindow::BCTable() {
    ImGui::Text("Coming soon...");
}

void SetupWindow::ForceTable() {
    ImGui::Text("Coming soon...");
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
