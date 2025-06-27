#pragma once
#include <optional>
#include <string>
#include <vector>
#include "../configs/constants.hpp"


class MarketEnvironment {
public:
    // 🎯 Set the valuation date for pricing and analytics
    explicit MarketEnvironment(const std::string& valuation_date);
    MarketEnvironment() = default;

    // ✍️ Allow user to manually override the underlying price (e.g. fallback)
    void setUnderlyingPriceManually(double price);

    // 🔸 Risk-neutral (Q-measure) parameters for pricing
    void setRiskFreeRate(double riskFreeRate);    // used as drift in martingale simulations
    void setVolatility(double volatility);        // used for Q-based pricing (e.g. Black-Scholes)

    // 🔍 GETTERS — general purpose
    std::optional<double> getUnderlyingPrice() const;
    bool hasLivePrice() const;

    const double getRiskFreeRate() const;
    const double getVolatility() const;

    // 🔍 GETTERS — real-world P-measure (e.g. for risk analytics)
    double getHistoricalDrift() const;
    double getHistoricalVolatility() const;

    // 📈 Historical data setters
    void setHistoricalPrices(const std::vector<double>& prices);

    // 📊 Estimate drift and volatility based on historical prices
    void estimateDriftAndVolatility();

private:
    std::string m_valuationDate;
    std::optional<double> m_underlyingPrice;
    bool m_priceFromApi = false;

    // Q-measure parameters (used in pricing models)
    double m_volatility = 0.1;
    double m_riskFreeRate = 0.3;

    // P-measure estimates (derived from historical returns)
    double m_historicalDrift = 0.1;
    double m_historicalVolatility = 0.3;

    std::vector<double> m_historicalPrices;
};
