#pragma once
#include <vector>

struct NodeData
{
    float x;
    float y;
};

class NodeManager
{
    public:
        std::vector<NodeData> nodes;
};