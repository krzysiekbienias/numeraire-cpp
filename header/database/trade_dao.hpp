# pragma once
#include <iostream>
#include"core/trade.hpp"
#include <sqlite3.h>

class DbConnection; //fwd

//Dap=Dat Access Object
class TradeDao {
public:
    explicit TradeDao(DbConnection& db);
    std::vector<Trade> getAllTrades();
    std::optional<Trade> getTradeById(int trade_id);


private:
    DbConnection& m_db;
};
