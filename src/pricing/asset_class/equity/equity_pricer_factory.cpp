#include "pricing/asset_class/equity/equity_pricer_factory.hpp"
#include "core/pricing_environment.hpp"
#include "utils/date_utils.hpp"
#include "logger.hpp"
#include "utils/json_utils.hpp"


EquityPricerFactory::EquityPricerFactory(const std::string& valuationDate):m_valuationDate(valuationDate){};

EquityPricerFactory::EquityPricerFactory(const std::string& valuationDate,
                               const std::string& scheduleConfigPath)
    : m_valuationDate(valuationDate) {
    
    JsonLoader::load("schedule config",scheduleConfigPath);
        auto schedCfg=JsonLoader::toStringMap("schedule config");
    m_scheduleGen.configureFromMap(schedCfg);
}


const ScheduleGenerator& EquityPricerFactory:: getScheduleConfig() const{
    return m_scheduleGen;
}

void EquityPricerFactory::setTrade(const Trade& trade) {
    // Step 1: Validate and parse trade_date and maturity using date_utils

    try {
        QuantLib::Date start = date_utils::toQLDateDDMMYYYY(trade.meta.trade_date);
        QuantLib::Date end   = date_utils::toQLDateDDMMYYYY(trade.meta.trade_maturity);

        if (end <= start) {
            Logger::get()->error("❌ Trade maturity must be after trade date: {} vs {}", trade.meta.trade_date, trade.meta.trade_maturity);
            throw std::invalid_argument("Maturity must be after trade date.");
        }

        // Step 2: Store the trade
        m_trade = trade;
        
        //step 3L Log additional details if equity
        if (std::holds_alternative<EquityTradeData>(trade.assetData)) {
            const auto & eq=std::get<EquityTradeData>(trade.assetData);
            Logger::get()->info("✅ Trade set: ID {}, Underlying {}", trade.meta.trade_id, eq.underlying_ticker);
        }
        else{
            Logger::get()->warn("⚠️ Trade set, but asset type not recognized for logging.");
        }
        
        
    } catch (const std::exception& e) {
        Logger::get()->error("❌ Failed to set trade: {}", e.what());
        throw;  // rethrow to surface error upstream
    }
}



QuantLib::Schedule EquityPricerFactory::buildSchedule() const {
    if (!m_trade.has_value()) {
        Logger::get()->error("❌ Cannot build schedule: trade not set.");
        throw std::runtime_error("Trade is not set.");
    }

    const auto& trade = m_trade.value();

    // Extract and validate dates
    if (trade.meta.trade_maturity.empty()) {
        Logger::get()->error("❌ Trade missing maturity date.");
        throw std::runtime_error("Trade startDate or endDate is missing.");
    }

    Logger::get()->info("🛠️ Building schedule from {} to {}", m_valuationDate, trade.meta.trade_maturity);
    return m_scheduleGen.generate(m_valuationDate, trade.meta.trade_maturity);
}


double EquityPricerFactory::computeYearFraction() const {
    if (!m_trade.has_value()) {
        Logger::get()->error("❌ Cannot compute year fraction: trade not set.");
        throw std::runtime_error("Trade is not set.");
    }

    const auto& trade = m_trade.value();

    if (m_valuationDate.empty() || trade.meta.trade_maturity.empty()) {
        Logger::get()->error("❌ Missing valuationDate or trade maturity.");
        throw std::runtime_error("Valuation date or maturity date is missing.");
    }

    QuantLib::Date startDate = date_utils::toQLDateDDMMYYYY(m_valuationDate);
    QuantLib::Date endDate = date_utils::toQLDateDDMMYYYY(trade.meta.trade_maturity);

    const auto& dayCounter = m_scheduleGen.getDayCounter();
    double yearFraction = dayCounter.yearFraction(startDate, endDate);

    Logger::get()->info("📐 Year fraction from {} to {} using {}: {}",
                        m_valuationDate, trade.meta.trade_maturity,
                        static_cast<int>(m_scheduleGen.getDayCountEnum()),  // optional: log enum index
                        yearFraction);

    return yearFraction;
}

void EquityPricerFactory::setMarketEnvironment(const MarketEnvironment& env){
    m_marketEnv=env;
}


void EquityPricerFactory::extractUnderlyingPrice() {
    if (!m_marketEnv.has_value()) {
            throw std::runtime_error("Market environment not available.");
        }

        const auto& env = m_marketEnv.value();
        auto priceOpt = env.getUnderlyingPrice();
        if (!priceOpt.has_value()) {
            throw std::runtime_error("Underlying price not available in MarketEnvironment.");
        }

        m_underlyingPrice = priceOpt.value();
        m_priceSet = true;

        Logger::get()->info("📦 Underlying price extracted and stored: {}", m_underlyingPrice);
}

std::string EquityPricerFactory::getUnderlierTicker() const {
    if (!m_trade.has_value()) {
        throw std::runtime_error("Trade not set in PricingFactory.");
    }

    // Attempt to extract the EquityTradeData from the variant
    if (!std::holds_alternative<EquityTradeData>(m_trade->assetData)) {
        throw std::runtime_error("Asset data is not of type EquityTradeData.");
    }

    const auto& eqData = std::get<EquityTradeData>(m_trade->assetData);
    return eqData.underlying_ticker;
}

double EquityPricerFactory:: getUnderlierPrice()const{
    return m_underlyingPrice;
}

double EquityPricerFactory::getStrike() const {
    if (!m_trade.has_value()) {
        throw std::runtime_error("❌ No trade is set in PricingFactory.");
    }

    const Trade& trade = m_trade.value();

    if (!std::holds_alternative<EquityTradeData>(trade.assetData)) {
        throw std::runtime_error("❌ Trade does not contain equity data — cannot extract strike.");
    }

    const auto& equity = std::get<EquityTradeData>(trade.assetData);

    if (!equity.strike.has_value()) {
        throw std::runtime_error("❌ Strike value is missing in EquityTradeData.");
    }

    return equity.strike.value();
}


std::string EquityPricerFactory::getOptionStyle() const {
    if (!m_trade.has_value()) {
        throw std::runtime_error("❌ No trade is set in PricingFactory.");
    }

    const Trade& trade = m_trade.value();

    const std::string& style = trade.meta.option_style;

    if (style.empty()) {
        throw std::runtime_error("❌ Option style is missing in TradeMetaData.");
    }

    return style;
}

std::string EquityPricerFactory::getPayoff()const{
    if (!m_trade.has_value()) {
        throw std::runtime_error("❌ No trade is set in ProcingFactory.");
    }
    const Trade & trade =m_trade.value();
    if (!std::holds_alternative<EquityTradeData>(trade.assetData)){
        throw std::runtime_error("❌ Trade does not contain equity data — cannot extract strike.");
        
    }
    const auto & equity=std::get<EquityTradeData>(trade.assetData);
    if(!equity.payoff.has_value()){
        throw std::runtime_error("❌ Trade does not contain equity data - cannot extract payoff.");
    }
    return equity.payoff.value();
}

double EquityPricerFactory::getRiskFreeRate()const{
    return m_marketEnv->getRiskFreeRate();
}

double EquityPricerFactory::getVolatility()const{
    return m_marketEnv->getVolatility();
}
