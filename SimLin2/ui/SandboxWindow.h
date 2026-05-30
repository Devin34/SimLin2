#pragma once

class SandboxWindow
{
public:
    void Draw();
    bool show_sandbox_window = true;

private:
    float width = 600;
    float height = 600;
    float mGridSpacing = 25.0f;
    float mZoom = 1.0f;
};