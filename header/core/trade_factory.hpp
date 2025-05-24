#pragma once
#include"trade.hpp"
#include <sqlite3.h>

class TradeFactory {
public:
    static std::optional<Trade> buildFromRow(sqlite3_stmt* row);
};
