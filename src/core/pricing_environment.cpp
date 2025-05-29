#include "pricing_environment.hpp"
#include <string>
#include <optional>
#include "cpr/cpr.h"
#include <nlohmann/json.hpp>
#include <iostream>


MarketEnvironment::MarketEnvironment(const std::string& valuation_date)
    : m_valuationDate(valuation_date) {}


using json = nlohmann::json;

std::optional<double> MarketEnvironment::queryPolygonPrice(const std::string& ticker) const {
    if (!m_apiKey.has_value()) {
        std::cerr << "❌ API key not set.\n";
        return std::nullopt;
    }

    std::string url = "https://api.polygon.io/v2/aggs/ticker/" + ticker + "/prev?adjusted=true&apiKey=" + m_apiKey.value();

    auto response = cpr::Get(cpr::Url{url});

    if (response.status_code != 200) {
        std::cerr << "❌ HTTP error: " << response.status_code << "\n";
        return std::nullopt;
    }

    try {
        auto j = json::parse(response.text);
        if (j.contains("results") && !j["results"].empty() && j["results"][0].contains("c")) {
            double close_price = j["results"][0]["c"].get<double>();
            return close_price;
        } else {
            std::cerr << "⚠️ JSON missing expected 'results[0].c' field:\n" << j.dump(2) << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "❌ JSON parse error: " << e.what() << "\n";
    }

    return std::nullopt;
}

bool MarketEnvironment::fetchUnderlyingPrice(const std::string& ticker) {
    auto priceOpt = queryPolygonPrice(ticker);
    if (priceOpt.has_value()) {
        m_underlyingPrice = priceOpt.value();
        m_priceFromApi = true;
        std::cout << "✅ Fetched price: " << *m_underlyingPrice << "\n";
        return true;
    }
    std::cout << "❌ Failed to fetch price for: " << ticker << "\n";
    return false;
}

void MarketEnvironment::configurePolygonAPI(const std::string& api_key) {
    m_apiKey = api_key;
    std::cout << "🔐 Polygon API key configured.\n";
}




std::optional<double> MarketEnvironment::getUnderlyingPrice() const {
    return m_underlyingPrice;
}

bool MarketEnvironment::hasLivePrice() const {
    return m_priceFromApi && m_underlyingPrice.has_value();
}

void MarketEnvironment::setRiskFreeRate(double riskFreerRate){
    m_riskFreeRate=riskFreerRate;
    
}


void MarketEnvironment::setVolatility(double volatility){
    m_volatility=volatility;
}


const double MarketEnvironment::getRiskFreeRate() const{
    return m_riskFreeRate;
}

const double MarketEnvironment::getVolatility()const{
    return m_volatility;
}
