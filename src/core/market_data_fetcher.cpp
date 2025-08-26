#include "core/market_data_fetcher.hpp"
#include "utils/date_utils.hpp"
#include <cmath>
#include <iostream>
#include <numeric>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include "utils/json_utils.hpp"


MarketDataFetcher::MarketDataFetcher(std::string apiKey) {
    if (apiKey.empty()) {
        throw std::invalid_argument("API key cannot be empty");
    }
    m_apiKey = std::move(apiKey);
}



std::optional<double>
MarketDataFetcher::getSpotCloseForDate(const std::string& ticker,
                                       const std::string& valuationDateISO) const
{
    // we assume format is already validated
    const std::string url =
        "https://api.polygon.io/v1/open-close/" + ticker + "/" + valuationDateISO +
        "?adjusted=true&apiKey=" + m_apiKey;

    Logger::get()->info("🔍 [spot] GET {} (valuationDate={})", url, valuationDateISO);

    auto resp = cpr::Get(cpr::Url{url});
    if (resp.status_code != 200) {
        Logger::get()->error("❌ [spot] HTTP {} for {} {}", resp.status_code, ticker, valuationDateISO);
        return std::nullopt;
    }

    nlohmann::json j;
    try { j = nlohmann::json::parse(resp.text); }
    catch (const std::exception& e) {
        Logger::get()->error("❌ [spot] JSON parse: {} for {} {}", e.what(), ticker, valuationDateISO);
        return std::nullopt;
    }

    if (!j.contains("close") || !j["close"].is_number()) {
        Logger::get()->error("❌ [spot] missing/invalid 'close' for {} {}", ticker, valuationDateISO);
        return std::nullopt;
    }
    return j["close"].get<double>();
}




