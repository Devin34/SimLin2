#pragma once
#include <vector>
#include <string>
struct NodeData
{
    float x;
    float y;
    std::string name;
};

class NodeManager
{
    public:
        std::vector<NodeData> nodes;
};