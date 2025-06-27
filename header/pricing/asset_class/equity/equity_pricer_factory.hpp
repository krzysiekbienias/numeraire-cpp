#pragma once

#include "core/trade.hpp"
#include "core/market_environment.hpp"
#include "core/schedule_generator.hpp"
#include "pricing_factory_interface.hpp"
#include "../pricing/option_pricer.hpp"

class EquityPricerFactory :public IPricerFactory {
public:
    
    //Constructors
    explicit EquityPricerFactory(const std::string& valuationDate);
     
    EquityPricerFactory(const std::string& valuationDate,
                    const std::string& scheduleConfigPath);
    
    
    // Core setup
    void setTrade(const Trade& trade) override;
    void setMarketEnvironment(const MarketEnvironment& env) override;
    
    // Schedule and time
    QuantLib::Schedule buildSchedule() const override;
    double computeYearFraction() const override;
    const ScheduleGenerator& getScheduleConfig() const;
    const std::string& getValuationDate() const;
    
    //Trade attributes & Market
    void extractUnderlyingPrice();
    std::string getUnderlierTicker() const override;
    double getUnderlierPrice()const override;
    double getStrike() const override;
    std::string getOptionStyle() const override;
    double getVolatility() const override;
    double getDividend() const;
    double getRiskFreeRate() const override;
    std::string getPayoff() const override;
    double getCashPayoff() const;
    
    // Product Pricer creation
    std::unique_ptr<OptionPricer> createPricer() const;
    
private:
    std::string m_valuationDate;
    std::optional<Trade> m_trade;
    std::optional<MarketEnvironment> m_marketEnv;
    double m_underlyingPrice;
    bool m_priceSet=false;

    ScheduleGenerator m_scheduleGen;
};
