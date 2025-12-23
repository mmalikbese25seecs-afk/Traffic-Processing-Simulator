#pragma once
#include <string>
#include <variant>

enum class ValueType
{
    Int,
    Float,
    Bool
};

// allow storing different config value types
struct ConfigValue
{
    std::string label;
    ValueType type;
    // Stores exactly one of the allowed types at a time (tagged union)
    std::variant<int, float, bool> value;
    float min_value = -1e9f;
    float max_value = 1e9f;
};
