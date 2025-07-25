#include "json_utils.hpp"
#include "logger.hpp"
#include <fstream>
#include <stdexcept>

void JsonUtils::load(const std::string&  jsonIdentifier,const std::string& path) {
    std::ifstream file(path); //opens the file stream
    if (!file) {
        Logger::get()->error("JSON file not found: {}", path);
        throw std::runtime_error("JSON file not found: " + path);
    }

    try {
        nlohmann::json jsonData;
        file >> jsonData; //parses the fule contents and fills the container. Now configData holds structured, strongly typed access to all JSON data
        loadedJsons[jsonIdentifier]=jsonData;
        Logger::get()->info("Loaded JSON '{}' from {}",jsonIdentifier, path);
    } catch (const nlohmann::json::parse_error& e) {
        Logger::get()->error("JSON parsing error in {}: {}",path, e.what());
        throw std::runtime_error("Invalid JSON format in " + path);
    }
}


const nlohmann::json& JsonUtils::getJson(const std::string& jsonIdentifier) {
    if (!loadedJsons.contains(jsonIdentifier)) {
            Logger::get()->error("JSON with identifier '{}' not loaded.", jsonIdentifier);
            throw std::runtime_error("JSON not loaded: " + jsonIdentifier);
        }
        return loadedJsons.at(jsonIdentifier);
}



bool JsonUtils::isFlatObject(const std::string& jsonIdentifier) {
    const auto& json = getJson(jsonIdentifier);
    if (!json.is_object()) return false;

    for (const auto& [key, value] : json.items()) {
        if (!value.is_string()) {
            return false;  // contains nested objects, arrays, or non-string values
        }
    }
    return true;
}

std::unordered_map<std::string, std::string> JsonUtils::toStringMap(const std::string& jsonIdentifier) {
    if (!isFlatObject(jsonIdentifier)) {
        Logger::get()->warn("❗ Cannot convert '{}' to map — JSON is not flat.", jsonIdentifier);
        throw std::runtime_error("Attempted to convert nested JSON to flat map.");
    }

    const auto& json = getJson(jsonIdentifier);
    std::unordered_map<std::string, std::string> map;
    for (const auto& [key, value] : json.items()) {
        map[key] = value.get<std::string>();
    }
    return map;
}

nlohmann::json JsonUtils::parseFromString(const std::string& rawJson) {
    try {
        return nlohmann::json::parse(rawJson);
    } catch (const nlohmann::json::parse_error& e) {
        throw std::runtime_error("Failed to parse JSON string: " + std::string(e.what()));
    }
}

void JsonUtils::saveToFile(const nlohmann::json& j, const std::string& path) {
    if (std::filesystem::exists(path)) {
        Logger::get()->warn("⚠️ Overwriting existing JSON file: {}", path);
    }

    std::ofstream out(path);
    if (!out.is_open()) {
        throw std::runtime_error("❌ Could not write to JSON file: " + path);
    }

    out << j.dump(4);
    Logger::get()->info("💾 JSON successfully saved to: {}", path);
}
