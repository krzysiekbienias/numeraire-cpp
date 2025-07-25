#include <iostream>
#include <filesystem>
#include <thread>
#include <chrono>
#include <string>
#include <unordered_map>
#include "utils/logger.hpp"
#include "utils/print_utils.hpp"
#include "utils/path_utils.hpp"
#include "utils/json_utils.hpp"
#include "database/trade_store.hpp"
#include "core/market_data_fetcher.hpp"

int main(int argc, char** argv) {
    std::cout << "🧠 Welcome to Numeraire++ Market Data Collector!\n";

    // 📅 Step 1: Determine valuation date
    std::string valuationDate;
    if (argc > 1) {
        valuationDate = argv[1];
    } else {
        const char* envValDate = std::getenv("VALUATION_DATE");
        if (envValDate) {
            valuationDate = std::string(envValDate);
        } else {
            std::cerr << "❌ Valuation date not provided.\n";
            return 1;
        }
    }

    // 📂 Step 2: Resolve paths and config
    std::string projectPath = resolveProjectPath(argv[0]);
    std::string configPath = resolveMainConfigPath(argv[0]);

    JsonUtils::load("main", configPath);
    auto mainMap = JsonUtils::toStringMap("main");

    JsonUtils::load("secrets", projectPath + mainMap.at("SECRETS_CONFIG"));
    auto secretsMap = JsonUtils::toStringMap("secrets");

    std::string dbPath = projectPath + mainMap.at("DB_PATH");
    TradesStore store(dbPath);
    auto trades = store.getAllTrades();

    MarketDataFetcher fetcher(valuationDate, secretsMap.at("POLYGON_IO_API_KEY"));

    // === Spot Prices Phase ===
    print_utils::printBoxedLabel("💡 Phase 1: Spot Prices");

    nlohmann::json spotJson;
    int apiCallCount = 0;
    int processed = 0;
    int tradeLimit = 1000;

    for (const auto& trade : trades) {
        if (!std::holds_alternative<EquityTradeData>(trade.assetData)) continue;
        if (processed++ >= tradeLimit) break;

        std::string ticker = std::get<EquityTradeData>(trade.assetData).underlying_ticker;

        auto spotOpt = fetcher.queryPolygonPrice(ticker, valuationDate);
        apiCallCount++;

        if (spotOpt.has_value()) {
            spotJson[ticker] = { {"spot", *spotOpt} };
        } else {
            Logger::get()->warn("⚠️ No spot for ticker: {}", ticker);
        }

        if (apiCallCount >= 4) {
            std::cout << "⏳ API cooldown...\n";
            std::this_thread::sleep_for(std::chrono::seconds(70));
            apiCallCount = 0;
        }
    }

    std::string spotPath = projectPath + "/" + mainMap.at("MARKET_CACHE_DIR") + "/spot_snapshot_" + valuationDate + ".json";
    JsonUtils::saveToFile(spotJson, spotPath);
    Logger::get()->info("✅ Spot snapshot saved to: {}", spotPath);

    // === Historical Prices Phase ===
    print_utils::printBoxedLabel("📘 Phase 2: Historical Prices");

    nlohmann::json histJson;
    apiCallCount = 0;
    processed = 0;

    for (const auto& trade : trades) {
        if (!std::holds_alternative<EquityTradeData>(trade.assetData)) continue;
        if (processed++ >= tradeLimit) break;

        std::string ticker = std::get<EquityTradeData>(trade.assetData).underlying_ticker;

        auto histOpt = fetcher.queryPolygonHistoricalTimeSeries(ticker, config::DEFAULT_HISTORICAL_WINDOW_DAYS);
        apiCallCount++;

        if (histOpt.has_value()) {
            histJson[ticker] = { {"historical_prices", *histOpt} };
        } else {
            Logger::get()->warn("⚠️ No historical data for ticker: {}", ticker);
        }

        if (apiCallCount >= 4) {
            std::cout << "⏳ API cooldown...\n";
            std::this_thread::sleep_for(std::chrono::seconds(70));
            apiCallCount = 0;
        }
    }

    std::string histPath = projectPath + "/" + mainMap.at("MARKET_CACHE_DIR") + "/historical_prices_" + valuationDate + ".json";
    JsonUtils::saveToFile(histJson, histPath);
    Logger::get()->info("✅ Historical prices saved to: {}", histPath);

    return 0;
}
