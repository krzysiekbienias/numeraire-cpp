#include "pricing/pricing_factory.hpp"
#include "core/trade.hpp"
#include "asset_class/equity/black_scholes_pricer.hpp"
#include "ql/math/distributions/normaldistribution.hpp"

//in well know equations it is allowed to use one letter symbols. It is even clearer to preserve the conventional notation.

double normalCDF(double x) {
    static QuantLib::CumulativeNormalDistribution phi;
    return phi(x);
}

double BlackScholesPricer::price(const PricingFactory& factory) const {
    double K=factory.getStrike();
    double r=factory.getRiskFreeRate();
    double tau=factory.computeYearFraction();
    double St=factory.getUnderlierPrice();
    double sigma = factory.getVolatility();
    
    double d1 = (std::log(St / K) + (r + 0.5 * sigma * sigma) * tau) / (sigma * std::sqrt(tau));
    double d2 = d1 - sigma * std::sqrt(tau);
    std::string payoff=factory.getPayoff();
    bool isCall = (payoff == "Call");
    if (isCall) {
    } else {
        return K * std::exp(-r * tau) * normalCDF(-d2) - St * normalCDF(-d1);
    }
    
}
