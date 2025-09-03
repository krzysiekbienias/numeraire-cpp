#include "market_store/options_selectors.hpp"
#include <algorithm>
#include <cctype>
#include <unordered_map>
#include <cmath>
#include <ql/time/date.hpp>
#include <set>
#include <utils/date_utils.hpp>


static inline bool all_digits(const std::string& s){
    for (unsigned char c: s) if (!std::isdigit(c)) return false;
    return !s.empty();
}

std::optional<OccSymbol> parseOne(const std::string& occ) {
    if (occ.size() < 2 || occ[0] != 'O' || occ[1] != ':') return std::nullopt;
    const std::string s = occ.substr(2);
    if (s.size() < (6 + 1 + 8)) return std::nullopt;

    const size_t strike_pos = s.size() - 8;
    const std::string strike_str = s.substr(strike_pos);
    if (!all_digits(strike_str)) return std::nullopt;

    const char type = s[strike_pos - 1];
    if (type != 'C' && type != 'P') return std::nullopt;

    const size_t yymmdd_pos = strike_pos - 1 - 6;
    const std::string yymmdd = s.substr(yymmdd_pos, 6);
    if (!all_digits(yymmdd)) return std::nullopt;

    const std::string underlying = s.substr(0, yymmdd_pos);
    const std::string yy = yymmdd.substr(0,2);
    const std::string mm = yymmdd.substr(2,2);
    const std::string dd = yymmdd.substr(4,2);
    const std::string expiryISO = "20" + yy + "-" + mm + "-" + dd;

    const long long strike_milli = std::stoll(strike_str);
    const double strike = static_cast<double>(strike_milli) / 1000.0;

    return OccSymbol{underlying, expiryISO, type, strike_milli, strike, occ};
}


std::vector<OccSymbol> parseAllVec(const std::vector<std::string>& symbols) {
    std::vector<OccSymbol> out; out.reserve(symbols.size());
    for (const auto& s : symbols) if (auto p = parseOne(s)) out.push_back(std::move(*p));
    return out;
}


static inline std::vector<std::string>
collectExpiries(const std::vector<OccSymbol>& all) {
    std::vector<std::string> xs; xs.reserve(all.size());
    for (const auto& x : all) xs.push_back(x.expiryISO);
    std::sort(xs.begin(), xs.end());
    xs.erase(std::unique(xs.begin(), xs.end()), xs.end());
    return xs;
}



std::vector<OccSymbol>
nearestExpiry(const std::vector<OccSymbol>& all,
              const std::string& targetMaturityISO,
              bool prefer_future)
{
    if (all.empty()) return {};

    // 1) Target as QL::Date (zakładamy poprawny ISO; jeśli chcesz, dołóż guard isValidYYYYMMDD)
    const QuantLib::Date tgt = date_utils::toQLDateYYYYMMDD(targetMaturityISO);

    // 2) Unikalne expiries jako QL::Date (pomijamy ewentualne śmieciowe daty)
    std::set<QuantLib::Date> exps;
    for (const auto& x : all) {
        if (date_utils::isValidYYYYMMDD(x.expiryISO)) {
            exps.insert(date_utils::toQLDateYYYYMMDD(x.expiryISO));
        }
    }
    if (exps.empty()) return {};

    // 3) Znajdź „najbliższe” expiry
    auto it = exps.lower_bound(tgt);
    QuantLib::Date chosen;

    // Exact match wygrywa zawsze
    if (it != exps.end() && *it == tgt) {
        chosen = *it;
    }
    // Preferujemy przyszłość: pierwszy >= target; jeśli brak, bierz ostatni < target
    else if (prefer_future) {
        chosen = (it != exps.end()) ? *it : *std::prev(exps.end());
    }
    // Najbliższy absolutnie (porównujemy odległość w dniach do sąsiadów)
    else {
        if (it == exps.begin()) {
            chosen = *it;                       // tylko przyszłość dostępna
        } else if (it == exps.end()) {
            chosen = *std::prev(exps.end());    // tylko przeszłość dostępna
        } else {
            const QuantLib::Date f = *it;               // pierwszy >= target
            const QuantLib::Date p = *std::prev(it);    // ostatni < target
            const long df = f - tgt;                    // dni do przodu
            const long dp = tgt - p;                    // dni do tyłu
            chosen = (df <= dp) ? f : p;
        }
    }

    // 4) Filtr — tylko symbole z wybranym expiry
    const std::string chosenISO = date_utils::toYYYYMMDD(chosen);
    std::vector<OccSymbol> out;
    out.reserve(256);
    for (const auto& x : all) {
        if (x.expiryISO == chosenISO) out.push_back(x);
    }
    return out;
}