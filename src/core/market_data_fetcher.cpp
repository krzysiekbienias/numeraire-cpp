#include "core/market_data_fetcher.hpp"
#include "utils/date_utils.hpp"
#include <cmath>
#include <iostream>
#include <numeric>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include "utils/json_utils.hpp"


MarketDataFetcher::MarketDataFetcher(const std::string& valuationDate, const std::string& apiKey)
    : m_valuationDate(valuationDate), m_apiKey(apiKey) {}

bool MarketDataFetcher::fetchSpotPrice(MarketEnvironment& env, const std::string& ticker) {
    auto spotOpt = queryPolygonPrice(ticker,m_valuationDate);
    if (!spotOpt.has_value()) {
        std::cerr << "❌ Failed to fetch price for: " << ticker << "\n";
        return false;
    }
    env.setUnderlyingPriceManually(*spotOpt);
    return true;
}



std::optional<double> MarketDataFetcher::queryPolygonPrice(const std::string& ticker, const std::string& valuationDate) const {
    QuantLib::Date qlValuationDate = date_utils::toQLDateDDMMYYYY(m_valuationDate);
    std::string urlDate= date_utils::toStringYYYYMMDD(qlValuationDate);
    std::string url = "https://api.polygon.io/v1/open-close/" + ticker + "/" + urlDate + "?adjusted=true&apiKey=" + m_apiKey;
    auto response = cpr::Get(cpr::Url{url});

    std::cout << "🔍 [queryPolygonPrice] URL: " << url << "\n";
    std::cout << "🔍 Response Status Code: " << response.status_code << "\n";
    std::cout << "🔍 Response Error Message: " << response.error.message << "\n";

    if (response.status_code != 200) {
        std::cerr << "❌ HTTP error: " << response.status_code << "\n";
        return std::nullopt;
    }

    nlohmann::json json;
    try {
        json = nlohmann::json::parse(response.text);
    } catch (const std::exception& e) {
        std::cerr << "❌ JSON parse error: " << e.what() << "\n";
        return std::nullopt;
    }

    if (json.contains("close")) {
        double close = json["close"].get<double>();
        std::cout << "📅 Snapshot price for " << ticker << " on " << valuationDate << " → " << close << "\n";
        return close;
    }

    std::cerr << "❌ No 'close' field found in response for " << ticker << " on " << valuationDate << "\n";
    return std::nullopt;
}

std::optional<std::vector<double>> MarketDataFetcher::queryPolygonHistoricalTimeSeries(
    const std::string& ticker, size_t required) const {

    QuantLib::Date qlEnd = date_utils::toQLDateDDMMYYYY(m_valuationDate);
    QuantLib::Date qlStart = qlEnd - QuantLib::Period(required * 2, QuantLib::Days);
    std::string start = date_utils::toStringYYYYMMDD(qlStart);
    std::string end = date_utils::toStringYYYYMMDD(qlEnd);

    return queryPolygonHistoricalTimeSeries(ticker, start, end);
}

std::optional<std::vector<double>> MarketDataFetcher::queryPolygonHistoricalTimeSeries(
    const std::string& ticker,
    const std::string& startDate,
    const std::string& endDate) const {

    std::string url = "https://api.polygon.io/v2/aggs/ticker/" + ticker +
                      "/range/1/day/" + startDate + "/" + endDate +
                      "?adjusted=true&sort=asc&apiKey=" + m_apiKey;

    auto response = cpr::Get(cpr::Url{url});

    if (response.status_code != 200) {
        std::cerr << "❌ HTTP error: " << response.status_code << "\n";
        return std::nullopt;
    }

    auto json = nlohmann::json::parse(response.text);
    if (!json.contains("results")) return std::nullopt;

    std::vector<double> closes;
    std::cout << "📅 Historical prices with dates:\n";
    for (const auto& record : json["results"]) {
        auto ts = record["t"].get<int64_t>();  // UNIX ms
        double close = record["c"].get<double>();

        // Convert UNIX timestamp to date
        std::time_t t = ts / 1000;
        std::tm* tm = std::gmtime(&t);
        char buf[16];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d", tm);

        std::cout << "  " << buf << "  →  " << close << "\n";
        closes.push_back(close);
    }

    if (closes.size() < 2) return std::nullopt;
    return closes;
}


std::optional<nlohmann::json> MarketDataFetcher::queryPolygonOptionPrice(const std::string& optionSymbol) const {
    std::string urlDate = date_utils::toStringYYYYMMDD(date_utils::toQLDateDDMMYYYY(m_valuationDate));
    std::string url = "https://api.polygon.io/v1/open-close/" + optionSymbol + "/" + urlDate +
                      "?adjusted=true&apiKey=" + m_apiKey;

    std::cout << "🔍 [queryPolygonOptionOpenClose] URL: " << url << "\n";
    auto response = cpr::Get(cpr::Url{url});
    if (response.status_code != 200) {
        std::cerr << "❌ HTTP error: " << response.status_code << "\n";
        return std::nullopt;
    }

    try {
        return nlohmann::json::parse(response.text);
    } catch (const std::exception& e) {
        std::cerr << "❌ JSON parse error: " << e.what() << "\n";
        return std::nullopt;
    }
}

std::string MarketDataFetcher::buildPolygonOptionSymbol(const std::string& ticker,
                                                        const std::string& expiry,  // format: DD-MM-YYYY
                                                        const std::string& optionType,  // "call" or "put"
                                                        double strike) {
    std::string yymmdd = date_utils::toStringYYMMDD(date_utils::toQLDateDDMMYYYY(expiry));
    std::string cp = (optionType == "call") ? "C" : "P";
    int strikeInt = static_cast<int>(std::round(strike * 1000));
    std::ostringstream oss;
    oss << "O:" << ticker << yymmdd << cp << std::setw(8) << std::setfill('0') << strikeInt;
    return oss.str();
}


void MarketDataFetcher::fetchOptionMarketDataForTicker(
    const std::string& ticker,
    const std::string& expiry,
    double spotPrice,
    const std::string& outputDir
) {
    std::vector<int> strikeOffsets = {-40, -30, -20, -10, 0, 10, 20, 30, 40}; //TODO move to static
    std::string optionType = "call";
    
    

    for (int offset : strikeOffsets) {
        double strike = std::round((spotPrice + offset) / 5.0) * 5.0;
        bool isATM = (offset == 0);
        std::string symbol = buildPolygonOptionSymbol(ticker, expiry, optionType, strike);
        auto jsonOpt = queryPolygonOptionPrice(symbol);

        if (!jsonOpt.has_value()) {
            std::cerr << "⚠️ Failed to fetch option data for: " << symbol << "\n";
            continue;
        }

        nlohmann::json finalJson;
        finalJson["valuation_date"] = m_valuationDate;
        finalJson["underlying"] = ticker;
        finalJson["expiry"] = expiry;
        finalJson["option_type"] = optionType;
        finalJson["strike"] = strike;
        finalJson["symbol"] = symbol;
        finalJson["spot_price"] = spotPrice;
        finalJson["is_atm"] = isATM;
        finalJson["option_snapshot"] = *jsonOpt;

        std::string filename = ticker + "_" + expiry + "_" + optionType + "_" + std::to_string(static_cast<int>(strike)) + ".json";
        std::string filepath = outputDir + "/" + filename;
        
        JsonUtils::saveToFile(finalJson, filepath);
        Logger::get()->info("✅ Saved option data: {}", filepath);

    }
}



