#pragma once

#include "core/trade.hpp"
#include "core/pricing_environment.hpp"
#include "core/schedule_generator.hpp"

class PricingFactory {
public:
    explicit PricingFactory(const std::string& valuationDate);
     
    PricingFactory(const std::string& valuationDate,
                    const std::string& scheduleConfigPath);
    
    

    void setTrade(const Trade& trade);
    void setMarketEnvironment(const MarketEnvironment& env);
    
    

    QuantLib::Schedule buildSchedule() const;
    double computeYearFraction() const;
    
    
    //getters
    
    const std::string& getValuationDate() const;
    const ScheduleGenerator& getScheduleConfig() const;
    void extractUnderlyingPrice();
    const std::string getUnderlierTicker() const;
    
    //getters for black Scholes
    const double getUnderlierPrice()const;
    const double getStrike() const;
    const double getVolatility() const;
    const double getDividend() const;
    const double getRiskFreeRate() const;
    
private:
    std::string m_valuationDate;
    std::optional<Trade> m_trade;
    std::optional<MarketEnvironment> m_marketEnv;
    double m_underlyingPrice;
    bool m_priceSet=false;

    ScheduleGenerator m_scheduleGen;
};
