#pragma once
#include "pricing/pricing_factory.hpp"
#include "option_pricer.hpp"

class BlackScholesPricer: public OptionPricer{
    public :
    double price(const PricingFactory & factory) const override;
};



