#pragma once
#include "pricing/asset_class/equity/equity_pricer_factory.hpp"
#include "pricing/option_pricer.hpp"
#include "pricing/models/black_scholes_model.hpp"


class PlainVanillaOption: public OptionPricer{
    public :
    PlainVanillaOption(const EquityPricerFactory& factory, const BlackScholesModel& model);
    
        double price() const override;
private:
    const EquityPricerFactory& m_factory;
    const BlackScholesModel & m_model;
};




