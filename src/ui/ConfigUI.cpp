#include "ConfigUI.hpp"
#include "MathUtil.hpp"
#include <sstream>
#include <iomanip>

static std::string ValueToString(const ConfigValue &cv)
{
    std::ostringstream ss;
    switch (cv.type)
    {
    case ValueType::Int:
    {
        if (auto p = std::get_if<int>(&cv.value))
            ss << *p;
        else
            ss << "ERR";
        break;
    }
    case ValueType::Float:
    {
        if (auto p = std::get_if<float>(&cv.value))
            ss << std::fixed << std::setprecision(2) << *p;
        else
            ss << "ERR";
        break;
    }
    case ValueType::Bool:
    {
        if (auto p = std::get_if<bool>(&cv.value))
            ss << (*p ? "true" : "false");
        else
            ss << "ERR";
        break;
    }
    default:
        ss << "ERR";
        break;
    }
    return ss.str();
}

void DrawConfigUI(
    Node &root,
    ConfigUIState &state,
    float contentTop,
    float viewH,
    float contentW)
{
    const float itemH = 30.0f;
    const float gap = 6.0f;
    const float marginX = 16.0f;
    const float marginY = 16.0f;

    std::vector<Field> fields;
    float layoutY = contentTop;
    LayoutNodes(root, fields, marginX, layoutY, 0.0f, contentW, itemH, gap);

    float contentHeight = layoutY - contentTop;
    float maxScroll = contentHeight > viewH ? contentHeight - viewH : 0.0f;

    float wheel = GetMouseWheelMove();
    if (!state.editing && wheel != 0.0f)
    {
        state.scrollY -= wheel * 30.0f;
        if (state.scrollY < 0)
            state.scrollY = 0;
        if (state.scrollY > maxScroll)
            state.scrollY = maxScroll;
    }

    Vector2 mouse = GetMousePosition();

    BeginScissorMode(
        (int)marginX - 2,
        (int)contentTop - 2,
        (int)contentW + 6,
        (int)viewH + 6);

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

            DrawText(f.node->collapsed ? " >" : " |",
                     (int)r.x + 6, (int)r.y + 6, 14, BLACK);

            DrawText(f.node->title.c_str(),
                     (int)r.x + 24, (int)r.y + 6, 14, BLACK);

            if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                f.node->collapsed = !f.node->collapsed;
                state.selected = nullptr;
                state.editing = false;
            }
        }
        else
        {
            // f.config is invalid addr here
            if (!f.config)
                continue;

            bool selected = (state.selected == f.config);
            DrawRectangleRec(r, selected ? Color{200, 230, 200, 255} : Color{220, 220, 220, 255});
            DrawRectangleLinesEx(r, 1, BLACK);

            std::string text =
                f.config->label + " = " +
                (state.editing && selected ? state.editBuffer : ValueToString(*f.config));

            DrawText(text.c_str(), (int)r.x + 6, (int)r.y + 6, 14, BLACK);

            if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                state.selected = f.config;
                state.editing = (f.config->type != ValueType::Bool);
                state.editBuffer = ValueToString(*f.config);

                if (f.config->type == ValueType::Bool)
                {
                    if (auto b = std::get_if<bool>(&f.config->value))
                        *b = !*b;
                }
            }
        }
    }

    EndScissorMode();

    // Keyboard input
    if (state.editing && state.selected)
    {
        int c = GetCharPressed();
        while (c > 0)
        {
            if ((c >= '0' && c <= '9') || c == '-' || c == '.')
                state.editBuffer.push_back((char)c);
            c = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) && !state.editBuffer.empty())
            state.editBuffer.pop_back();

        if (IsKeyPressed(KEY_ENTER))
        {
            ApplyTextEdit(*state.selected, state.editBuffer);
            state.editing = false;
        }

        if (IsKeyPressed(KEY_ESCAPE))
            state.editing = false;
    }

    // Scrollbar
    if (maxScroll > 0.0f)
    {
        float barH = (viewH / contentHeight) * viewH;
        float barY = contentTop + (state.scrollY / maxScroll) * (viewH - barH);
        DrawRectangle((int)(marginX + contentW + 6), (int)contentTop, 8, (int)viewH, Color{230, 230, 230, 255});
        DrawRectangle((int)(marginX + contentW + 6), (int)barY, 8, (int)barH, Color{160, 160, 160, 255});
    }
}
