#pragma once
#include <string>
#include <vector>
#include "../core/trade.hpp" // albo odpowiednia ścieżka do Trade

namespace report_utils {

    void summarizeOptionsAgainstTrades(
        const std::string& outputDir,
        const std::vector<Trade>& trades
    );
    bool hasAnyOptionFiles(const std::string& dir, const std::string& prefix);

}
