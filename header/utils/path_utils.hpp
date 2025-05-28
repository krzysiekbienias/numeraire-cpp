#pragma once
#include <string>

std::string resolveProjectPath(const std::string& executable_path);
std::string resolveDbPath(const std::string& executable_path);
std::string resolveMainConfigPath(const std::string& executable_path);

