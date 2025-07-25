#include <iostream>
#include <thread>
#include "core/schedule_generator.hpp"
#include "utils/logger.hpp"
#include "utils/print_utils.hpp"
#include "utils/path_utils.hpp"
#include "database/trade_store.hpp"
#include "utils/json_utils.hpp"
#include "core/market_env_cache.hpp"
#include "core/market_data_fetcher.hpp"
#include "pricing/asset_class/equity/equity_pricer_factory.hpp"

int main(int argc, char** argv) {
    print_utils::printBoxedLabel("📈 Numeraire++ Portfolio Pricing");

    std::string projectPath = resolveProjectPath(argv[0]);
    std::string configPath = resolveMainConfigPath(argv[0]);

    JsonUtils::load("main", configPath);
    auto mainConfigMap = JsonUtils::toStringMap("main");
    
    JsonUtils::load("secrets", projectPath + JsonUtils::toStringMap("main").at("SECRETS_CONFIG"));
    auto secrets = JsonUtils::toStringMap("secrets");

    std::string valuationDate = "17-01-2025";

    // === Step 1: Retrieve trades from DB ===
    print_utils::printBoxedLabel("📡 Trade Retrieval");
    TradesStore store(projectPath + JsonUtils::toStringMap("main").at("DB_PATH"));
    auto allTrades = store.getAllTrades();
    std::cout << "📋 Total trades retrieved: " << allTrades.size() << "\n";
    
    // === Step 2: Load cached market data or fall back ===
    MarketEnvironment marketEnv(valuationDate);
    MarketEnvCache cache(projectPath + "/cached_market_data");
    std::string cacheFilePath = projectPath + "/cached_market_data/" + valuationDate + "_market.json";
    
    // 🔌 Load MarketEnv cache
    try {
        cache.loadFromFile(cacheFilePath);
        std::cout << "📂 Loaded market cache from: " << cacheFilePath << "\n";
    } catch (const std::exception& e) {
        std::cout << "⚠️ Could not load cache: " << e.what() << "\n";
    }
    
    int pricedCount = 0;
    int skippedCount = 0;

    for (const auto& trade : allTrades) {
        print_utils::printBoxedLabel("🧾 Trade ID " + std::to_string(trade.meta.trade_id));

        try {
            if (trade.meta.asset_class != "Equity") {
                std::cout << "⚠️ Skipping unsupported trade type: " << trade.meta.asset_class << "\n";
                skippedCount++;
                continue;
            }

            // 🎯 Build pricer factory
            EquityPricerFactory pf(valuationDate, projectPath + mainConfigMap.at("SCHEDULE_CONFIG"));
            pf.setTrade(trade);
            pf.buildSchedule();

            // 🧠 Load env from cache
            std::string ticker = pf.getUnderlierTicker();
            if (!cache.hasDataFor(ticker)) {
                std::cout << "⚠️ Skipping trade — no cached market data for ticker: " << ticker << "\n";
                skippedCount++;
                continue;
            }

            MarketEnvironment env = cache.getEnvFor(ticker);
            pf.setMarketEnvironment(env);
            pf.extractUnderlyingPrice(); // optional if already in env

            auto pricer = pf.createPricer();
            double price = pricer->price();
            std::cout << "💰 Price: " << price << "\n";
            pricedCount++;

        } catch (const std::exception& ex) {
            std::cout << "❌ Pricing failed: " << ex.what() << "\n";
            skippedCount++;
        }
    }

    // 📊 Summary
    print_utils::printBoxedLabel("📊 Portfolio Pricing Summary");
    std::cout << "✅ Priced successfully : " << pricedCount << "\n";
    std::cout << "⚠️ Skipped or failed   : " << skippedCount << "\n";
    std::cout << "📈 Total trades        : " << allTrades.size() << "\n";
    
    
    return 0;
}
