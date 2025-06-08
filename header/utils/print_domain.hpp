#pragma once
#include "core/trade.hpp"
#include "core/schedule_generator.hpp"


namespace domain_debug {
    void printTrade(const std::optional<Trade>& t_opt);
    void printGeneratedScheduleDates(const QuantLib::Schedule& schedule);
    void printScheduleConfig(const ScheduleGenerator& scheduleGen);
    void printTradeBook(const std::vector<Trade>& trades);
}
