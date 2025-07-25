#pragma once
#include "market_environment.hpp"
#include <string>
#include "../configs/constants.hpp"
#include <nlohmann/json.hpp>

class MarketDataFetcher {
public:
    // 🔧 Inject API key and valuation date for external data requests.
    MarketDataFetcher(const std::string& valuationDate, const std::string& apiKey);

    // 📡 Fetch spot price.
    bool fetchSpotPrice(MarketEnvironment& env, const std::string& ticker);
    std::optional<double> queryPolygonPrice(const std::string& ticker, const std::string& valuationDate) const;
    // 📡 Fetch historical price.
    
    std::optional<std::vector<double>> queryPolygonHistoricalTimeSeries(
        const std::string& ticker,
        size_t required) const;

    std::optional<std::vector<double>> queryPolygonHistoricalTimeSeries(
        const std::string& ticker,
        const std::string& startDate,
        const std::string& endDate) const;
    
    // 📡 Fetch option json.
    std::optional<nlohmann::json> queryPolygonOptionsSnapshot(const std::string& underlyingTicker) const;

private:
    std::string m_valuationDate;
    std::string m_apiKey;

   
};
