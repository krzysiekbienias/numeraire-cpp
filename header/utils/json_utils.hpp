#ifndef CONFIG_LOADER_HPP
#define CONFIG_LOADER_HPP

#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "logger.hpp"


/**
 * @class JsonLoader
 * @brief Static utility class to manage loading and retrieving JSON files using unique identifiers.
 *
 * The JsonLoader class provides a centralized way to load JSON files from disk once and
 * retrieve values by tag, using an identifier-based lookup system. It uses a static map to
 * cache loaded JSON objects and avoid redundant I/O.
 *
 * Example usage:
 * @code
 * JsonLoader::load("config", "config/settings.json");
 * int threads = loader.getValue<int>("config", "thread_count");
 * @endcode
 *
 * - Each JSON file is identified by a unique string (e.g., "config", "user_data").
 * - If a key is missing, getValue() logs a warning and throws std::runtime_error.
 *
 * @note All methods are static or behave as such; the class does not require instantiation.
 */

class JsonLoader {
    
private:
    static inline std::unordered_map<std::string,nlohmann::json> loadedJsons; //to handle with multidimensional json without additional helpers

    
public:
    static void load(const std::string&  jsonIdentifier,const std::string& path);
    
    template<typename T>
    T getValue(const std::string& jsonIdentifier, const std::string& tag) {
        const auto& j = getJson(jsonIdentifier);
        if (!j.contains(tag)) {
            Logger::get()->warn("Key '{}' not found in JSON '{}'", tag, jsonIdentifier);
            throw std::runtime_error("Missing key: " + tag);
        }
        return j.at(tag).get<T>();
    }
    
    static const nlohmann::json& getJson(const std::string&  jsonIdentifier);
    
    static bool isFlatObject(const std::string& jsonIdentifier);
    static std::unordered_map<std::string, std::string> toStringMap(const std::string& jsonIdentifier);

};

#endif // CONFIG_LOADER_HPP
