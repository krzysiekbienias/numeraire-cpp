#include"database/trade_dao.hpp"
#include<iostream>
#include <unistd.h>
#include"core/trade.hpp"
#include"utils/logger.hpp"
#include"utils/sqlite_utils.hpp"
#include"core/trade_factory.hpp"
#include "database/db_connection.hpp"

TradeDao::TradeDao(DbConnection &dbc) : m_db(dbc) {}


std::optional<Trade> TradeDao::getTradeById(int trade_id) {
    const std::string sql = R"(
        SELECT trade_id, asset_class, underlying_ticker, product_type, payoff, trade_date,
               trade_maturity, dividend, option_style, structured_params,
               user_id, strike
        FROM wiener_tradebook
        WHERE trade_id = ?;
    )";

    auto stmtOpt = sqlite_utils::prepareStatement(m_db.get(), sql);
    if (!stmtOpt) return std::nullopt;


    sqlite3_stmt* stmt = *stmtOpt;
    sqlite_utils::bindInt(stmt, 1, trade_id);

    std::optional<Trade> tradeOpt = std::nullopt;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        tradeOpt = TradeFactory::buildFromRow(stmt);
    }

    sqlite3_finalize(stmt);
    return tradeOpt;
}


std::vector<Trade> TradeDao::getAllTrades() {
    const char* sql = R"(
        SELECT trade_id, asset_class, underlying_ticker, product_type, payoff, trade_date,
               trade_maturity, dividend, option_style, structured_params,
               user_id, strike
        FROM wiener_tradebook;
    )";

    sqlite3_stmt* stmt;
    std::vector<Trade> trades;

    if (sqlite3_prepare_v2(m_db.get(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        Logger::get()->error("❌ Failed to prepare query for all trades: {}", sqlite3_errmsg(m_db.get()));
        return trades;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        TradeMetaData meta;
        meta.trade_id       = sqlite3_column_int(stmt, 0);
        meta.asset_class    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string ticker  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        meta.product_type   = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

        std::optional<std::string> payoff =
            sqlite3_column_type(stmt, 4) != SQLITE_NULL
                ? std::optional<std::string>{reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))}
                : std::nullopt;

        meta.trade_date     = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        meta.trade_maturity = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        double dividend     = sqlite3_column_double(stmt, 7);
        meta.option_style   = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));

        std::optional<std::string> structured_params =
            sqlite3_column_type(stmt, 9) != SQLITE_NULL
                ? std::optional<std::string>{reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9))}
                : std::nullopt;

        meta.user_id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 10));
        std::optional<double> strike =
            sqlite3_column_type(stmt, 11) != SQLITE_NULL
                ? std::optional<double>{sqlite3_column_double(stmt, 11)}
                : std::nullopt;

        Trade trade;

        if (meta.asset_class == "Equity") {
            EquityTradeData eq;
            eq.underlying_ticker = ticker;
            eq.dividend = dividend;
            eq.strike = strike;
            eq.payoff = payoff;
            eq.structured_params = structured_params;

            trade.meta = meta;
            trade.assetData = eq;
        } else {
            Logger::get()->warn("⚠️ Skipping trade with unsupported asset class: {}", meta.asset_class);
            continue;  // Skip unsupported trades
        }

        trades.push_back(std::move(trade));
    }

    sqlite3_finalize(stmt);
    return trades;
}
