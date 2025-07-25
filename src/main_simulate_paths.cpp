
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
#include "simulation/path_simulator.hpp"
#include "simulation/euler_scheme.hpp"


int main(int argc, char** argv) {
    print_utils::printBoxedLabel("🎲 Numeraire++ Path Simulation");
        
    std::string projectPath = resolveProjectPath(argv[0]);
    std::string configPath = resolveMainConfigPath(argv[0]);

    // === Load configs and secrets ===
    JsonUtils::load("main", configPath);
    auto mainConfigMap = JsonUtils::toStringMap("main");

    JsonUtils::load("secrets", projectPath + mainConfigMap.at("SECRETS_CONFIG"));
    auto secrets = JsonUtils::toStringMap("secrets");

    std::string valuationDate = "17-01-2025";
    
    // === Step 1: Load Trades from DB ===
    TradesStore store(projectPath + mainConfigMap.at("DB_PATH"));
    auto allTrades = store.getAllTrades();

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
    
    int simulatedCount = 0;
    int skippedCount = 0;

    for (const auto& trade : allTrades) {
        print_utils::printBoxedLabel("🧾 Trade ID " + std::to_string(trade.meta.trade_id));

        try {
            if (trade.meta.asset_class != "Equity") {
                std::cout << "⚠️ Skipping unsupported trade type: " << trade.meta.asset_class << "\n";
                skippedCount++;
                continue;
            }

            // === Initialize pricing factory (used for schedule + parameter extraction)
            EquityPricerFactory pf(valuationDate, projectPath + mainConfigMap.at("SCHEDULE_CONFIG_MC"));
            pf.setTrade(trade);
            pf.buildSchedule();
            std::vector<std::string> dates=pf.getScheduleDateStrings();
            
            auto freq = pf.getScheduleConfig().getFrequency();

            std::string ticker = pf.getUnderlierTicker();
            if (!cache.hasDataFor(ticker)) {
                std::cout << "⚠️ Skipping trade — no cached market data for ticker: " << ticker << "\n";
                skippedCount++;
                continue;
            }

            MarketEnvironment env = cache.getEnvFor(ticker);
            
            pf.setMarketEnvironment(env);
            pf.extractUnderlyingPrice();
            std::cout << "🔎 [DEBUG] Spot in env for ticker " << ticker << ": "
                      << (env.getUnderlyingPrice().has_value() ? std::to_string(env.getUnderlyingPrice().value()) : "nullopt")
                      << "\n";

            // === Historical estimation for P-measure simulation
            env.estimateDriftAndVolatility();
            double drift = env.getHistoricalDrift();
            double vol = env.getHistoricalVolatility();

            double spot = pf.getUnderlierPrice();
            
            //set schema
            auto scheme = std::make_shared<EulerScheme>();
            std::string schemeName = scheme->name();
            
            std::vector<double> dtVec;
            
            if (freq == Frequency::Once) {
                double T = pf.computeYearFraction();
                dtVec = std::vector<double>{T};  // vector with singleton for frequency ='Once'
            } else {
                dtVec = pf.computeYearFractionVector();  // 💡 distribution along to schedule
            }
            std::cout << "Spot: " << spot << ", drift: " << drift << ", vol: " << vol << "\n";
            SimulationEngine engine(
                config::DEFAULT_NUM_PATHS,
                dtVec,
                                    
                spot,
                drift,
                vol,
                scheme,
                dates
            );

            engine.run();

            std::string outSimulationPath = projectPath + "/csv_paths/equity_simulations/tradeId_" +
                std::to_string(trade.meta.trade_id) + "_" +
                valuationDate + "_" + schemeName + "_paths.csv";

            engine.exportToCSV(outSimulationPath);

            std::cout << "📤 Paths exported to: " << outSimulationPath << "\n";

            simulatedCount++;
        } catch (const std::exception& ex) {
            std::cout << "❌ Simulation failed: " << ex.what() << "\n";
            skippedCount++;
        }
    }

    print_utils::printBoxedLabel("📊 Path Simulation Summary");
    std::cout << "✅ Simulated successfully : " << simulatedCount << "\n";
    std::cout << "⚠️ Skipped or failed      : " << skippedCount << "\n";
    std::cout << "📈 Total trades           : " << allTrades.size() << "\n";
        
    
    return 0;
}
