#include "utils/path_utils.hpp"
#include <filesystem>
#include <iostream>
#include "utils/logger.hpp"


std::string resolveProjectPath(const std::string& executable_path) {
    namespace fs = std::filesystem;
    fs::path exec_path = fs::absolute(executable_path);
    fs::path parent = exec_path.parent_path();

    // Jeśli binarka w `bin/`, idź tylko jeden poziom wyżej
    if (parent.filename() == "bin") {
        return parent.parent_path().string();
    }

    // Jeśli np. w `xcode_build/Debug/`, idź trzy poziomy w górę
    return parent.parent_path().parent_path().parent_path().string();
}

std::string resolveDbPath(const std::string& executable_path) {
    auto project_root = resolveProjectPath(executable_path);
    auto db_path = std::filesystem::path(project_root) / "db.sqlite3";

    Logger::get()->debug("Executable path: {}", executable_path);
    Logger::get()->debug("Resolved DB path: {}", db_path.string());

    return db_path.string();
}

std::string resolveMainConfigPath(const std::string& executable_path) {
    auto project_root = resolveProjectPath(executable_path);
    auto config_path = std::filesystem::path(project_root) / "configs" / "main_config.json";

    Logger::get()->debug("Executable path: {}", executable_path);
    Logger::get()->debug("Resolved Main Config path: {}", config_path.string());

    return config_path.string();
}
