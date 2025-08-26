#pragma once
#include <string>
#include <optional>
#include <unordered_map>
#include <vector>

namespace market_store {
bool saveSpotsToFile(const std::string& filePath,
                         const std::unordered_map<std::string, double>& spots);

// Wczytuje cały snapshot spotów z konkretnego pliku JSON
std::optional<std::unordered_map<std::string, double>>
loadAllSpotsFromFile(const std::string& filePath);

/**
 * @brief Load entire spot map from a JSON file.
 * @param filePath Path to snapshot JSON.
 * @return Map ticker -> spot, or empty on parse error.
 */
std::optional<double>
loadSpotFromFile(const std::string& filePath, const std::string& ticker);

// Zwraca tylko wybrany podzbiór tickerów (jeśli istnieją w pliku)
std::unordered_map<std::string, double>
loadSpotsSubsetFromFile(const std::string& filePath,
                        const std::vector<std::string>& tickers);

// (Opcjonalnie) aliasy zgodne ze stylem “base dir + date”,
// żeby nie przepisywać kodu gdzie indziej:
static inline std::optional<std::unordered_map<std::string, double>>
loadAllSpots(const std::string& baseCacheDir, const std::string& asOfDDMMYYYY) {
    return loadAllSpotsFromFile(baseCacheDir + "/spot_price/spot_snapshot_" + asOfDDMMYYYY + ".json");
}

static inline std::optional<double>
loadSpot(const std::string& baseCacheDir, const std::string& asOfDDMMYYYY, const std::string& ticker) {
    return loadSpotFromFile(baseCacheDir + "/spot_price/spot_snapshot_" + asOfDDMMYYYY + ".json", ticker);
}

static inline std::unordered_map<std::string, double>
loadSpotsFor(const std::string& baseCacheDir, const std::string& asOfDDMMYYYY, const std::vector<std::string>& tickers) {
    return loadSpotsSubsetFromFile(baseCacheDir + "/spot_price/spot_snapshot_" + asOfDDMMYYYY + ".json", tickers);
}

} // namespace market_store
