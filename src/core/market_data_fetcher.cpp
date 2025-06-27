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
    auto spotOpt = queryPolygonPrice(ticker);
    if (!spotOpt.has_value()) {
        std::cerr << "❌ Failed to fetch price for: " << ticker << "\n";
        return false;
    }
    env.setUnderlyingPriceManually(*spotOpt);
    return true;
}

bool MarketDataFetcher::fetchHistoricalDriftAndVolatility(MarketEnvironment& env, const std::string& ticker, size_t requiredObs) {
    auto closesOpt = queryPolygonHistoricalTimeSeries(ticker, requiredObs);
    if (!closesOpt.has_value() || closesOpt->size() < 2) {
        std::cerr << "❌ Failed to retrieve valid historical prices for " << ticker << "\n";
        return false;
    }

    const auto& prices = *closesOpt;
    std::vector<double> logReturns;
    for (size_t i = 1; i < prices.size(); ++i) {
        logReturns.push_back(std::log(prices[i] / prices[i - 1]));
    }

    double mean = std::accumulate(logReturns.begin(), logReturns.end(), 0.0) / logReturns.size();
    double sq_sum = std::inner_product(logReturns.begin(), logReturns.end(), logReturns.begin(), 0.0);
    double stddev = std::sqrt((sq_sum / logReturns.size()) - (mean * mean));

    const double annualFactor = std::sqrt(252.0);
    env.setRiskFreeRate(0.0);  // drift is real-world, not risk-neutral
    env.setVolatility(stddev * annualFactor);


    return true;
}

std::optional<double> MarketDataFetcher::queryPolygonPrice(const std::string& ticker) const {
    std::string url = "https://api.polygon.io/v2/aggs/ticker/" + ticker + "/prev?adjusted=true&apiKey=" + m_apiKey;
    auto response = cpr::Get(cpr::Url{url});

    std::cout << "🔍 Response Status Code: " << response.status_code << "\n";
    std::cout << "🔍 Response Error Message: " << response.error.message << "\n";
    std::cout << "🔍 Response Text: " << response.text << "\n";
    std::cout << "🔍 Response Header Count: " << response.header.size() << "\n";

    if (response.status_code != 200) {
        std::cerr << "❌ HTTP error: " << response.status_code << "\n";
        return std::nullopt;
    }

    auto json = nlohmann::json::parse(response.text);
    if (json.contains("results") && !json["results"].empty()) {
        return json["results"][0]["c"].get<double>();
    }
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
    for (const auto& record : json["results"]) {
        closes.push_back(record["c"].get<double>());
    }

    if (closes.size() < 2) return std::nullopt;
    return closes;
}

