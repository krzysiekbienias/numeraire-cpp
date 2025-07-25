#include "core/market_data_fetcher.hpp"
#include "utils/date_utils.hpp"
#include <cmath>
#include <iostream>
#include <numeric>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

MarketDataFetcher::MarketDataFetcher(const std::string& valuationDate, const std::string& apiKey)
    : m_valuationDate(valuationDate), m_apiKey(apiKey) {}

bool MarketDataFetcher::fetchSpotPrice(MarketEnvironment& env, const std::string& ticker) {
    auto spotOpt = queryPolygonPrice(ticker,m_valuationDate);
    if (!spotOpt.has_value()) {
        std::cerr << "❌ Failed to fetch price for: " << ticker << "\n";
        return false;
    }
    env.setUnderlyingPriceManually(*spotOpt);
    return true;
}



std::optional<double> MarketDataFetcher::queryPolygonPrice(const std::string& ticker, const std::string& valuationDate) const {
    QuantLib::Date qlValuationDate = date_utils::toQLDateDDMMYYYY(m_valuationDate);
    std::string urlDate= date_utils::toStringYYYYMMDD(qlValuationDate);
    std::string url = "https://api.polygon.io/v1/open-close/" + ticker + "/" + urlDate + "?adjusted=true&apiKey=" + m_apiKey;
    auto response = cpr::Get(cpr::Url{url});

    std::cout << "🔍 [queryPolygonPrice] URL: " << url << "\n";
    std::cout << "🔍 Response Status Code: " << response.status_code << "\n";
    std::cout << "🔍 Response Error Message: " << response.error.message << "\n";

    if (response.status_code != 200) {
        std::cerr << "❌ HTTP error: " << response.status_code << "\n";
        return std::nullopt;
    }

    nlohmann::json json;
    try {
        json = nlohmann::json::parse(response.text);
    } catch (const std::exception& e) {
        std::cerr << "❌ JSON parse error: " << e.what() << "\n";
        return std::nullopt;
    }

    if (json.contains("close")) {
        double close = json["close"].get<double>();
        std::cout << "📅 Snapshot price for " << ticker << " on " << valuationDate << " → " << close << "\n";
        return close;
    }

    std::cerr << "❌ No 'close' field found in response for " << ticker << " on " << valuationDate << "\n";
    return std::nullopt;
}

std::optional<std::vector<double>> MarketDataFetcher::queryPolygonHistoricalTimeSeries(
    const std::string& ticker, size_t required) const {

    QuantLib::Date qlEnd = date_utils::toQLDateDDMMYYYY(m_valuationDate);
    QuantLib::Date qlStart = qlEnd - QuantLib::Period(required * 2, QuantLib::Days);
    std::string start = date_utils::toStringYYYYMMDD(qlStart);
    std::string end = date_utils::toStringYYYYMMDD(qlEnd);

    return queryPolygonHistoricalTimeSeries(ticker, start, end);
}

std::optional<std::vector<double>> MarketDataFetcher::queryPolygonHistoricalTimeSeries(
    const std::string& ticker,
    const std::string& startDate,
    const std::string& endDate) const {

    std::string url = "https://api.polygon.io/v2/aggs/ticker/" + ticker +
                      "/range/1/day/" + startDate + "/" + endDate +
                      "?adjusted=true&sort=asc&apiKey=" + m_apiKey;

    auto response = cpr::Get(cpr::Url{url});

    if (response.status_code != 200) {
        std::cerr << "❌ HTTP error: " << response.status_code << "\n";
        return std::nullopt;
    }

    auto json = nlohmann::json::parse(response.text);
    if (!json.contains("results")) return std::nullopt;

    std::vector<double> closes;
    std::cout << "📅 Historical prices with dates:\n";
    for (const auto& record : json["results"]) {
        auto ts = record["t"].get<int64_t>();  // UNIX ms
        double close = record["c"].get<double>();

        // Convert UNIX timestamp to date
        std::time_t t = ts / 1000;
        std::tm* tm = std::gmtime(&t);
        char buf[16];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d", tm);

        std::cout << "  " << buf << "  →  " << close << "\n";
        closes.push_back(close);
    }

    if (closes.size() < 2) return std::nullopt;
    return closes;
}


std::optional<nlohmann::json> MarketDataFetcher::queryPolygonOptionsSnapshot(const std::string& underlyingTicker) const {
    std::string url = "https://api.polygon.io/v3/snapshot/options/" + underlyingTicker + "?apiKey=" + m_apiKey;
    std::cout << "🔍 [queryPolygonOptionsSnapshot] URL: " << url << "\n";

    auto response = cpr::Get(cpr::Url{url});
    if (response.status_code != 200) {
        std::cerr << "❌ HTTP error: " << response.status_code << "\n";
        return std::nullopt;
    }

    try {
        auto json = nlohmann::json::parse(response.text);
        if (json.contains("results")) {
            return json["results"];
        } else {
            std::cerr << "❌ 'results' field missing in Polygon option snapshot." << std::endl;
            return std::nullopt;
        }
    } catch (const std::exception& e) {
        std::cerr << "❌ JSON parse error: " << e.what() << std::endl;
        return std::nullopt;
    }
}
