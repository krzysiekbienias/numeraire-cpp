#pragma once
#include <string>
#include <ql/time/schedule.hpp>
#include "core/trade.hpp"
#include "core/pricing_environment.hpp"

class IPricerFactory {
public:
    virtual void setTrade(const Trade& trade) = 0;
    virtual void setMarketEnvironment(const MarketEnvironment& env) = 0;
    virtual double computeYearFraction() const = 0;
    virtual double getStrike() const = 0;
    virtual double getRiskFreeRate() const = 0;
    virtual double getVolatility() const = 0;
    virtual double getUnderlierPrice() const = 0;
    virtual std::string getOptionStyle() const = 0;
    virtual std::string getPayoff() const = 0;
    virtual std::string getUnderlierTicker() const = 0;
    virtual QuantLib::Schedule buildSchedule() const = 0;
    virtual ~IPricerFactory() = default;
};
