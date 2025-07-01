# pragma once
# include <string>


class IDiscretizationScheme {
public:
    virtual ~IDiscretizationScheme() = default;

    virtual double nextStep(
        double current,
        double dt,
        double drift,
        double volatility
    ) const = 0;

    virtual std::string name() const = 0;
};
