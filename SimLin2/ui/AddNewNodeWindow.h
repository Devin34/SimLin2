#pragma once

class AddNewNodeWindow
{
public:
    void Draw(bool& is_open);
    bool show_add_new_node_window = false;

private:
    float width = 200;
    float height = 150;
};