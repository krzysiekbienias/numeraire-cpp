# pragma once

class IDiscretizationScheme {
public:
    virtual ~IDiscretizationScheme() = default;

    virtual double nextStep(
        double current,
        double dt,
        double drift,
        double diffusion
    ) const = 0;

    virtual std::string name() const = 0;
    virtual void setTimeSteps(const std::vector<double>& dt) = 0;
    virtual double simulateNextStep(double current, size_t stepIndex) const = 0;
};
