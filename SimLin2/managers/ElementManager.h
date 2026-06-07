#pragma once

#include <string>
#include <vector>
#include "../NodeManager.h"
#include "MaterialManager.h"

struct ElementData {
    std::string name;
    NodeData start_node;
    NodeData end_node;
    MaterialData material;

};

class ElementManager {
public:
    std::vector<ElementData> elements;
};