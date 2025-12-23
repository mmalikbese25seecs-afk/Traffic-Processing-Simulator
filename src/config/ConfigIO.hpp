#pragma once
#include "ConfigTree.hpp"
#include <string>

bool SaveConfigToFile(Node& root, const std::string& path);
bool LoadConfigFromFile(Node& root, const std::string& path);
