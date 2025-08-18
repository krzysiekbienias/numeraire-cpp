#include <iostream>
#include <filesystem>
#include <thread>
#include <chrono>
#include <string>
#include <algorithm>
#include <set>
#include <cctype>
#include <unordered_map>
#include "utils/logger.hpp"
#include "utils/print_utils.hpp"
#include "utils/path_utils.hpp"
#include "utils/json_utils.hpp"
#include "utils/holiday_utils.hpp"
#include "utils/date_utils.hpp"
#include "database/trade_store.hpp"
#include "core/market_data_fetcher.hpp"
#include "utils/report_utils.hpp"

int main(int argc, char** argv) {
    std::cout << "🧠 Welcome to Numeraire++ Market Data Collector!\n";
    // 📅 Step 1a: Determine valuation date
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
    
    // 📅 Step 1b: Determine which phase(s) to run
    std::string phase = (argc > 2) ? std::string(argv[2]) : "both";

    // to-lower
    auto to_lower = [](std::string s) {
        std::transform(s.begin(), s.end(), s.begin(),
                       [](unsigned char c){ return static_cast<char>(std::tolower(c)); });
        return s;
    };
    phase = to_lower(phase);

    bool runSpot = false, runOptions = false;
    if (phase == "spot" || phase == "--spot" || phase == "-s") {
        runSpot = true;
    } else if (phase == "options" || phase == "option" || phase == "--options" || phase == "-o") {
        runOptions = true;
    } else if (phase == "both" || phase == "all" || phase == "--both" || phase == "-b") {
        runSpot = runOptions = true;
    } else {
        std::cerr << "❌ Unknown phase '" << phase << "'. Use: spot | options | both\n";
        return 1;
    }

    Logger::get()->info("🚦 Selected phase: {}", phase);
    
    

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
    
    // Step 3: Load holiday list
    std::string holidayPath=projectPath+"/"+"holidays.csv";
    auto holidays=loadHolidaysFromCsv(holidayPath);
    
    Logger::get()->info("🍹 List of Bank Holidays loaded.");

    // === Spot Prices Phase ===
    if (runSpot){
        print_utils::printBoxedLabel("💡 Phase 1: Spot Prices");

        const std::string spotDir  = projectPath + "/" + mainMap.at("MARKET_CACHE_DIR") + "/spot_price";
        std::filesystem::create_directories(spotDir);

        std::string spotPath = spotDir + "/spot_snapshot_" + valuationDate + ".json";
        Logger::get()->info("🗂️ spotDir  = {}", spotDir);
        Logger::get()->info("📝 spotPath = {}", spotPath);

        nlohmann::json spotJson;
        std::string spotId = "spot_snapshot" + valuationDate;

        if (std::filesystem::exists(spotPath)) {
            Logger::get()->info("📦 Spot prices JSON already exists → skipping fetch. ({})", spotPath);
            JsonUtils::load(spotId, spotPath);
            spotJson = JsonUtils::getJson(spotId);
        } else {
            int apiCallCount = 0;
            int processed = 0;
            const int tradeLimit = 1000;

            std::set<std::string> tickers; // (opcjonalnie) deduplikacja
            for (const auto& t : trades) {
                if (!std::holds_alternative<EquityTradeData>(t.assetData)) continue;
                const auto& eq = std::get<EquityTradeData>(t.assetData);
                if (!eq.underlying_ticker.empty()) tickers.insert(eq.underlying_ticker);
            }

            for (const auto& ticker : tickers) {
                auto spotOpt = fetcher.queryPolygonPrice(ticker, valuationDate);
                apiCallCount++;

                if (spotOpt.has_value()) {
                    spotJson[ticker] = { {"spot", *spotOpt} };
                    Logger::get()->info("✅ Spot fetched for {}", ticker);
                } else {
                    Logger::get()->warn("⚠️ No spot for ticker: {}", ticker);
                }

                if (apiCallCount >= 4) {
                    std::cout << "⏳ API cooldown...\n";
                    std::this_thread::sleep_for(std::chrono::seconds(70));
                    apiCallCount = 0;
                }
            }

            // ⬇️ Zapis RAZ po pętli, więc brak „Overwriting…”
            JsonUtils::saveToFile(spotJson, spotPath);
            Logger::get()->info("✅ Spot snapshot saved to: {}", spotPath);
        }
    } else {
        Logger::get()->info("⏭️ Spot phase skipped.");
    }

    
    
    
    // === Option Prices Phase ===
    if (runOptions){
        print_utils::printBoxedLabel("💡 Phase 2: Options Data");
        
        {
            namespace fs = std::filesystem;
            
            // mini quoting do sh (żeby np. spacje w ścieżce nie rozwaliły cmd)
            auto sh_quote = [](const std::string& s) {
                std::string out; out.reserve(s.size() + 2);
                out.push_back('\'');
                for (char c : s) {
                    if (c == '\'') out += "'\\''";
                    else out.push_back(c);
                }
                out.push_back('\'');
                return out;
            };
            
            // ── Option paths
            const std::string marketCacheDir = mainMap.at("MARKET_CACHE_DIR");
            const std::string scriptPath     = projectPath + "/live_options.sh";
            
            // ── Target Directory: .../options_data_<VAL_DATE>
            const std::string optionsRoot = projectPath + "/" + marketCacheDir + "/live_options";
            fs::create_directories(optionsRoot);
            const std::string outDirForDate = optionsRoot + "/" + valuationDate;
            fs::create_directories(outDirForDate);
            
            
            // ── Unikalne tickery equity z DB
            std::set<std::string> tickers;
            for (const auto& t : trades) {
                if (!std::holds_alternative<EquityTradeData>(t.assetData)) continue;
                const auto& eq = std::get<EquityTradeData>(t.assetData);
                if (!eq.underlying_ticker.empty()) tickers.insert(eq.underlying_ticker);
            }
            
            if (tickers.empty()) {
                Logger::get()->warn("⚠️ No equity tickers found — skipping options phase.");
            } else {
                std::cout << "🔧 Option tickers:";
                for (const auto& tk : tickers) std::cout << " " << tk;
                std::cout << "\n";
                for (const auto& tk : tickers) {
                    // wywołanie skryptu tylko z argumentami:
                    // $1 = ticker, $2 = valuationDate, $3 = outputRoot
                    const std::string expectedFile = outDirForDate + "/" + tk + "_" + valuationDate + ".json";
                    if (fs::exists(expectedFile)){
                        Logger::get()->info("⏭️  Exists, skipping: {}", expectedFile);
                        continue;
                    }
                    const std::string cmd =
                    sh_quote(scriptPath) + " " +
                    sh_quote(tk) + " " +
                    sh_quote(valuationDate) + " " +
                    sh_quote(optionsRoot);
                    
                    std::cout << "▶️  " << cmd << "\n";
                    int rc = std::system(cmd.c_str());
                    if (rc != 0) {
                        Logger::get()->error("❌ Options script failed for {} (rc={})", tk, rc);
                    } else {
                        Logger::get()->info("✅ Options fetched for {}", tk);
                    }
                }
                
                Logger::get()->info("📦 Options data stored under: {}", optionsRoot);
            }
        }
    }else{
        Logger::get()->info("⏭️ Options phase skipped.");
    }

    
    return 0;
}
