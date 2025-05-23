#include"utils/print_domain.hpp"
#include<iostream>


namespace domain_debug {

void printSchedule(const QuantLib::Schedule& schedule){
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

    std::cout << "Trade #" << t.trade_id << " [" << t.underlying_ticker << "]\n";
    std::cout << "  Product: " << t.product_type << ", Style: " << t.option_style << "\n";
    if (t.payoff) std::cout << "  Payoff: " << *t.payoff << "\n";
    if (t.strike) std::cout << "  Strike: " << *t.strike << "\n";
    std::cout << "  Maturity: " << t.trade_maturity << ", Date: " << t.trade_date << "\n";
    if (t.structured_params)
        std::cout << "  Structured params: " << *t.structured_params << "\n";
}

void printTradeBook(const std::vector<Trade>& trades) {
    if (trades.empty()) {
        std::cout << "📭 No trades in the tradebook.\n";
        return;
    }

    for (const auto& t : trades) {
        printTrade(t);  // This uses the overload for confirmed Trade&
        std::cout << "\n";
    }
}

}

