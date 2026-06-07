#pragma once

#include <string>
#include <vector>
#include "../NodeManager.h"
#include "MaterialManager.h"

struct ElementData {
    std::string name;
    NodeData node1;
    NodeData node2;
    MaterialData material;

};

class ElementManager {
public:
    std::vector<ElementData> elements;
};