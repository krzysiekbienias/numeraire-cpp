#pragma once
#include <string>
#include <optional>
#include <vector>

namespace market_store::options {
    bool saveSymbolsToFile(const std::string& filePath,
                                  const std::vector<std::string>& symbols);

/**
 * @brief Load list of OCC symbols from given JSON file.
 * @param filePath Path to JSON file (live_options/<date>/<ticker>_<date>.json).
 * @return Vector of option symbols, or nullopt if file is missing/invalid.
 */
    [[nodiscard]] std::optional<std::vector<std::string>>
    loadSymbolsFromFile(const std::string& filePath);

/**
 * @brief Build full path to a ticker’s live options file.
 */
    inline std::string
    makeFilePath(const std::string& baseCacheDir,
                 const std::string& asOfDDMMYYYY,
                 const std::string& ticker)
    {
        return baseCacheDir + "/live_options/" + asOfDDMMYYYY + "/" +
               ticker + "_" + asOfDDMMYYYY + ".json";
    }


/**
 * @brief Convenience wrapper: build path and load symbols.
 */
[[nodiscard]] inline std::optional<std::vector<std::string>>
loadSymbolsFromFile(const std::string& baseCacheDir,
                    const std::string& asOfDDMMYYYY,
                    const std::string& ticker)
{
    return loadSymbolsFromFile(
        makeFilePath(baseCacheDir, asOfDDMMYYYY, ticker)
    );
}
    bool saveSymbolsToFile(const std::string& filePath,
                           const std::vector<std::string>& symbols);

} // namespace market_store::options
