#include "ConfigUI.hpp"
#include "ConfigTree.hpp"
#include "MathUtil.hpp"
#include <raylib.h>
#include <sstream>
#include <iomanip>

// Helper to convert ConfigValue to string
static std::string ValueToString(const ConfigValue &config)
{
    switch (config.type)
    {
    case ValueType::Int:
        return std::to_string(std::get<int>(config.value));
    case ValueType::Float:
    {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2) << std::get<float>(config.value);
        return ss.str();
    }
    case ValueType::Bool:
        return std::get<bool>(config.value) ? "true" : "false";
    }
    return "ERR";
}

// Draws the config UI and updates pending edits map
void DrawConfigUI(Node &root, ConfigUIState &state, float contentTop, float viewH, float contentW)
{
    // height of each item
    const float itemHeight = 30.0f;
    // gap between items
    const float gap = 6.0f;
    // horizontal margin for content
    const float marginX = 16.0f;

    // field is gui representation of Node
    std::vector<Field> fields;
    float layoutY = contentTop;
    // generate Fields
    LayoutNodes(root, fields, marginX, layoutY, 0.0f, contentW, itemHeight, gap);

    Vector2 mouse = GetMousePosition();
    float wheel = GetMouseWheelMove();

    // scroll
    float contentHeight = layoutY - contentTop;
    float maxScroll = contentHeight > viewH ? contentHeight - viewH : 0.0f;
    if (wheel != 0.0f)
    {
        state.scrollY -= wheel * 30.0f;
        if (state.scrollY < 0)
            state.scrollY = 0;
        if (state.scrollY > maxScroll)
            state.scrollY = maxScroll;
    }

    // scissor means we only draw inside defined area
    BeginScissorMode((int)marginX - 2, (int)contentTop - 2, (int)contentW + 6, (int)viewH + 6);

    for (Field &field : fields)
    {
        Rectangle rect = field.rect;
        rect.y -= state.scrollY;

        if (rect.y + rect.height < contentTop || rect.y > contentTop + viewH)
            continue;

        bool hovered = CheckCollisionPointRec(mouse, rect);

        if (field.is_header)
        {
            // background
            Color bgColor = hovered ? Color{180, 200, 230, 255} : Color{150, 170, 200, 255};
            DrawRectangleRec(rect, bgColor);
            // outline
            DrawRectangleLinesEx(rect, 1, BLACK);
            // collapse/expand indicator
            DrawText(field.node->collapsed ? ">" : "|", (int)rect.x + 6, (int)rect.y + 6, 14, BLACK);
            // title
            DrawText(field.node->title.c_str(), (int)rect.x + 24, (int)rect.y + 6, 14, BLACK);

            // collapse or uncollapse on click
            if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                field.node->collapsed = !field.node->collapsed;
        }
        else if (field.config)
        {
            // background
            DrawRectangleRec(rect, (state.selectedNode && state.selectedNode->value && &(*state.selectedNode->value) == field.config)
                                       ? Color{200, 230, 200, 255}
                                       : Color{220, 220, 220, 255});
            DrawRectangleLinesEx(rect, 1, BLACK);

            // text
            std::string displayText;
            auto it = state.pendingEdits.find(field.config);
            if (it != state.pendingEdits.end())
                displayText = it->second;
            else
                displayText = ValueToString(*field.config);
            displayText = field.node->title + " = " + displayText;
            DrawText(displayText.c_str(), (int)rect.x + 12, (int)rect.y + 6, 14, BLACK);

            // click to select / start editing
            if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                state.selectedNode = field.node;
                if (field.config->type != ValueType::Bool)
                    state.pendingEdits[field.config] = ValueToString(*field.config);
                else
                {
                    bool &b = std::get<bool>(field.config->value);
                    b = !b;
                }
            }
        }
    }

    EndScissorMode();

    // Keyboard input
    if (state.selectedNode && state.selectedNode->value)
    {
        ConfigValue *selected = &(*state.selectedNode->value);
        if (selected->type != ValueType::Bool)
        {
            int characterPressed = GetCharPressed();
            while (characterPressed > 0)
            {
                if ((characterPressed >= '0' && characterPressed <= '9') || characterPressed == '-' || characterPressed == '.')
                    state.pendingEdits[selected] += (char)characterPressed;
                characterPressed = GetCharPressed();
            }

            // remove last character on backspace
            if (IsKeyPressed(KEY_BACKSPACE) && !state.pendingEdits[selected].empty())
                state.pendingEdits[selected].pop_back();

            // apply edit on enter
            if (IsKeyPressed(KEY_ENTER))
            {
                ApplyTextEdit(*selected, state.pendingEdits[selected]);
                state.pendingEdits.erase(selected);
                state.selectedNode = nullptr;
            }

            if (IsKeyPressed(KEY_ESCAPE))
                state.selectedNode = nullptr;
        }
    }
}

// Apply all pending edits when UI is closed
void ApplyPendingEdits(ConfigUIState &state)
{
    for (auto &pair : state.pendingEdits)
        ApplyTextEdit(*pair.first, pair.second);

    state.pendingEdits.clear();
    state.selectedNode = nullptr;
}
