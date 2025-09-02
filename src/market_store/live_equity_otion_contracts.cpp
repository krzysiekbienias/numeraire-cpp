#include "market_store/live_equity_otion_contracts.hpp"
#include <nlohmann/json.hpp>
#include <fstream>

namespace market_store::options {


    bool saveSymbolsToFile(const std::string& filePath,
                                  const std::vector<std::string>& symbols) {
        try {
            nlohmann::json j=symbols;
            std::ofstream ofs(filePath);
            ofs << j.dump(2) << '\n';
            return true;
        } catch (...) {
            return false;
        }
    }


    std::optional<std::vector<std::string>>
    loadSymbolsFromFile(const std::string& filePath) {
        try {
            std::ifstream f(filePath);
            if (!f) return std::nullopt;

            nlohmann::json j;
            f >> j;
            if (!j.is_array()) return std::nullopt;

            std::vector<std::string> out;
            out.reserve(j.size());
            for (const auto& el : j) {
                if (el.is_string()) out.push_back(el.get<std::string>());
            }
            return out;
        } catch (...) {
            return std::nullopt;
        }
    }


} // namespace market_store::options
