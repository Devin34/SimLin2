#pragma once

#include "../../NodeManager.h"
#include "MatEditorWindow.h"
#include "../../managers/MaterialManager.h"
#include "../../managers/ElementManager.h"




class SetupWindow : public Window {


    public:
        int FindNodeIndex(const NodeData& node);
        void MaterialTable();
        void NodeTable();
        void ElementTable();
        void BCTable();
        void ForceTable();
        void Draw() override;


        SetupWindow(NodeManager& node_manager, MaterialManager& material_manager, ElementManager& element_manager);

    private:
        struct ChildWindows {
            MatEditorWindow mat_editor_window;
        };


        NodeManager& mNodeManager;
        MaterialManager& mMaterialManager;
        ElementManager& mElementManager;
        ChildWindows child_windows;


};