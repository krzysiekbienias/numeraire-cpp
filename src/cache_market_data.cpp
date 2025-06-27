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
#include "../configs/constants.hpp"
#include "core/market_data_fetcher.hpp"
#include "core/market_env_cache.hpp"


int main(int argc, char** argv) {
    std::cout << "🧠 Welcome to Numeraire++ market data caching tool!\n";

    // 📅 Step 1: Determine valuation date
    std::string valuationDate;
    if (argc > 1) {
        valuationDate = argv[1];
    } else {
        const char* envValDate = std::getenv("VALUATION_DATE");
        if (envValDate) {
            valuationDate = std::string(envValDate);
        } else {
            std::cerr << "❌ Valuation date not provided. Use ./run_cache_market_data.sh <DD-MM-YYYY> or set VALUATION_DATE.\n";
            return 1;
        }
    }

    Logger::get()->info("📅 Valuation date set to: {}", valuationDate);

    // 📂 Step 2: Resolve project and config paths
    std::string projectPath = resolveProjectPath(argv[0]);
    std::string configPath = resolveMainConfigPath(argv[0]);

    // 🧩 Step 3: Load configuration files
    JsonLoader::load("main", configPath);
    auto mainMap = JsonLoader::toStringMap("main");

    JsonLoader::load("secrets", projectPath + mainMap.at("SECRETS_CONFIG"));
    auto secretsMap = JsonLoader::toStringMap("secrets");

    // 🗃️ Step 4: Load trades from DB
    std::string dbPath = projectPath + mainMap.at("DB_PATH");
    TradesStore store(dbPath);
    std::vector<Trade> trades = store.getAllTrades();

    // 🌐 Step 5: Setup fetcher and cache system
    MarketDataFetcher fetcher(valuationDate, secretsMap.at("POLYGON_IO_API_KEY"));
    MarketEnvCache cache(valuationDate);

    // 🧱 Step 6: Check for existing cache
    std::string cacheDir = projectPath + "/" + mainMap.at("MARKET_CACHE_DIR");
    std::string outPath = cacheDir + "/" + valuationDate + "_market.json";

    if (std::filesystem::exists(outPath)) {
        Logger::get()->warn("🟡 Cache file already exists: {} – skipping data fetch.", outPath);
        std::cout << "🟡 Market data already cached – skipping API calls.\n";
        return 0;
    }

    // 📊 Step 7: Init counters
    int totalTickers = 0;
    int cachedCount = 0;
    int failedCount = 0;
    int apiCallCount = 0;
    
    

    for (const auto& trade : trades) {
        if (!std::holds_alternative<EquityTradeData>(trade.assetData)) continue;

        std::string ticker = std::get<EquityTradeData>(trade.assetData).underlying_ticker;
        print_utils::printBoxedLabel("📥 Fetching market data for: " + ticker);
        totalTickers++;

        MarketEnvironment env(valuationDate);

        // --- Snapshot ---
        auto spotOpt = fetcher.queryPolygonPrice(ticker);
        apiCallCount++;
        if (!spotOpt.has_value()) {
            std::cout << "❌ No spot price for " << ticker << ", skipping.\n";
            failedCount++;
            continue;
        }
        env.setUnderlyingPriceManually(*spotOpt);

        // --- Historical ---
        auto histPricesOpt = fetcher.queryPolygonHistoricalTimeSeries(
            ticker,
            config::DEFAULT_HISTORICAL_WINDOW_DAYS
        );
        apiCallCount++;
        if (!histPricesOpt.has_value()) {
            std::cout << "❌ No historical data for " << ticker << ", skipping.\n";
            failedCount++;
            continue;
        }
        env.setHistoricalPrices(*histPricesOpt);

        // --- Save to cache ---
        cache.insertEnv(ticker, env);
        cachedCount++;

        // --- API Limit Control ---
        if (apiCallCount >= 4) {
            std::cout << "⏳ Reached 4 API calls, sleeping 70s...\n";
            std::this_thread::sleep_for(std::chrono::seconds(70));
            apiCallCount = 0;
        }
    }
    
    print_utils::printBoxedLabel("📊 Market Data Caching Summary");
    Logger::get()->info("📈 Total tickers processed   : {}", totalTickers);
    Logger::get()->info("✅ Successfully cached       : {}", cachedCount);
    Logger::get()->info("⚠️ Skipped due to error      : {}", failedCount);
    
    
    
    cache.saveToFile(outPath);
    Logger::get()->info("✅ Market data cached to: {}", outPath);

    return 0;
}
