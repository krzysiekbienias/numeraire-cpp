#include "core/trade_factory.hpp"
#include "utils/sqlite_utils.hpp"
#include "utils/logger.hpp"


using namespace sqlite_utils;

namespace {

// --- Helper to build EquityTradeData from row ---
EquityTradeData buildEquityData(sqlite3_stmt* row, const std::string& ticker) {
    EquityTradeData eq;
    eq.underlying_ticker = ticker;
    eq.dividend = sqlite3_column_double(row, 7);
    eq.strike = getDoubleOrNull(row, 11);
    eq.payoff = getStringOrNull(row, 4);
    eq.structured_params = getStringOrNull(row, 9);
    return eq;
}

// --- Placeholder for future FX trade support ---
/*
FXTradeData buildFXData(sqlite3_stmt* row) {
    // Extract FX-specific fields here
    FXTradeData fx;
    ...
    return fx;
}
*/

} // anonymous namespace


std::optional<Trade> TradeFactory::buildFromRow(sqlite3_stmt* row) {
    TradeMetaData meta;
    meta.trade_id       = sqlite3_column_int(row, 0);
    meta.asset_class    = reinterpret_cast<const char*>(sqlite3_column_text(row, 1));
    std::string ticker  = reinterpret_cast<const char*>(sqlite3_column_text(row, 2));
    meta.product_type   = reinterpret_cast<const char*>(sqlite3_column_text(row, 3));
    meta.trade_date     = reinterpret_cast<const char*>(sqlite3_column_text(row, 5));
    meta.trade_maturity = reinterpret_cast<const char*>(sqlite3_column_text(row, 6));
    meta.option_style   = reinterpret_cast<const char*>(sqlite3_column_text(row, 8));
    meta.user_id        = reinterpret_cast<const char*>(sqlite3_column_text(row, 10));

    if (meta.asset_class == "Equity") {
        Trade trade;
        trade.meta = meta;
        trade.assetData = buildEquityData(row, ticker);
        return trade;

    // Uncomment when you introduce FX trade support
    /*
    } else if (meta.asset_class == "FX") {
        Trade trade;
        trade.meta = meta;
        trade.assetData = buildFXData(row);
        return trade;
    */

    } else {
        Logger::get()->error("❌ Unsupported asset class: {}", meta.asset_class);
        return std::nullopt;
    }
}
