# pragma once

#include <nlohmann/json.hpp>
#include <fstream>
#include "live_equity_otion_contracts.hpp"

namespace market_store::options {
    bool saveOptionValueToFile(const std::string& filePath, const nlohmann::json& payload);

    std::optional<nlohmann::json>
    loadOptionValueFromFile(const std::string& filePath);
}