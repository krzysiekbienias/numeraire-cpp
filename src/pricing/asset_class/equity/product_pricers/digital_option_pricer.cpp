#include "pricing/asset_class/equity/equity_pricer_factory.hpp"
#include "core/trade.hpp"
#include "pricing/asset_class/equity/product_pricers/digital_option_pricer.hpp"
#include "ql/math/distributions/normaldistribution.hpp"

//in well know equations it is allowed to use one letter symbols. It is even clearer to preserve the conventional notation.


DigitalOption::DigitalOption(const EquityPricerFactory& factory, const BlackScholesModel& model):m_factory(factory),m_model(model){}

double DigitalOption::price() const {
    double K=m_factory.getStrike();
    double r=m_factory.getRiskFreeRate();
    double tau=m_factory.computeYearFraction();
    double St=m_factory.getUnderlierPrice();
    double sigma = m_factory.getVolatility();
    double cashPayoff=m_factory.getCashPayoff();
    bool isCall =(m_factory.getPayoff()=="Call");
    
    return m_model.priceDigital(St, K, r, sigma, tau, cashPayoff, isCall);
}

