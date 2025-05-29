#pragma once
#include <optional>
#include "string"
class MarketEnvironment {
public:
    explicit MarketEnvironment(const std::string& valuation_date);

    void configurePolygonAPI(const std::string& api_key);
    bool fetchUnderlyingPrice(const std::string& ticker);
    void setUnderlyingPriceManually(double price);
    
    void setRiskFreeRate(double riskFreerRate);
    void setVolatility(double volatility);

    std::optional<double> getUnderlyingPrice() const;
    bool hasLivePrice() const;
    
    const double getRiskFreeRate() const;
    
    const double getVolatility() const;
    

private:
    std::string m_valuationDate;
    std::optional<std::string> m_apiKey;
    std::optional<double> m_underlyingPrice;
    double m_volatility;
    double m_riskFreeRate;
    bool m_priceFromApi = false;
    //queryPolygonPrice is private becasue it is only used internally inside fetchUnderlyingPrice , it is stateless and const
    std::optional<double> queryPolygonPrice(const std::string& ticker) const;
};
