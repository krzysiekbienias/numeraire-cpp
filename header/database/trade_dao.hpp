# pragma once
#include <iostream>
#include"core/trade.hpp"
#include <sqlite3.h>

class TradesStore {
public:
    explicit TradesStore(const std::string& db_path);  // opens connection
    ~TradesStore();                                    // closes connection automatically -> RAII

    std::vector<Trade> getAllTrades();
    std::optional<Trade> getTradeById(int trade_id);
    std::vector<Trade> getTradesByUser(const std::string& user_id);
    std::vector<Trade> getTradesByProductType(const std::string& product_type);

private:
    sqlite3* db;
    bool open(const std::string& db_path);  // only used internally
    bool openDebugMode(const std::string& db_path);
    void close();                           // only used internally
};
