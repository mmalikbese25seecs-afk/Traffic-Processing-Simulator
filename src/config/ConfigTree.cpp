#include "ConfigTree.hpp"

void LayoutNodes(
    Node& node,
    std::vector<Field>& outFields,
    float x,
    float& y,
    float indent,
    float width,
    float itemHeight,
    float gap)
{
    if (!node.value.has_value())
    {
        Rectangle r = { x + indent, y, width - indent, itemHeight };
        outFields.push_back({ r, &node, true, nullptr, indent });
        y += itemHeight + gap;

        if (!node.collapsed)
        {
            for (Node& c : node.children)
            {
                LayoutNodes(c, outFields, x, y, indent + 18.0f, width, itemHeight, gap);
            }
        }
    }
    else
    {
        Rectangle r = { x + indent, y, width - indent, itemHeight };
        outFields.push_back({ r, &node, false, &(*node.value), indent });
        y += itemHeight + gap;
    }
}

void CollectConfigPointers(Node& node, std::vector<ConfigValue*>& out)
{
    if (node.value.has_value())
        out.push_back(&(*node.value));

    for (Node& c : node.children)
        CollectConfigPointers(c, out);
}
