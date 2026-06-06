#pragma once

#include "../../NodeManager.h"
#include "MatEditorWindow.h"




class SetupWindow : public Window {


    public:
        void MaterialTable();
        void NodeTable();
        void ElementTable();
        void BCTable();
        void ForceTable();
        void Draw() override;


        SetupWindow(NodeManager& node_manager);

    private:
        struct ChildWindows {
            MatEditorWindow mat_editor_window;
        };


        NodeManager& mNodeManager;
        ChildWindows child_windows;


};