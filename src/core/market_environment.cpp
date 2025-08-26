#include "core/market_environment.hpp"
#include <string>
#include <optional>
#include "cpr/cpr.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include "utils/date_utils.hpp"
#include "../configs/constants.hpp"




MarketEnvironment::MarketEnvironment(const std::string& valuation_date)
    : m_valuationDate(valuation_date) {}

void MarketEnvironment::setUnderlyingPriceManually(double price) {
    m_underlyingPrice = price;
}

void MarketEnvironment::setRiskFreeRate(double riskFreeRate) {
    m_riskFreeRate = riskFreeRate;
}

void MarketEnvironment::setVolatility(double volatility) {
    m_volatility = volatility;
}

std::optional<double> MarketEnvironment::getUnderlyingPrice() const {
    return m_underlyingPrice;
}

bool MarketEnvironment::hasLivePrice() const {
    return m_underlyingPrice.has_value() && m_priceFromApi;
}

const double MarketEnvironment::getRiskFreeRate() const {
    return m_riskFreeRate;
}

const double MarketEnvironment::getVolatility() const {
    return m_volatility;
}

double MarketEnvironment::getHistoricalDrift() const {
    return m_historicalDrift;
}

double MarketEnvironment::getHistoricalVolatility() const {
    return m_historicalVolatility;
}

void MarketEnvironment::setHistoricalPrices(const std::vector<double>& prices) {
    m_historicalPrices = prices;
}

const std::vector<double>& MarketEnvironment::getHistoricalPrices() const {
    return m_historicalPrices;
}

void MarketEnvironment::estimateDriftAndVolatility() {
    if (m_historicalPrices.size() < 2) {
        throw std::runtime_error("❌ Not enough historical prices to estimate drift and volatility.");
    }

    std::vector<double> logReturns;
    for (size_t i = 1; i < m_historicalPrices.size(); ++i) {
        double r = std::log(m_historicalPrices[i] / m_historicalPrices[i - 1]);
        logReturns.push_back(r);
    }

    double mean = std::accumulate(logReturns.begin(), logReturns.end(), 0.0) / logReturns.size();
    double variance = 0.0;
    for (double r : logReturns) {
        variance += (r - mean) * (r - mean);
    }
    variance /= logReturns.size();

    m_historicalDrift = mean;
    m_historicalVolatility = std::sqrt(variance);
}
