// include/core/market_data_fetcher.hpp
#pragma once
#include <string>
#include <optional>
#include <vector>
#include <nlohmann/json.hpp>

class MarketDataFetcher {
public:
    explicit MarketDataFetcher(std::string apiKey);

    // ---- Spot (pure HTTP, no file I/O, no env mutation)
    std::optional<double>
    getSpotCloseForDate(const std::string& ticker,
                        const std::string& valuationDateISO) const; // YYYY-MM-DD

    std::optional<std::vector<std::string>>
    getOptionSymbolsForDate(const std::string& underlyingTicker,
                            const std::string& asOfISO) const;
    std::optional<nlohmann::json>
    getOptionOpenCloseBySymbol(const std::string& optionSymbol,
                               const std::string& asOfISO) const;

    // ---- Historical (pure HTTP)
    std::optional<std::vector<double>>
    getHistoricalCloses(const std::string& ticker,
                        size_t required,
                        const std::string& ddmmyyyy) const;

    std::optional<std::vector<double>>
    getHistoricalCloses(const std::string& ticker,
                        const std::string& startDDMMYYYY,
                        const std::string& endDDMMYYYY) const;

    // ---- Options (pure HTTP)
    std::optional<std::vector<std::string>>
    getOptionContractsForDate(const std::string& underlyingTicker,
                              const std::string& ddmmyyyy) const;

    std::optional<nlohmann::json>
    getOptionOpenCloseForDate(const std::string& optionSymbol,
                              const std::string& ddmmyyyy) const;

    // Utility (kept if useful for your flow)
    static std::string buildPolygonOptionSymbol(const std::string& ticker,
                                                const std::string& expiryDDMMYYYY,
                                                const std::string& optionType, // "call"/"put"
                                                double strike);

private:
    std::string m_apiKey;
};
