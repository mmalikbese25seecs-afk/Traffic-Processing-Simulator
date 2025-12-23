#pragma once
#include <string>
#include <variant>

enum class ValueType
{
    Int,
    Float,
    Bool
};

struct ConfigValue
{
    std::string label;
    ValueType type;
    std::variant<int, float, bool> value;
    float min_value = -1e9f;
    float max_value = 1e9f;
};
