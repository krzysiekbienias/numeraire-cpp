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
#include "market_store/live_equity_otion_contracts.hpp"


#if !defined(NDEBUG)
constexpr const std::string_view DEV_DEFAULT_VALUATION_DATE = "2025-08-12";  // <— DEV testing date
#else
constexpr const std::string_view DEV_DEFAULT_VALUATION_DATE = "";
#endif



namespace cli {

// ---- argumenty CLI ---------------------------------------------------------
struct Args {
    std::string dateISO;            // --date YYYY-MM-DD
    std::string startISO, endISO;   // --start/--end YYYY-MM-DD
    std::string holidaysCsv;        // --holidays path.csv

    // Fazy (wynik parsowania --phase)
    bool doSpot = false;
    bool doOptSymbols = false;
    bool doOptValues  = false;
};

static void print_help() {
    std::cout <<
R"(Usage:
  main_cache_market_data --date YYYY-MM-DD [--phase spot|options|both|option-symbols|option-values|all] [--holidays path.csv]
  main_cache_market_data --start YYYY-MM-DD --end YYYY-MM-DD [--phase ...] [--holidays path.csv]

Notes:
  - Dates must be ISO (YYYY-MM-DD).
  - You can pass multiple --phase flags or a comma-separated list, e.g.:
      --phase spot --phase option-values
      --phase option-symbols,option-values
  - Backward compatibility:
      --phase options == option-symbols + option-values
      --phase both    == option-symbols + option-values
  - If no --phase is provided, default is: spot
  - Holidays CSV: one ISO date per line; lines starting with '#' are ignored.
)";
}

static void apply_phase_token(Args& out, const std::string& tok, std::string& err) {
    if      (tok == "spot")            out.doSpot = true;
    else if (tok == "option-symbols" || tok == "opt-symbols") out.doOptSymbols = true;
    else if (tok == "option-values"  || tok == "opt-values")  out.doOptValues  = true;
    else if (tok == "options" || tok == "both") { // kompatybilność
        out.doOptSymbols = true; out.doOptValues = true;
    }
    else if (tok == "all") {
        out.doSpot = out.doOptSymbols = out.doOptValues = true;
    }
    else {
        err = "unknown phase: " + tok;
    }
}

static bool parse(int argc, char** argv, Args& out, std::string& err) {
    for (int i = 1; i < argc; ++i) {
        std::string k = argv[i];
        auto need = [&](const char* name)->const char* {
            if (i+1 >= argc) { err = std::string("missing value for ") + name; return nullptr; }
            return argv[++i];
        };

        if      (k == "--date")      { if (auto v = need("--date"))      out.dateISO = v; else return false; }
        else if (k == "--start")     { if (auto v = need("--start"))     out.startISO = v; else return false; }
        else if (k == "--end")       { if (auto v = need("--end"))       out.endISO = v; else return false; }
        else if (k == "--holidays")  { if (auto v = need("--holidays"))  out.holidaysCsv = v; else return false; }
        else if (k == "--phase") {
            if (const char* v = need("--phase")) {
                std::string phases = v;
                size_t pos = 0;
                while (pos != std::string::npos) {
                    size_t comma = phases.find(',', pos);
                    std::string tok = phases.substr(pos, comma == std::string::npos ? phases.size()-pos : comma-pos);
                    if (!tok.empty()) {
                        std::string e;
                        apply_phase_token(out, tok, e);
                        if (!e.empty()) { err = e; return false; }
                    }
                    if (comma == std::string::npos) break;
                    pos = comma + 1;
                }
            } else return false;
        }
        else if (k == "--help" || k == "-h") { err.clear(); return false; }
        else { err = "unknown flag: " + k; return false; }
    }

    // fallback: ENV single-date, potem DEV default (tylko jeśli nie podano żadnej daty)
    if (out.dateISO.empty() && out.startISO.empty() && out.endISO.empty()) {
        if (const char* env = std::getenv("VALUATION_DATE")) {
            out.dateISO = env;
        } else if (!DEV_DEFAULT_VALUATION_DATE.empty()) {
            out.dateISO = std::string(DEV_DEFAULT_VALUATION_DATE);
            std::cerr << "ℹ️ Using DEV default --date=" << out.dateISO << "\n";
        }
    }

    const bool single = !out.dateISO.empty();
    const bool ranged = !out.startISO.empty() || !out.endISO.empty();
    if (single && ranged) { err = "use either --date OR --start/--end"; return false; }
    if (!single && !ranged) { err = "provide --date YYYY-MM-DD or --start/--end"; return false; }
    if (ranged && (out.startISO.empty() || out.endISO.empty())) {
        err = "both --start and --end must be provided (YYYY-MM-DD)";
        return false;
    }

    // default: jeśli nie wskazano faz → spot
    if (!out.doSpot && !out.doOptSymbols && !out.doOptValues) {
        out.doSpot = true;
    }
    return true;
}

} // namespace cli




int main(int argc, char** argv) {
    std::cout << "🧠 Welcome to Numeraire++ Market Data Collector!\n";
    std::string projectPath, configPath;

    if (argc > 0 && argv && argv[0]) {
        projectPath = resolveProjectPath(argv[0]);
        configPath  = resolveMainConfigPath(argv[0]);
    } else {
        std::cerr << "❌ argv[0] unavailable — cannot resolve paths.\n";
        return 1;
    }
    
    JsonUtils::load("main", configPath);
    auto mainMap = JsonUtils::toStringMap("main");

    JsonUtils::load("secrets", projectPath + mainMap.at("SECRETS_CONFIG"));
    auto secretsMap = JsonUtils::toStringMap("secrets");

    std::string dbPath = projectPath + mainMap.at("DB_PATH");
    
    cli::Args args;
    std::string err; //<------ error comming from parser
    
    
    // if cli::parse(...)==false GAME OVER just print helper/error and stop the program
    if (!cli::parse(argc, argv, args, err)) {
        if (!err.empty()) std::cerr << "❌ " << err << "\n\n";
        cli::print_help();
        return err.empty() ? 0 : 1;
    }
    
    // --- Validate dates ---
    auto check_iso = [&](const std::string& iso, const char* name)->bool{
        if (!date_utils::isValidYYYYMMDD(iso)) {
            std::cerr << "❌ Invalid " << name << " (expected YYYY-MM-DD): " << iso << "\n";
            return false;
        }
        return true;
    };
    
    
    
    return 0;
}
