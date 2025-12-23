#include "ConfigUI.hpp"
#include "ConfigTree.hpp"
#include "MathUtil.hpp"
#include <raylib.h>
#include <sstream>
#include <iomanip>

// Helper to convert ConfigValue to string
static std::string ValueToString(const ConfigValue &cv)
{
    switch (cv.type)
    {
    case ValueType::Int:
        return std::to_string(std::get<int>(cv.value));
    case ValueType::Float:
    {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2) << std::get<float>(cv.value);
        return ss.str();
    }
    case ValueType::Bool:
        return std::get<bool>(cv.value) ? "true" : "false";
    }
    return "ERR";
}

// Draws the config UI and updates pending edits map
void DrawConfigUI(Node &root, ConfigUIState &state, float contentTop, float viewH, float contentW)
{
    const float itemH = 30.0f;
    const float gap = 6.0f;
    const float marginX = 16.0f;

    std::vector<Field> fields;
    float layoutY = contentTop;
    LayoutNodes(root, fields, marginX, layoutY, 0.0f, contentW, itemH, gap);

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

    BeginScissorMode((int)marginX - 2, (int)contentTop - 2, (int)contentW + 6, (int)viewH + 6);

    for (Field &f : fields)
    {
        Rectangle r = f.rect;
        r.y -= state.scrollY;

        if (r.y + r.height < contentTop || r.y > contentTop + viewH)
            continue;

        bool hovered = CheckCollisionPointRec(mouse, r);

        if (f.is_header)
        {
            DrawRectangleRec(r, hovered ? Color{180, 200, 230, 255} : Color{150, 170, 200, 255});
            DrawRectangleLinesEx(r, 1, BLACK);
            DrawText(f.node->collapsed ? ">" : "|", (int)r.x + 6, (int)r.y + 6, 14, BLACK);
            DrawText(f.node->title.c_str(), (int)r.x + 24, (int)r.y + 6, 14, BLACK);

            if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                f.node->collapsed = !f.node->collapsed;
        }
        else if (f.config)
        {
            DrawRectangleRec(r, (state.selectedNode && state.selectedNode->value && &(*state.selectedNode->value) == f.config)
                                    ? Color{200, 230, 200, 255}
                                    : Color{220, 220, 220, 255});
            DrawRectangleLinesEx(r, 1, BLACK);

            std::string displayText;
            auto it = state.pendingEdits.find(f.config);
            if (it != state.pendingEdits.end())
                displayText = it->second;
            else
                displayText = ValueToString(*f.config);

            displayText = f.node->title + " = " + displayText;
            DrawText(displayText.c_str(), (int)r.x + 6, (int)r.y + 6, 14, BLACK);

            if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                state.selectedNode = f.node;
                if (f.config->type != ValueType::Bool)
                    state.pendingEdits[f.config] = ValueToString(*f.config);
                else
                {
                    bool &b = std::get<bool>(f.config->value);
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
            int c = GetCharPressed();
            while (c > 0)
            {
                if ((c >= '0' && c <= '9') || c == '-' || c == '.')
                    state.pendingEdits[selected] += (char)c;
                c = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) && !state.pendingEdits[selected].empty())
                state.pendingEdits[selected].pop_back();

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
