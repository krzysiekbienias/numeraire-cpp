#include <nlohmann/json.hpp>
#include <fstream>
#include "utils/logger.hpp"

namespace market_store::options {

    bool saveOptionValueToFile(const std::string& filePath, const nlohmann::json& payload) {
        try {
            std::ofstream out(filePath);
            out << payload.dump(2);
            return static_cast<bool>(out);
        } catch (const std::exception& e) {
            Logger::get()->error("❌ [opt-values] Save failed for {}: {}", filePath, e.what());
            return false;
        }
    }


    std::optional<nlohmann::json>
    loadOptionValueFromFile(const std::string& filePath) {
        try {
            std::ifstream in(filePath);
            if (!in) {
                Logger::get()->warn("⚠ [opt-values] Cannot open file {}", filePath);
                return std::nullopt;
            }
            // szybka osłona na puste pliki
            in.seekg(0, std::ios::end);
            const auto size = in.tellg();
            in.seekg(0, std::ios::beg);
            if (size == 0) {
                Logger::get()->warn("⚠ [opt-values] Empty file {}", filePath);
                return std::nullopt;
            }

            nlohmann::json j;
            in >> j;
            return j;
        } catch (const std::exception& e) {
            Logger::get()->error("❌ [opt-values] Load/parse failed for {}: {}", filePath, e.what());
            return std::nullopt;
        }
    }

}
