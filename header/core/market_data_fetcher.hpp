#pragma once
#include "market_environment.hpp"
#include <string>
#include "../configs/constants.hpp"

class MarketDataFetcher {
public:
    // 🔧 Inject API key and valuation date for external data requests
    MarketDataFetcher(const std::string& valuationDate, const std::string& apiKey);

    // 📡 Fetch spot price and insert into MarketEnvironment
    bool fetchSpotPrice(MarketEnvironment& env, const std::string& ticker);

    // 📈 Fetch historical prices and estimate drift/volatility
    bool fetchHistoricalDriftAndVolatility(MarketEnvironment& env, const std::string& ticker, size_t requiredObs = config::DEFAULT_HISTORICAL_WINDOW_DAYS);
    
    
    // Internal helpers (previously in MarketEnvironment)
    std::optional<double> queryPolygonPrice(const std::string& ticker) const;
    std::optional<std::vector<double>> queryPolygonHistoricalTimeSeries(
        const std::string& ticker,
        size_t required) const;

    std::optional<std::vector<double>> queryPolygonHistoricalTimeSeries(
        const std::string& ticker,
        const std::string& startDate,
        const std::string& endDate) const;

private:
    std::string m_valuationDate;
    std::string m_apiKey;

   
};
