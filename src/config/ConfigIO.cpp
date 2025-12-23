#include "ConfigIO.hpp"
#include "MathUtil.hpp"
#include <fstream>
#include <sstream>
#include <cctype>

bool SaveConfigToFile(Node& root, const std::string& path)
{
    std::ofstream stream(path);
    if (!stream.is_open())
        return false;

    std::vector<ConfigValue*> allConfigValues;
    CollectConfigPointers(root, allConfigValues);

    for (ConfigValue* config : allConfigValues)
    {
        // label:type:value
        stream << config->label << ":";
        stream << (config->type == ValueType::Int ? "int:" :
                config->type == ValueType::Float ? "float:" : "bool:");

        // add value based on type
        if (config->type == ValueType::Int)
            stream << std::get<int>(config->value);
        else if (config->type == ValueType::Float)
            stream << std::get<float>(config->value);
        else
            stream << (std::get<bool>(config->value) ? "true" : "false");

        stream << "\n";
    }
    return true;
}

bool LoadConfigFromFile(Node& root, const std::string& path)
{
    std::ifstream stream(path);
    if (!stream.is_open())
        return false;

    std::vector<ConfigValue*> allConfigValues;
    CollectConfigPointers(root, allConfigValues);

    auto findByLabel = [&](const std::string& label) -> ConfigValue*
    {
        for (ConfigValue* config : allConfigValues)
            if (config->label == label)
                return config;
        return nullptr;
    };

    std::string line;
    while (std::getline(stream, line))
    {
        // seperate string from label:type:value
        size_t a = line.find(':');
        size_t b = line.find(':', a + 1);
        if (a == std::string::npos || b == std::string::npos)
            continue;

        // split into label, type, value
        std::string label = line.substr(0, a);
        std::string type = line.substr(a + 1, b - a - 1);
        std::string val = line.substr(b + 1);

        ConfigValue* config = findByLabel(label);
        if (!config)
            continue;

        try
        {
            if (type == "int" && config->type == ValueType::Int)
            {
                int v = std::stoi(val);
                config->value = (int)ClampFloat(v, config->min_value, config->max_value);
            }
            else if (type == "float" && config->type == ValueType::Float)
            {
                float v = std::stof(val);
                config->value = ClampFloat(v, config->min_value, config->max_value);
            }
            else if (type == "bool" && config->type == ValueType::Bool)
            {
                config->value = (val == "true");
            }
        }
        // Ignore conversion errors
        catch (...) {}
    }
    return true;
}
