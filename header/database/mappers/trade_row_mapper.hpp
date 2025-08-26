#pragma once
#include <optional>
#include "core/trade.hpp"
#include <sqlite3.h>

struct TradeRowMapper {
    static std::optional<Trade> fromRow(sqlite3_stmt* row);
};
