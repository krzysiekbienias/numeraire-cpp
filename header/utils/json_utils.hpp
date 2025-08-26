#ifndef CONFIG_LOADER_HPP
#define CONFIG_LOADER_HPP

#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "logger.hpp"


/**
 * @class JsonUtils
 * @brief Static utility class to manage loading and retrieving JSON files using unique identifiers.
 *
 * The JsonUtils class provides a centralized way to load JSON files from disk once and
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

class JsonUtils {
    
private:
    static inline std::unordered_map<std::string,nlohmann::json> loadedJsons; //to handle with multidimensional json without additional helpers

    
public:
    /**
     * @brief Load and parse a JSON file into the internal JsonUtils store.
     *
     * This function opens the given file, parses its contents using nlohmann::json,
     * and stores the resulting JSON object under the provided identifier.
     * The loaded JSON can later be accessed via getJson(jsonIdentifier) or
     * converted into other structures (e.g., a flat map).
     *
     * Usage example:
     * @code
     * JsonUtils::load("main", "/configs/main_config.json");
     * auto cfg = JsonUtils::getFlatConfigMap("main");
     * @endcode
     *
     * @param jsonIdentifier Logical identifier used to retrieve this JSON later.
     *                       (e.g. "main", "secrets", "schedule").
     * @param path Filesystem path to the JSON file to be loaded.
     *
     * @throws std::runtime_error If the file cannot be opened or the contents
     *                            are not valid JSON.
     *
     * @note Successfully loaded JSONs are cached internally in JsonUtils::loadedJsons.
     */
    static void load(const std::string&  jsonIdentifier,const std::string& path);
    
    template<typename T>
    static T getValue(const std::string& jsonIdentifier, const std::string& tag) {
        const auto& j = getJson(jsonIdentifier);
        if (!j.contains(tag)) {
            Logger::get()->warn("Key '{}' not found in JSON '{}'", tag, jsonIdentifier);
            throw std::runtime_error("Missing key: " + tag);
        }
        return j.at(tag).get<T>();
    }
    
    static const nlohmann::json& getJson(const std::string&  jsonIdentifier);
    
    static bool isFlatObject(const std::string& jsonIdentifier);
    /**
     * @brief Convert a flat JSON object into an unordered string-to-string map.
     *
     * This utility reads the JSON object identified by @p jsonIdentifier
     * and converts all key-value pairs into an `std::unordered_map<std::string, std::string>`.
     *
     * @param jsonIdentifier  The identifier/name of the JSON object previously loaded
     *                        into JsonUtils (must be a flat object).
     *
     * @return An unordered map with keys and values from the JSON object.
     *
     * @throws std::runtime_error If the JSON referenced by @p jsonIdentifier is not flat.
     *
     * @note
     * - Each JSON value is converted to `std::string` using `value.get<std::string>()`.
     * - Nested JSON structures are not supported.
     * - A warning is logged via Logger if the object is not flat.
     */
    
    
    /**
     * @brief Convert a flat JSON object into an unordered_map<string,string>.
     *
     * This is intended only for simple, flat configuration JSONs where all values
     * are strings (e.g. {"DB_PATH":"/db.sqlite3","HOLIDAYS_FILE":"holidays.csv"}).
     *
     * ⚠️ If the JSON contains nested objects or non-string values, this method
     * will throw an exception. For more complex JSON structures, use
     * JsonUtils::parseFromString() and access fields directly with nlohmann::json.
     *
     * @param jsonIdentifier Identifier of the JSON already loaded into JsonUtils.
     * @return Unordered map of string key/value pairs.
     */
    static std::unordered_map<std::string, std::string> toStringMap(const std::string& jsonIdentifier);
    
    /**
     * @brief Parse a raw JSON string into a nlohmann::json object.
     *
     * This method allows working with nested and mixed-type JSON structures.
     * Use this when the JSON cannot be flattened into simple key/value pairs.
     *
     * Example:
     * @code
     * auto j = JsonUtils::parseFromString(R"({"paths":{"db":"/db.sqlite3"}})");
     * std::string dbPath = j["paths"]["db"].get<std::string>();
     * @endcode
     *
     * @param rawJson String containing JSON data.
     * @return Parsed nlohmann::json object.
     * @throws std::runtime_error if the string is not valid JSON.
     */
    static nlohmann::json parseFromString(const std::string & rawJson);
    
    static void saveToFile(const nlohmann::json& j, const std::string& path);
    


};



#endif // CONFIG_LOADER_HPP
