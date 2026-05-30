#pragma once
#include "AddNewNodeWindow.h"


class SetupWindow
{
public:
    void Draw();
    bool show_setup_window = true;
    bool show_add_node_window = false;
    AddNewNodeWindow add_new_node_window;

private:
    float width = 1000;
    float height = 600;
    float mGridSpacing = 25.0f;
    float mZoom = 1.0f;
};