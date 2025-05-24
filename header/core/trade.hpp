#pragma once
#include <string>
#include <optional>

// --- Common metadata across all trade types ---

struct TradeMetaData {
    int trade_id;
    std::string asset_class;
    std::string product_type;
    std::string option_style;
    std::string trade_date;
    std::string trade_maturity;
    std::string user_id;
};



// --- Equity-specific fields ---
struct EquityTradeData {
    std::string underlying_ticker;
    double dividend;
    std::optional<double> strike;
    std::optional<std::string> payoff;
    std::optional<std::string> structured_params;
};

struct Trade {
    TradeMetaData meta;
        std::variant<EquityTradeData /*, RateTradeData, FXTradeData */> assetData;
};
