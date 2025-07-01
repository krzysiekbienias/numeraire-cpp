#pragma once
#include "discretization_scheme.hpp"
#include <string>

class EulerScheme : public IDiscretizationScheme {
public:
    double nextStep(double current, double dt, double drift, double volatility) const override;
    std::string name() const override { return "Euler"; }
};
