#include "path_utils.hpp"
#include <filesystem>
#include <iostream>
#include "utils/logger.hpp"


std::string resolveProjectPath(const std::string& executable_path) {
    namespace fs = std::filesystem;
    fs::path exec_path = fs::absolute(executable_path);
    fs::path project_root = exec_path.parent_path().parent_path().parent_path();
    return project_root.string();
}

std::string resolveDbPath(const std::string& executable_path) {
    namespace fs = std::filesystem;
    fs::path exec_path = fs::absolute(executable_path);
    fs::path project_root = exec_path.parent_path().parent_path().parent_path();
    fs::path db_path = project_root / "db.sqlite3";

    Logger::get()->debug("Executable path: {}", exec_path.string());
    Logger::get()->debug("Resolved DB path: {}", db_path.string());

    return db_path.string();
}

std::string resolveMainConfigPath(const std::string& executable_path) {
    namespace fs = std::filesystem;
    fs::path exec_path = fs::absolute(executable_path);
    fs::path project_root = exec_path.parent_path().parent_path().parent_path();
    fs::path config_path = project_root / "configs" / "main_config.json";

    Logger::get()->debug("Executable path: {}", exec_path.string());
    Logger::get()->debug("Resolved Main Config path: {}", config_path.string());

    return config_path.string();
}
