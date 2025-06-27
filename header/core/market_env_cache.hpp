#pragma once
#include <string>
#include <unordered_map>
#include "market_environment.hpp"

class MarketEnvCache {
public:
    // 🔧 Konstruktor z datą wyceny (potrzebny do zapisu cache na konkretny dzień)
    explicit MarketEnvCache(const std::string& valuationDate);

    // 📂 Załaduj cache z pliku JSON
    void loadFromFile(const std::string& path);

    // 💾 Zapisz cache do pliku JSON
    void saveToFile(const std::string& path) const;

    // 🔍 Sprawdź, czy mamy dane dla danego tickera
    bool hasDataFor(const std::string& ticker) const;

    // 🔍 Pobierz MarketEnvironment dla danego tickera
    MarketEnvironment getEnvFor(const std::string& ticker) const;

    // ➕ Wstaw nowe środowisko rynkowe
    void insertEnv(const std::string& ticker, const MarketEnvironment& env);

private:
    std::string m_valuationDate;
    std::unordered_map<std::string, MarketEnvironment> m_envs;
};
