#pragma once
#include "core/trade.hpp"
#include "core/schedule_generator.hpp"


namespace domain_debug {
    void printTrade(const std::optional<Trade>& t_opt);
    void printSchedule(const QuantLib::Schedule& schedule);
    void printTradeBook(const std::vector<Trade>& trades);
}
