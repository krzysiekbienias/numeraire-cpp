#include <iostream>
#include "core/dummy.hpp"
#include "core/schedule_generator.hpp"
#include "utils/logger.hpp"
#include "utils/print_utils.hpp"
#include "utils/print_domain.hpp"
#include "utils/path_utils.hpp"
#include "database/trade_store.hpp"
#include "pricing/pricing_factory.hpp"
#include "utils/json_utils.hpp"
#include "asset_class/equity/black_scholes_pricer.hpp"


int main(int argc, char** argv) {
    std::cout << "Welcome to Numeraire++ pricing engine!\n";
    print_utils::printBoxedLabel("Load main config");
    std::string projectPath=resolveProjectPath(argv[0]);
    std::string mainConfigPath=resolveMainConfigPath(argv[0]);
    JsonLoader::load("main",mainConfigPath); //loads nad cashes under "main"
    std::unordered_map<std::string, std::string> mainConfigMap=JsonLoader::toStringMap("main");
    

    print_utils::printBoxedLabel("📦 Pricing Setup");
    
    
    std::string valuationDate="17-01-2025";
    int tradeID=2;
    std::cout << "💼 Trade ID        : " << tradeID << "\n";
    std::cout << "📆 Valuation Date  : " << valuationDate << "\n";
    
    
    print_utils::printBoxedLabel("📡 Trade Retrieval");
    
    TradesStore tradeStore(projectPath+ mainConfigMap.at("DB_PATH"));

    std::cout << "🔍 Looking up trade ID: " << tradeID << " in trade store...\n";
    auto tradeOpt = tradeStore.getTradeById(tradeID);
    if (!tradeOpt) {
        std::cout << "❌ Trade not found. Aborting pricing.\n";
        return 1;
    }
    domain_debug::printTrade(tradeOpt);
    
    print_utils::printBoxedLabel("🏗️  Pricing Factory");
    std::cout << "🛠️  Building pricing factory...\n\n";
    std::cout << "✅ Trade loaded successfully into PricingFactory.\n";
    
    
    
    PricingFactory pf{valuationDate,
                     projectPath+mainConfigMap.at("SCHEDULE_CONFIG")};
    pf.setTrade(* tradeOpt);
    pf.buildSchedule();
    
    JsonLoader::load("secrets",projectPath+mainConfigMap.at("SECRETS_CONFIG"));
    std::unordered_map<std::string, std::string> secretsMap=JsonLoader::toStringMap("secrets");
    
    domain_debug::printScheduleConfig(pf.getScheduleConfig());
    double yearFraction =pf.computeYearFraction();
    std::cout << "📐 Year Fraction: " << yearFraction << "\n";
    print_utils::printBoxedLabel("Market Environment ");
    
    MarketEnvironment marketEnvironemnt{valuationDate};
    marketEnvironemnt.configurePolygonAPI(secretsMap.at("POLYGON_IO_API_KEY"));
    marketEnvironemnt.fetchUnderlyingPrice(pf.getUnderlierTicker());
    marketEnvironemnt.getUnderlyingPrice();
    marketEnvironemnt.setVolatility(0.3);
    marketEnvironemnt.setRiskFreeRate(0.03);
    
    //Inject MarketEnvironemnt Into factory
    pf.setMarketEnvironment(marketEnvironemnt);
    pf.extractUnderlyingPrice();
    print_utils::printBoxedLabel("✅ Pricing Setup Complete");
    
    print_utils::printBoxedLabel("Analytical Price");
    BlackScholesPricer analyticalPrice;
    analyticalPrice.price(pf);
    
    
    
    
    
    
    return 0;
}

