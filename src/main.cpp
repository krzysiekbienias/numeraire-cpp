#include <iostream>
#include "core/dummy.hpp"
#include "core/schedule_generator.hpp"
#include "utils/logger.hpp"
#include "utils/print_utils.hpp"
#include "utils/print_domain.hpp"
#include "utils/path_utils.hpp"
#include "database/trade_store.hpp"
#include "utils/json_utils.hpp"
#include "pricing/asset_class/equity/product_pricers/plain_vanilla_option_pricer.hpp"
#include "pricing/asset_class/equity/equity_pricer_factory.hpp"


int main(int argc, char** argv) {
    std::cout << "Welcome to Numeraire++ pricing engine!\n";
    print_utils::printBoxedLabel("Load main config");

    std::string projectPath = resolveProjectPath(argv[0]);
    std::string mainConfigPath = resolveMainConfigPath(argv[0]);
    JsonLoader::load("main", mainConfigPath);
    auto mainConfigMap = JsonLoader::toStringMap("main");

    std::string valuationDate = "17-01-2025";

    print_utils::printBoxedLabel("📡 Trade Retrieval");
    TradesStore tradeStore(projectPath + mainConfigMap.at("DB_PATH"));
    std::vector<Trade> allTrades = tradeStore.getAllTrades();
    std::cout << "📋 Total trades retrieved: " << allTrades.size() << "\n";

    JsonLoader::load("secrets", projectPath + mainConfigMap.at("SECRETS_CONFIG"));
    auto secretsMap = JsonLoader::toStringMap("secrets");

    static BlackScholesModel bsModel;

    int pricedCount = 0;
    int skippedCount = 0;
    int totalPricedWithAPI = 0;

    for (const auto& trade : allTrades) {
        print_utils::printBoxedLabel("🧾 Trade ID " + std::to_string(trade.meta.trade_id));

        try {
            if (trade.meta.asset_class != "Equity" || trade.meta.product_type != "PlainVanillaOption") {
                std::cout << "⚠️ Skipping unsupported trade type: " << trade.meta.product_type << "\n";
                skippedCount++;
                continue;
            }

            EquityPricerFactory pf(valuationDate, projectPath + mainConfigMap.at("SCHEDULE_CONFIG"));
            pf.setTrade(trade);
            pf.buildSchedule();

            MarketEnvironment marketEnv(valuationDate);
            marketEnv.configurePolygonAPI(secretsMap.at("POLYGON_IO_API_KEY"));
            marketEnv.fetchUnderlyingPrice(pf.getUnderlierTicker());
            marketEnv.setVolatility(0.3);
            marketEnv.setRiskFreeRate(0.03);

            pf.setMarketEnvironment(marketEnv);
            pf.extractUnderlyingPrice();

            PlainVanillaOption pricer(pf, bsModel);
            double price = pricer.price();
            std::cout << "💰 Price: " << price << "\n";

            pricedCount++;
            totalPricedWithAPI++;

            if (totalPricedWithAPI % 5 == 0) {
                std::cout << "⏳ Reached 5 pricing API calls, sleeping 30s...\n";
                std::this_thread::sleep_for(std::chrono::seconds(70));
            }

        } catch (const std::exception& ex) {
            std::cout << "❌ Pricing failed: " << ex.what() << "\n";
            skippedCount++;
        }
    }

    print_utils::printBoxedLabel("📊 Portfolio Pricing Summary");
    std::cout << "✅ Priced successfully : " << pricedCount << "\n";
    std::cout << "⚠️ Skipped or failed   : " << skippedCount << "\n";
    std::cout << "📈 Total trades        : " << allTrades.size() << "\n";

    return 0;
}

