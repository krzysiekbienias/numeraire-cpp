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

ParseResult parse(const std::vector<std::string>& args) {
    Args parsedArgs;
    std::string err;

    for (std::size_t i = 0; i < args.size(); ++i) {
        const std::string& k = args[i];

        auto need = [&](const char* name) -> const std::string* {
            if (i + 1 >= args.size()) {
                err = std::string("missing value for ") + name;
                return nullptr;
            }
            ++i;                 // consume value token
            return &args[i];
        };

        if      (k == "--date") {
            if (auto v = need("--date")) parsedArgs.dateISO = *v; else return {{}, err};
        }
        else if (k == "--start") {
            if (auto v = need("--start")) parsedArgs.startISO = *v; else return {{}, err};
        }
        else if (k == "--end") {
            if (auto v = need("--end")) parsedArgs.endISO = *v; else return {{}, err};
        }
        else if (k == "--holidays") {
            if (auto v = need("--holidays")) parsedArgs.holidaysCsv = *v; else return {{}, err};
        }
        else if (k == "--phase") {
            if (const std::string* v = need("--phase")) {
                const std::string& phases = *v;
                std::size_t pos = 0;

                while (true) {
                    std::size_t comma = phases.find(',', pos);
                    std::string tok =
                        phases.substr(pos, (comma == std::string::npos) ? std::string::npos : (comma - pos));

                    if (!tok.empty()) {
                        std::string e;
                        apply_phase_token(parsedArgs.phases, tok, e);
                        if (!e.empty()) return {{}, e};
                    }

                    if (comma == std::string::npos) break;
                    pos = comma + 1;
                }
            } else {
                return {{}, err};
            }
        }
        else if (k == "--help" || k == "-h") {
            parsedArgs.showHelp = true;
        }
        else {
            return {{}, "unknown flag: " + k};
        }
    }

    if (parsedArgs.showHelp) {
        return {parsedArgs, {}};
    }

    // ENV / DEV defaults if no date/range given
    if (parsedArgs.dateISO.empty() && parsedArgs.startISO.empty() && parsedArgs.endISO.empty()) {
        if (const char* env = std::getenv("VALUATION_DATE")) parsedArgs.dateISO = env;
        else if (DEV_DEFAULT_VALUATION_DATE && *DEV_DEFAULT_VALUATION_DATE) parsedArgs.dateISO = DEV_DEFAULT_VALUATION_DATE;
    }

    const bool single = !parsedArgs.dateISO.empty();
    const bool ranged = !parsedArgs.startISO.empty() || !parsedArgs.endISO.empty();
    if (single && ranged) return {{}, "use either --date OR --start/--end"};
    if (!single && !ranged) return {{}, "provide --date YYYY-MM-DD or --start/--end"};
    if (ranged && (parsedArgs.startISO.empty() || parsedArgs.endISO.empty()))
        return {{}, "both --start and --end must be provided (YYYY-MM-DD)"};

    if (parsedArgs.phases == Phase::None) parsedArgs.phases = Phase::Spot; // default

    return {parsedArgs, {}};
}

// Backwards-compatible wrapper:
ParseResult parse(int argc, char** argv) {
    std::vector<std::string> args;
    if (argc > 1 && argv) {
        args.reserve(static_cast<std::size_t>(argc - 1));
        for (int i = 1; i < argc; ++i) {
            args.emplace_back(argv[i] ? argv[i] : "");
        }
    }
    return parse(args);
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
