// src/cli/args_parser.cpp
#include "cli/args_parser.hpp"
#include <cstdlib>
#include <sstream>

#if !defined(NDEBUG)
constexpr const char* DEV_DEFAULT_VALUATION_DATE = "2025-08-12";
#else
constexpr const char* DEV_DEFAULT_VALUATION_DATE = "";
#endif

namespace cli {

static void apply_phase_token(Phase& out, const std::string& tok, std::string& err) {
    if (tok == "spot") out = out | Phase::Spot;
    else if (tok == "option-symbols" || tok == "opt-symbols") out = out | Phase::OptionSymbols;
    else if (tok == "option-values"  || tok == "opt-values")  out = out | Phase::OptionValues;
    else if (tok == "options" || tok == "both") out = out | (Phase::OptionSymbols | Phase::OptionValues);
    else if (tok == "all") out = Phase::All;
    else err = "unknown phase: " + tok;
}

ParseResult parse(int argc, char** argv) {
    Args a; std::string err;

    for (int i = 1; i < argc; ++i) {
        std::string k = argv[i];
        auto need = [&](const char* name)-> const char* {
            if (i + 1 >= argc) { err = std::string("missing value for ") + name; return nullptr; }
            return argv[++i];
        };

        if      (k == "--date")      { if (auto v = need("--date")) a.dateISO = v; else return {{}, err}; }
        else if (k == "--start")     { if (auto v = need("--start")) a.startISO = v; else return {{}, err}; }
        else if (k == "--end")       { if (auto v = need("--end")) a.endISO = v; else return {{}, err}; }
        else if (k == "--holidays")  { if (auto v = need("--holidays")) a.holidaysCsv = v; else return {{}, err}; }
        else if (k == "--phase") {
            if (const char* v = need("--phase")) {
                std::string phases = v; size_t pos = 0;
                while (true) {
                    size_t comma = phases.find(',', pos);
                    std::string tok = phases.substr(pos, (comma == std::string::npos) ? std::string::npos : comma - pos);
                    if (!tok.empty()) {
                        std::string e; apply_phase_token(a.phases, tok, e);
                        if (!e.empty()) return {{}, e};
                    }
                    if (comma == std::string::npos) break;
                    pos = comma + 1;
                }
            } else return {{}, err};
        } else if (k == "--help" || k == "-h") {
            a.showHelp = true;
        } else {
            return {{}, "unknown flag: " + k};
        }
    }

    // ENV / DEV defaults if no date/range given
    if (a.dateISO.empty() && a.startISO.empty() && a.endISO.empty()) {
        if (const char* env = std::getenv("VALUATION_DATE")) a.dateISO = env;
        else if (DEV_DEFAULT_VALUATION_DATE && *DEV_DEFAULT_VALUATION_DATE) a.dateISO = DEV_DEFAULT_VALUATION_DATE;
    }

    const bool single = !a.dateISO.empty();
    const bool ranged = !a.startISO.empty() || !a.endISO.empty();
    if (single && ranged) return {{}, "use either --date OR --start/--end"};
    if (!single && !ranged) return {{}, "provide --date YYYY-MM-DD or --start/--end"};
    if (ranged && (a.startISO.empty() || a.endISO.empty()))
        return {{}, "both --start and --end must be provided (YYYY-MM-DD)"};

    if (a.phases == Phase::None) a.phases = Phase::Spot; // default

    return {a, {}};
}

std::string help_text() {
    return
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

} // namespace cli
