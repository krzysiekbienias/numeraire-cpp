#include "euler_scheme.hpp"
#include <cmath>
#include <random>

double EulerScheme::nextStep(double current, double dt, double drift, double volatility) const {
    static thread_local std::mt19937_64 rng{std::random_device{}()};
    static thread_local std::normal_distribution<> dist(0.0, 1.0);
    double z = dist(rng);

    return current * std::exp((drift - 0.5 * volatility * volatility) * dt + volatility * std::sqrt(dt) * z);
}
