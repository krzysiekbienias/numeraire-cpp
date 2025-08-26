#include "database/mappers/trade_row_mapper.hpp"
#include "utils/sqlite_utils.hpp"
#include "utils/logger.hpp"

namespace {
    enum Col {
        TRADE_ID = 0,
        ASSET_CLASS,
        UNDERLYING_TICKER,
        PRODUCT_TYPE,
        PAYOFF,
        TRADE_DATE,
        TRADE_MATURITY,
        DIVIDEND,
        OPTION_STYLE,
        STRUCTURED_PARAMS,
        USER_ID,
        STRIKE
    };

    EquityTradeData buildEquityData(sqlite3_stmt* row, const std::string& ticker) {
        EquityTradeData eq;
        eq.underlying_ticker = ticker;
        eq.dividend          = sqlite3_column_double(row, Col::DIVIDEND);
        eq.strike            = sqlite_utils::getDoubleOrNull(row, Col::STRIKE);
        eq.payoff            = sqlite_utils::getStringOrNull(row, Col::PAYOFF);
        eq.structured_params = sqlite_utils::getStringOrNull(row, Col::STRUCTURED_PARAMS);
        return eq;
    }
} // namespace

std::optional<Trade> TradeRowMapper::fromRow(sqlite3_stmt* row) {
    TradeMetaData meta;
    meta.trade_id       = sqlite3_column_int(row, Col::TRADE_ID);
    meta.asset_class    = reinterpret_cast<const char*>(sqlite3_column_text(row, Col::ASSET_CLASS));
    std::string ticker  = reinterpret_cast<const char*>(sqlite3_column_text(row, Col::UNDERLYING_TICKER));
    meta.product_type   = reinterpret_cast<const char*>(sqlite3_column_text(row, Col::PRODUCT_TYPE));
    meta.trade_date     = reinterpret_cast<const char*>(sqlite3_column_text(row, Col::TRADE_DATE));
    meta.trade_maturity = reinterpret_cast<const char*>(sqlite3_column_text(row, Col::TRADE_MATURITY));
    meta.option_style   = reinterpret_cast<const char*>(sqlite3_column_text(row, Col::OPTION_STYLE));
    meta.user_id        = reinterpret_cast<const char*>(sqlite3_column_text(row, Col::USER_ID));

    if (meta.asset_class == "Equity") {
        Trade t;
        t.meta      = std::move(meta);
        t.assetData = buildEquityData(row, ticker);
        return t;
    }

    Logger::get()->error("❌ Unsupported asset class: {}", meta.asset_class);
    return std::nullopt;
}
