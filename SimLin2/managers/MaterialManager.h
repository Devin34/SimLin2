#pragma once

#include <string>
#include <vector>

struct MaterialData {
    std::string name;
    double elastic_modulus;
};

class MaterialManager {
    public:
        std::vector<MaterialData> materials;
};