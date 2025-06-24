#include "pricing/models/black_scholes_model.hpp"
#include <cmath>
#include <ql/math/distributions/normaldistribution.hpp>

double normalCDF(double x) {
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

