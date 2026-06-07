#pragma once

#include <string>
#include <vector>
#include "../NodeManager.h"
#include "MaterialManager.h"

struct ElementData {
    int node1 = -1;
    int node2 = -1;
    int material = -1;

};

class ElementManager {
public:
    std::vector<ElementData> elements;
};