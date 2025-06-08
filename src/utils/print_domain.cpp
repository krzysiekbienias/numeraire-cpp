#include"utils/print_domain.hpp"
#include<iostream>
#include"enum_mappers.hpp"


namespace domain_debug {

void printScheduleConfig(const ScheduleGenerator& scheduleGen) {
    std::cout << "📅 Schedule Configuration:\n";

    // For clarity, we assume your enums can be converted to strings with `enum_utils` (optional but clean)
    std::cout << "  Calendar Type      : " << enum_utils::toString(scheduleGen.getCalendarType()) << "\n";
    std::cout << "  Frequency          : " << enum_utils::toString(scheduleGen.getFrequency()) << "\n";
    std::cout << "  Business Convention: " << enum_utils::toString(scheduleGen.getDateConvention()) << "\n";
    std::cout << "  Generation Rule    : " << enum_utils::toString(scheduleGen.getDateRule()) << "\n";
    std::cout << "  Day Count Basis    : " << enum_utils::toString(scheduleGen.getDayCountEnum()) << "\n";
}

//this prints dates between start and end date.
void printGeneratedScheduleDates(const QuantLib::Schedule& schedule){
    std::cout << "Generated schedule:\n";
    for (const auto& date : schedule) {
        std::cout << date << " falls on a "<<date.weekday()<< "\n";
    }
  }


void printTrade(const std::optional<Trade>& t_opt) {
    if (!t_opt) {
        std::cout << "⚠️  No trade found.\n";
        return;
    }

    const Trade& t = *t_opt;

    // Print metadata
    std::cout << "Trade #" << t.meta.trade_id << " | Asset Class: " << t.meta.asset_class << "\n";
    std::cout << "  Product: " << t.meta.product_type << ", Style: " << t.meta.option_style << "\n";
    std::cout << "  Trade Date: " << t.meta.trade_date << ", Maturity: " << t.meta.trade_maturity << "\n";
    std::cout << "  User: " << t.meta.user_id << "\n";

    // Print asset-specific info (currently only Equity supported)
    std::visit([](const auto& asset) {
        using T = std::decay_t<decltype(asset)>;
        if constexpr (std::is_same_v<T, EquityTradeData>) {
            std::cout << "  Underlying: " << asset.underlying_ticker << "\n";
            std::cout << "  Dividend: " << asset.dividend << "\n";
            if (asset.strike)
                std::cout << "  Strike: " << *asset.strike << "\n";
            if (asset.payoff)
                std::cout << "  Payoff: " << *asset.payoff << "\n";
            if (asset.structured_params)
                std::cout << "  Structured Params: " << *asset.structured_params << "\n";
        } else {
            std::cout << "⚠️  Unknown or unsupported asset type.\n";
        }
    }, t.assetData);
}

void printTradeBook(const std::vector<Trade>& trades) {
    if (trades.empty()) {
        std::cout << "📭 No trades in the tradebook.\n";
        return;
    }

    for (const auto& t : trades) {
        printTrade(std::make_optional(t));  // Reuse the logic
        std::cout << "\n";
    }
}






}

