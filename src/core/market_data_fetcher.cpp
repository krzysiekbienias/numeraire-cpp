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


// market_data_fetcher.cpp
std::optional<std::vector<std::string>>
MarketDataFetcher::getOptionSymbolsForDate(const std::string& underlyingTicker,
                                           const std::string& asOfISO) const
{
    // Polygon v3: reference/options/contracts filtered by underlying & as_of
    //url is a bit more complicated to handle with tricky tickers.
    std::string url =
        "https://api.polygon.io/v3/reference/options/contracts"
        "?underlying_ticker=" + cpr::util::urlEncode(underlyingTicker) +
        "&as_of=" + cpr::util::urlEncode(asOfISO) +
        "&limit=1000&apiKey=" + m_apiKey;

    std::vector<std::string> symbols;
    for (;;) {
        Logger::get()->info("🔍 [opt-symbols] GET {}", url);
        auto resp = cpr::Get(cpr::Url{url});
        if (resp.status_code != 200) {
            Logger::get()->error("❌ [opt-symbols] HTTP {} for {} {}", resp.status_code, underlyingTicker, asOfISO);
            return std::nullopt;
        }

        nlohmann::json j;
        try { j = nlohmann::json::parse(resp.text); }
        catch (const std::exception& e) {
            Logger::get()->error("❌ [opt-symbols] JSON parse: {} for {} {}", e.what(), underlyingTicker, asOfISO);
            return std::nullopt;
        }

        if (!j.contains("results") || !j["results"].is_array()) {
            Logger::get()->warn("⚠ [opt-symbols] No 'results' array for {} {}", underlyingTicker, asOfISO);
            break;
        }

        for (const auto& r : j["results"]) {
            if (r.contains("ticker") && r["ticker"].is_string()) {
                symbols.push_back(r["ticker"].get<std::string>());
            }
        }

        // Handle pagination (v3 returns next_url when more pages exist)
        if (j.contains("next_url") && j["next_url"].is_string() && !j["next_url"].get<std::string>().empty()) {
            url = j["next_url"].get<std::string>() + "&apiKey=" + m_apiKey;
        } else {
            break;
        }
    }

    if (symbols.empty()) return std::nullopt;
    Logger::get()->info("✅ [opt-symbols] Collected {} contracts for {} as_of {}", symbols.size(), underlyingTicker, asOfISO);
    return symbols;
}




