#include "pricing/models/black_scholes_model.hpp"
#include <cmath>
#include <ql/math/distributions/normaldistribution.hpp>

double normalCDF(double x) {//TODO move to some math utils
    static QuantLib::CumulativeNormalDistribution phi;
    return phi(x);
}

double BlackScholesModel::priceVanilla(double S, double K, double r, double sigma, double tau, bool isCall) const{
    double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * tau) / (sigma * std::sqrt(tau));
    double d2 = d1 - sigma * std::sqrt(tau);

    if (isCall) {
        return S * normalCDF(d1) - K * std::exp(-r * tau) * normalCDF(d2);
    } else {
        return K * std::exp(-r * tau) * normalCDF(-d2) - S * normalCDF(-d1);
    }
}


double BlackScholesModel::priceDigital(double S, double K, double r, double sigma, double tau, double cashPayoff, bool isCall) const {
    double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * tau) / (sigma * std::sqrt(tau));
    double d2 = d1 - sigma * std::sqrt(tau);
    
    if (isCall) {
        return cashPayoff * std::exp(-r * tau) * normalCDF(d2);
    } else {
        return cashPayoff * std::exp(-r * tau) * normalCDF(-d2);
    }
}


double BlackScholesModel::assetOrNothing(double S, double K, double r, double sigma, double tau, bool isCall) const {
    double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * tau) / (sigma * std::sqrt(tau));
    double d2 = d1 - sigma * std::sqrt(tau);
    
    if (isCall) {
        return S*std::exp(-r * tau) * normalCDF(d1);
    } else {
        return S*std::exp(-r * tau) * normalCDF(-d1);
    }
}
