#include "utils/report_utils.hpp"
#include <filesystem>
#include <map>
#include <regex>
#include <set>
#include <iostream>

namespace report_utils {

void summarizeOptionsAgainstTrades(
    const std::string& outputDir,
    const std::vector<Trade>& trades
) {
    std::set<std::string> dbTickers;
    for (const auto& t : trades) {
        if (std::holds_alternative<EquityTradeData>(t.assetData)) {
            dbTickers.insert(std::get<EquityTradeData>(t.assetData).underlying_ticker);
        }
    }

    std::map<std::string, int> filesPerTicker;
    int totalFiles = 0;

    std::regex fnameRe(R"(^([A-Z\.]+)_[0-9]{2}-[0-9]{2}-[0-9]{4}_.+\.json$)");

    if (std::filesystem::exists(outputDir)) {
        for (const auto& dirent : std::filesystem::directory_iterator(outputDir)) {
            if (!dirent.is_regular_file()) continue;
            const std::string filename = dirent.path().filename().string();

            std::smatch m;
            if (std::regex_match(filename, m, fnameRe)) {
                const std::string ticker = m[1].str();
                filesPerTicker[ticker]++;
                totalFiles++;
            }
        }
    }

    int coveredTickers = 0;
    std::vector<std::string> missingTickers;

    for (const auto& tk : dbTickers) {
        if (filesPerTicker.find(tk) != filesPerTicker.end()) {
            coveredTickers++;
        } else {
            missingTickers.push_back(tk);
        }
    }

    std::cout << "\n================= 📊 OPTIONS SUMMARY (" << outputDir << ") =================\n";
    std::cout << "DB tickers: " << dbTickers.size()
              << " | With option files: " << coveredTickers
              << " | Missing: " << (dbTickers.size() - coveredTickers) << "\n";
    std::cout << "Total option files in folder: " << totalFiles << "\n\n";

    std::cout << "Per-ticker file counts:\n";
    for (const auto& [tk, cnt] : filesPerTicker) {
        std::cout << "  • " << tk << ": " << cnt << "\n";
    }

    if (!missingTickers.empty()) {
        std::cout << "\nTickers with NO files:\n";
        for (const auto& tk : missingTickers) {
            std::cout << "  • " << tk << "\n";
        }
    } else {
        std::cout << "\n✅ All DB tickers have at least one option file.\n";
    }
    std::cout << "=====================================================================\n\n";
}


bool hasAnyOptionFiles(const std::string& dir, const std::string& prefix) {
        for (const auto& entry : std::filesystem::directory_iterator(dir)) {
            if (entry.is_regular_file()) {
                auto name = entry.path().filename().string();
                if (name.rfind(prefix, 0) == 0) {
                    return true;
                }
            }
        }
        return false;
    }

} // namespace report_utils
