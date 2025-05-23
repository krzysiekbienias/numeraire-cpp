#include"database/trade_store.hpp"
#include<iostream>
#include <unistd.h>
#include"core/trade.hpp"
#include"utils/logger.hpp"

TradesStore::TradesStore(const std::string& db_path) : db(nullptr) {
    open(db_path);
}

TradesStore::~TradesStore() {
    close();
}



bool TradesStore::open(const std::string& db_path) {
    if (sqlite3_open(db_path.c_str(), &db) != SQLITE_OK) {
        Logger::get()->error("❌ Could not open DB: {}", sqlite3_errmsg(db));
        return false;
    }
    Logger::get()->info("✅ Connected to DB at: {}", db_path);
    return true;
}

bool TradesStore::openDebugMode(const std::string& db_path) {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    std::cout << "[DEBUG] CWD: " << cwd << "\n";
    std::cout << "[DEBUG] Trying to open: " << db_path << "\n";

    if (sqlite3_open(db_path.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Could not open DB: " << sqlite3_errmsg(db) << "\n";
        return false;
    }

    std::cout << "[DEBUG] DB opened successfully.\n";

    // List tables for verification
    const char* sql = "SELECT name FROM sqlite_master WHERE type='table';";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        std::cout << "[DEBUG] Tables in DB:\n";
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::cout << "  - " << sqlite3_column_text(stmt, 0) << "\n";
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to list tables: " << sqlite3_errmsg(db) << "\n";
    }

    return true;
}

void TradesStore::close() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}


std::optional<Trade> TradesStore::getTradeById(int trade_id) {
    const char* sql = R"(
        SELECT trade_id, underlying_ticker, product_type, payoff, trade_date,
               trade_maturity, dividend, option_style, structured_params,
               user_id, strike
        FROM wiener_tradebook
        WHERE trade_id = ?;
    )";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        Logger::get()->error( "Failed to prepare query for all trades: {}", sqlite3_errmsg(db));
        return std::nullopt;
    }

    sqlite3_bind_int(stmt, 1, trade_id); // bind input

    Trade t;
    bool found = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        found = true;
        t.trade_id = sqlite3_column_int(stmt, 0);
        t.underlying_ticker = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        t.product_type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        t.payoff = sqlite3_column_type(stmt, 3) != SQLITE_NULL
            ? std::optional<std::string>{reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))}
            : std::nullopt;
        t.trade_date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        t.trade_maturity = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        t.dividend = sqlite3_column_double(stmt, 6);
        t.option_style = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));
        t.structured_params = sqlite3_column_type(stmt, 8) != SQLITE_NULL
            ? std::optional<std::string>{reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8))}
            : std::nullopt;
        t.user_id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9));
        t.strike = sqlite3_column_type(stmt, 10) != SQLITE_NULL
            ? std::optional<double>{sqlite3_column_double(stmt, 10)}
            : std::nullopt;
    }

    sqlite3_finalize(stmt);
    return found ? std::optional<Trade>{t} : std::nullopt;
}


std::vector<Trade> TradesStore::getAllTrades() {
    const char* sql = R"(
        SELECT trade_id, underlying_ticker, product_type, payoff, trade_date,
               trade_maturity, dividend, option_style, structured_params,
               user_id, strike
        FROM wiener_tradebook;
    )";

    sqlite3_stmt* stmt;
    std::vector<Trade> trades;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        Logger::get()->error("❌ Failed to prepare query for all trades: {}", sqlite3_errmsg(db));
        return trades;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Trade t;
        t.trade_id = sqlite3_column_int(stmt, 0);
        t.underlying_ticker = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        t.product_type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        t.payoff = sqlite3_column_type(stmt, 3) != SQLITE_NULL
            ? std::optional<std::string>{reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))}
            : std::nullopt;
        t.trade_date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        t.trade_maturity = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        t.dividend = sqlite3_column_double(stmt, 6);
        t.option_style = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));
        t.structured_params = sqlite3_column_type(stmt, 8) != SQLITE_NULL
            ? std::optional<std::string>{reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8))}
            : std::nullopt;
        t.user_id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9));
        t.strike = sqlite3_column_type(stmt, 10) != SQLITE_NULL
            ? std::optional<double>{sqlite3_column_double(stmt, 10)}
            : std::nullopt;

        trades.push_back(std::move(t));
    }

    sqlite3_finalize(stmt);
    return trades;
}
