#include "pricing/pricing_factory.hpp"
#include "core/trade.hpp"
#include "asset_class/equity/black_scholes_pricer.hpp"


//in well know equations it is allowed to use one letter symbols. It is even clearer to preserve the conventional notation.

double BlackScholesPricer::price(const PricingFactory& factory) const {
    double K=factory.getStrike();
    double r=factory.getRiskFreeRate();
    double tau=factory.computeYearFraction();
    double St=factory.getUnderlierPrice();
    
    
    return 0.01;
    
}
