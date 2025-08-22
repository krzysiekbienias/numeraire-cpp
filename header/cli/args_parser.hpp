// header/cli/args_parser.hpp
#pragma once
#include <string>
#include <optional>

namespace cli {

    enum class Phase : unsigned {
        None          = 0,
        Spot          = 1 << 0,
        OptionSymbols = 1 << 1,
        OptionValues  = 1 << 2,
        All           = Spot | OptionSymbols | OptionValues
    };
    inline Phase operator|(Phase a, Phase b) {
        return static_cast<Phase>(static_cast<unsigned>(a) | static_cast<unsigned>(b));
    }
    inline bool has(Phase p, Phase f) {
        return (static_cast<unsigned>(p) & static_cast<unsigned>(f)) != 0;
    }

    struct Args {
        std::string dateISO;
        std::string startISO, endISO;
        std::string holidaysCsv;
        Phase phases = Phase::None;
        bool showHelp = false;
    };

    struct ParseResult {
        std::optional<Args> args;
        std::string error;
    };

    ParseResult parse(int argc, char** argv);   // pure: no I/O
    std::string help_text();                    // just returns the text

} // namespace cli
