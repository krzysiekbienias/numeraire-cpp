#include "pricing/asset_class/equity/equity_pricer_factory.hpp"
#include "core/trade.hpp"
#include "pricing/asset_class/equity/product_pricers/plain_vanilla_option_pricer.hpp"
#include "ql/math/distributions/normaldistribution.hpp"

//in well know equations it is allowed to use one letter symbols. It is even clearer to preserve the conventional notation.


PlainVanillaOption::PlainVanillaOption(const EquityPricerFactory& factory, const BlackScholesModel& model):m_factory(factory),m_model(model){}

double PlainVanillaOption::price() const {
    double K=m_factory.getStrike();
    double r=m_factory.getRiskFreeRate();
    double tau=m_factory.computeYearFraction();
    double St=m_factory.getUnderlierPrice();
    double sigma = m_factory.getVolatility();
    bool isCall =(m_factory.getPayoff()=="Call");
    
    return m_model.priceVanilla(St, K, r, sigma, tau, isCall);
    
    
    
}
