# pragma once


class EulerScheme : public DiscretizationScheme {
public:
    double nextStep(double current, double dt, double mu, double sigma, double z) const override;
    std::string name() const override { return "Euler"; }
};
