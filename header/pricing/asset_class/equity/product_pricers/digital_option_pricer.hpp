#include "pricing/asset_class/equity/equity_pricer_factory.hpp"
#include "core/trade.hpp"
#include "ql/math/distributions/normaldistribution.hpp"
#include "pricing/models/black_scholes_model.hpp"

class DigitalOption : public OptionPricer {
public:
    DigitalOption(const EquityPricerFactory& factory, const BlackScholesModel& model);
    double price() const override;

private:
    const EquityPricerFactory& m_factory;
    const BlackScholesModel& m_model;
};

