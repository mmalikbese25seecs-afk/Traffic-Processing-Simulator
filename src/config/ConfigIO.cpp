#include "ConfigIO.hpp"
#include "MathUtil.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

bool SaveConfigToFile(Node& root, const std::string& path)
{
    std::ofstream ofs(path);
    if (!ofs.is_open())
        return false;

    std::vector<ConfigValue*> all;
    CollectConfigPointers(root, all);

    for (ConfigValue* cv : all)
    {
        ofs << cv->label << ":";
        ofs << (cv->type == ValueType::Int ? "int:" :
                cv->type == ValueType::Float ? "float:" : "bool:");

        if (cv->type == ValueType::Int)
            ofs << std::get<int>(cv->value);
        else if (cv->type == ValueType::Float)
            ofs << std::get<float>(cv->value);
        else
            ofs << (std::get<bool>(cv->value) ? "1" : "0");

        ofs << "\n";
    }
    return true;
}

bool LoadConfigFromFile(Node& root, const std::string& path)
{
    std::ifstream ifs(path);
    if (!ifs.is_open())
        return false;

    std::vector<ConfigValue*> all;
    CollectConfigPointers(root, all);

    auto findByLabel = [&](const std::string& label) -> ConfigValue*
    {
        for (ConfigValue* c : all)
            if (c->label == label)
                return c;
        return nullptr;
    };

    std::string line;
    while (std::getline(ifs, line))
    {
        size_t a = line.find(':');
        size_t b = line.find(':', a + 1);
        if (a == std::string::npos || b == std::string::npos)
            continue;

        std::string label = line.substr(0, a);
        std::string type = line.substr(a + 1, b - a - 1);
        std::string val = line.substr(b + 1);

        ConfigValue* cv = findByLabel(label);
        if (!cv)
            continue;

        try
        {
            if (type == "int" && cv->type == ValueType::Int)
            {
                int v = std::stoi(val);
                cv->value = (int)ClampFloat(v, cv->min_value, cv->max_value);
            }
            else if (type == "float" && cv->type == ValueType::Float)
            {
                float v = std::stof(val);
                cv->value = ClampFloat(v, cv->min_value, cv->max_value);
            }
            else if (type == "bool" && cv->type == ValueType::Bool)
            {
                cv->value = (val == "1" || val == "true");
            }
        }
        catch (...) {}
    }
    return true;
}
