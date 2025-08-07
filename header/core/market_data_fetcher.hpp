#pragma once
#include "market_environment.hpp"
#include <string>
#include "../configs/constants.hpp"
#include <nlohmann/json.hpp>
#include<ql/time/date.hpp>
#include <set>

struct OptionContract {
    std::string ticker;
    std::string contractType;
    std::string exerciseStyle;
    std::string expirationDate;
    double strikePrice;
};


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
    
    
    // 📡 Fetch option price.
    std::optional<nlohmann::json> queryPolygonOptionPrice(const std::string& optionSymbol) const;
    
    std::string buildPolygonOptionSymbol(const std::string& ticker,
                                                            const std::string& expiry,  // format: DD-MM-YYYY
                                                            const std::string& optionType,  // "call" or "put"
                                         double strike);
    
    void fetchOptionMarketDataForTicker(
        const std::string& ticker,
        const std::string& expiry,
        double spotPrice,
        const std::string& outputDir);

    


private:
    std::string m_valuationDate;
    std::string m_apiKey;

   
};
