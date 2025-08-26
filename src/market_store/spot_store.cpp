#include "market_store/spot_store.hpp"
#include <nlohmann/json.hpp>
#include <fstream>

using nlohmann::json;

namespace market_store {

bool saveSpotsToFile(const std::string& filePath,
                     const std::unordered_map<std::string, double>& spots)
{
    json out = json::object();
    for (const auto& [ticker, px] : spots) {
        out[ticker] = json{{"spot", px}};
    }

    std::filesystem::create_directories(std::filesystem::path(filePath).parent_path());

    std::ofstream ofs(filePath);
    if (!ofs) return false;
    ofs << out.dump(4); // pretty print
    return static_cast<bool>(ofs);
}

std::optional<std::unordered_map<std::string, double>>
loadAllSpotsFromFile(const std::string& filePath) {
    std::ifstream ifs(filePath);
    if (!ifs) return std::nullopt;

    json j;
    try { ifs >> j; } catch (...) { return std::nullopt; }

    // oczekiwany kształt:
    // { "AAPL": { "spot": 202.38 }, "AMZN": { "spot": 214.75 }, ... }
    if (!j.is_object()) return std::nullopt;

    std::unordered_map<std::string, double> out;
    out.reserve(j.size());
    for (auto it = j.begin(); it != j.end(); ++it) {
        const auto& obj = it.value();
        if (obj.is_object() && obj.contains("spot") && obj["spot"].is_number()) {
            out[it.key()] = obj["spot"].get<double>();
        }
    }
    if (out.empty()) return std::nullopt;
    return out;
}

std::optional<double>
loadSpotFromFile(const std::string& filePath, const std::string& ticker) {
    auto all = loadAllSpotsFromFile(filePath);
    if (!all) return std::nullopt;
    auto it = all->find(ticker);
    if (it == all->end()) return std::nullopt;
    return it->second;
}

std::unordered_map<std::string, double>
loadSpotsSubsetFromFile(const std::string& filePath,
                        const std::vector<std::string>& tickers) {
    std::unordered_map<std::string, double> result;
    auto all = loadAllSpotsFromFile(filePath);
    if (!all) return result;

    result.reserve(tickers.size());
    for (const auto& t : tickers) {
        auto it = all->find(t);
        if (it != all->end()) result.emplace(t, it->second);
    }
    return result;
}

} // namespace market_store

