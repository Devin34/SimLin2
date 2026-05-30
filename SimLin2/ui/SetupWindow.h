#pragma once
#include "AddNewNodeWindow.h"
#include <string>
#include <vector>

// Define what a node contains
struct NodeData {
    std::string name;
    float x;
    float y;
};

class SetupWindow
{


public:
    void Draw();
    bool show_setup_window = true;
    bool show_add_node_window = false;
    AddNewNodeWindow add_new_node_window;

private:
    int number_of_nodes = 0;
    float width = 1000;
    float height = 600;
    // This vector holds your dynamic list of nodes
    std::vector<NodeData> nodes;
    };