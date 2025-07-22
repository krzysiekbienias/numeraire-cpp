#include "market_env_cache.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

MarketEnvCache::MarketEnvCache(const std::string& valuationDate)
    : m_valuationDate(valuationDate) {}

void MarketEnvCache::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("❌ Could not open cache file: " + path);
    }

    json j;
    file >> j;

    m_valuationDate = j.at("valuation_date").get<std::string>();
    const auto& tickers = j.at("tickers");

    for (const auto& [ticker, envData] : tickers.items()) {
        MarketEnvironment env(m_valuationDate);

        if (envData.contains("spot")) {
            env.setUnderlyingPriceManually(envData.at("spot").get<double>());
        }

        if (envData.contains("historical_prices")) {
            env.setHistoricalPrices(envData.at("historical_prices").get<std::vector<double>>());
        }

        m_envs[ticker] = env;
    }
}

void MarketEnvCache::saveToFile(const std::string& path) const {
    json j;
    j["valuation_date"] = m_valuationDate;

    json tickers;
    for (const auto& [ticker, env] : m_envs) {
        json envData;
        //📌 Spot Price
        if (env.getUnderlyingPrice().has_value()) {
            envData["spot"] = env.getUnderlyingPrice().value();
        }
        // Historical prices
        const auto& hist = env.getHistoricalPrices();
        if (!hist.empty()) {
            envData["historical_prices"] = hist;
        }
        
        // 🏦 Risk-free rate
        envData["risk_free_rate"] = env.getRiskFreeRate();
        
       
        envData["volatility"] = env.getVolatility();
                

        tickers[ticker] = envData;
    }

    j["tickers"] = tickers;

    std::ofstream out(path);
    if (!out.is_open()) {
        throw std::runtime_error("❌ Could not write cache file: " + path);
    }

    out << j.dump(4);
}

bool MarketEnvCache::hasDataFor(const std::string& ticker) const {
    return m_envs.find(ticker) != m_envs.end();
}

MarketEnvironment MarketEnvCache::getEnvFor(const std::string& ticker) const {
    auto it = m_envs.find(ticker);
    if (it == m_envs.end()) {
        throw std::runtime_error("❌ No environment cached for ticker: " + ticker);
    }
    return it->second;
}

void MarketEnvCache::insertEnv(const std::string& ticker, const MarketEnvironment& env) {
    m_envs[ticker] = env;
}
