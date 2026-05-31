#pragma once

#include "RenderContext.h"
#include "../NodeManager.h"

class NodeRenderer {
    public:
        static void Draw(const RenderContext& ctx, NodeManager& nodeManager);
};