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
#include "database/trade_dao.hpp"
#include "core/market_data_fetcher.hpp"
#include "utils/report_utils.hpp"
#include "market_store/live_equity_otion_contracts.hpp"
#include "cli/args_parser.hpp"
#include <string_view>

#include "database/db_connection.hpp"
#include "market_store/spot_store.hpp"
#include "market_store/options_selectors.hpp"




int main(int argc, char **argv) {
    std::cout << "🧠 Welcome to Numeraire++ Market Data Collector!\n";
    std::string projectPath, configPath;

    // Resolve project root and config paths from argv[0]
    if (argc > 0 && argv && argv[0]) {
        projectPath = resolveProjectPath(argv[0]);
        configPath = resolveMainConfigPath(argv[0]);
    } else {
        std::cerr << "❌ argv[0] unavailable — cannot resolve paths.\n";
        return 1;
    }
    // Load main + secrets configuration JSONs
    JsonUtils::load("main", configPath);
    auto cfg = JsonUtils::toStringMap("main");

    JsonUtils::load("secrets", projectPath + cfg.at("SECRETS_CONFIG"));
    auto secretsMap = JsonUtils::toStringMap("secrets");
    // Open a database connection
    Logger::get()->info("Set up DB Connection...");
    std::string dbPath = projectPath + cfg.at("DB_PATH");

    DbConnection dbc(dbPath);
    if (!dbc.isOpen()) {
        Logger::get()->error("❌ Failed to open database connection at: {}", dbPath);
        return 2;
    }

    TradeDao tradeDao(dbc);
    const std::vector<Trade> trades = tradeDao.getAllTrades();


    // Extraction of tickers from a trade object.
    std::set<std::string> tickers;
    for (const auto &[meta, assetData]: trades) {
        if (!std::holds_alternative<EquityTradeData>(assetData))continue;
        if (const auto &eq = std::get<EquityTradeData>(assetData); !eq.underlying_ticker.empty())
            tickers.insert(eq.underlying_ticker);
    }

    // Parse CLI arguments (--date, --range, --phase, etc.)
    auto res = cli::parse(argc, argv);
    if (!res.error.empty()) {
        std::cerr << "❌ " << res.error << "\n\n" << cli::help_text();
        return 1;
    }
    if (res.args->showHelp) {
        std::cout << cli::help_text();
        return 0;
    }

    std::string spotPath = projectPath + cfg.at("MARKET_CACHE_DIR") + cfg.at("SPOT_PRICE_TARGET_DIR") +
                           "/spot_snapshot_" + res.args->dateISO + ".json";

    std::string holidayPath = projectPath + "/" + cfg.at("HOLIDAYS_FILE");
    auto bankHolidays = date_utils::loadBankHolidaysISO(holidayPath);

    // Alias for easier access to parsed CLI args
    const auto &args = *res.args;

    const bool single = !args.dateISO.empty();
    const bool ranged = !args.startISO.empty() || !args.endISO.empty();

    // Initialize API fetcher (Polygon.io) with a key from secrets
    MarketDataFetcher fetcher(secretsMap.at("POLYGON_IO_API_KEY"));


    if (single) {
        const std::string &isoDate = args.dateISO;
        if (!date_utils::isValidYYYYMMDD(isoDate)) {
            std::cerr << "❌ Invalid ISO date: " << isoDate << " (expected YYYY-MM-DD)\n";
            return 64; //<----wrong use of argument
        }
        const QuantLib::Date qlDate = date_utils::toQLDateYYYYMMDD(isoDate);
        const bool weekend = date_utils::isWeekend(qlDate);
        const bool holiday = bankHolidays.contains(qlDate);

        if (weekend || holiday) {
            std::cout << "🍹 The date you chose is bank holiday or weekend. Skipping fetch.\n";
            return 0;
        }
        //<<---skip weekend and holidays and stop the program, no fetching possible
        if (cli::has(args.phases, cli::Phase::Spot)) {
            print_utils::printBoxedLabel("Spot Price Fetching");
            if (std::filesystem::exists(spotPath)) {
                Logger::get()->info("Spot price file already exists. Skipping fetch ({})", spotPath);
            } else {
                std::unordered_map<std::string, double> spots;


                int apiCallCount = 0;

                for (auto it = tickers.begin(); it != tickers.end(); ++it) {
                    const auto &ticker = *it;
                    if (auto spotClose = fetcher.getSpotCloseForDate(ticker, isoDate)) {
                        spots[ticker] = *spotClose;
                    } else {
                        Logger::get()->warn("⚠ No close for {} on {}", ticker, isoDate);
                    }
                    ++apiCallCount;
                    if (apiCallCount >= 5) {
                        if (std::next(it) != tickers.end()) {
                            std::cout << "⏳API cooldown ...\n";
                            std::this_thread::sleep_for(std::chrono::seconds(62));
                        }
                        apiCallCount = 0; //reset counter
                    }
                }
                Logger::get()->info("🎯 Spot phase finished. Fetched {} tickers as of {}.", spots.size(), isoDate);


                if (!spots.empty()) {
                    market_store::saveSpotsToFile(spotPath, spots);
                }
            }
        }
        if (cli::has(args.phases, cli::Phase::OptionSymbols)) {
            print_utils::printBoxedLabel("Option Symbols Fetching");
            const std::string dateOptionSymbols =
                    projectPath + "/" + cfg.at("MARKET_CACHE_DIR") + "/" + cfg.at("OPTION_SYMBOLS_TARGET_DIR") + "/" +
                    isoDate;
            std::filesystem::create_directories(dateOptionSymbols);
            for (const auto &ticker: tickers) {
                const std::string outPath = dateOptionSymbols + "/" + ticker + "_" + isoDate + ".json";

                if (std::filesystem::exists(outPath)) {
                    Logger::get()->info("↪️ [opt-symbols] File exists — skipping ({})", outPath);
                    continue;
                }

                auto symbolsOpt = fetcher.getOptionSymbolsForDate(ticker, isoDate);
                if (!symbolsOpt || symbolsOpt->empty()) {
                    Logger::get()->warn("⚠ [opt-symbols] No symbols for {} as_of {}", ticker, isoDate);
                    continue;
                }

                // opcjonalnie: sort + unique dla stabilności
                std::vector<std::string> symbols = std::move(*symbolsOpt);
                std::sort(symbols.begin(), symbols.end());
                symbols.erase(std::unique(symbols.begin(), symbols.end()), symbols.end());

                if (market_store::options::saveSymbolsToFile(outPath, symbols)) {
                    Logger::get()->info("💾 [opt-symbols] Saved {} symbols for {} -> {}",
                                        symbols.size(), ticker, outPath);
                } else {
                    Logger::get()->error("❌ [opt-symbols] Save failed for {}", outPath);
                }
            }

            Logger::get()->info("🎯 Option Symbols phase finished for {} tickers (as of {}).",
                                tickers.size(), isoDate);
        }

        std::optional<std::vector<std::string> > loadedSymbolsPerTicker;
        if (cli::has(args.phases, cli::Phase::OptionValues)) {
            print_utils::printBoxedLabel("Option Pricing Fetching");
            // load symbols for date and ticker.
            // We need one path to load symbols
            // One target folder for storing option values.

            const std::string optionSymbolsDir =
                    projectPath + "/" + cfg.at("MARKET_CACHE_DIR") + "/" + cfg.at("OPTION_SYMBOLS_TARGET_DIR");


            const std::string underlyingPriceDir = projectPath + "/" + cfg.at("MARKET_CACHE_DIR") + "/" + cfg.at(
                                                       "SPOT_PRICE_TARGET_DIR");
            //we need here to traverse through trades not through tickers.
            for (const auto& [meta, assetData] : trades) {  // <<— destrukturyzacja Trade
                // bierzemy tylko equity
                if (const auto* eq = std::get_if<EquityTradeData>(&assetData);
                    eq && !eq->underlying_ticker.empty()) {
                    const std::string& ticker      = eq->underlying_ticker;
                    const std::string& maturityFromDB = meta.trade_maturity;    // not correct format must be changed
                    const std::string maturityISO=date_utils::toYYYYMMDD(maturityFromDB);


                    if (maturityISO.empty()) {
                        Logger::get()->warn("⚠ [opt-values] Trade {} has empty maturity — skip", meta.trade_id);
                        continue;
                    }
                    std::string optionSymbolsDirPath=optionSymbolsDir+"/"+isoDate+"/"+ticker+"_"+isoDate+".json";
                    loadedSymbolsPerTicker=market_store::options::loadSymbolsFromFile(optionSymbolsDirPath);
                    Logger::get()->info("Loaded {} options symbols for {}",loadedSymbolsPerTicker->size(),ticker);
                    std::vector<OccSymbol> occVectors;
                    //inside parseAllVector we traverse through all symbols per ticker so be careful about performance
                    occVectors=parseAllVec(*loadedSymbolsPerTicker);
                    Logger::get()->info("Symbols strings parsed to OCC structure.");
                    std::vector<OccSymbol> filteredToNearestExpiry;
                    filteredToNearestExpiry=nearestExpiry(occVectors,maturityISO);
                    Logger::get()->info("Filtered OCC to the nearest expiry.");
                }
            }
        }
    }
    return 0;
}
