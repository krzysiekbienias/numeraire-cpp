#include "market_store/options_selectors.hpp"
#include <algorithm>
#include <cctype>
#include <unordered_map>
#include <cmath>
#include <ql/time/date.hpp>
#include <set>
#include <utils/date_utils.hpp>
#include <iterator>
#include <cstdlib>

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




static inline bool isValidTypeChar(char t) { return t == 'C' || t == 'P'; }

static std::vector<OccSymbol>
_nearestExpiry_core(const std::vector<OccSymbol>& all,
                    const std::string& targetMaturityISO,
                    bool prefer_future,
                    std::optional<char> type_filter)
{
    if (all.empty()) return {};

    const QuantLib::Date tgt = date_utils::toQLDateYYYYMMDD(targetMaturityISO);

    std::set<QuantLib::Date> exps;
    for (const auto& x : all) {
        if (!date_utils::isValidYYYYMMDD(x.expiryISO)) continue;
        if (type_filter && x.type != *type_filter) continue;
        exps.insert(date_utils::toQLDateYYYYMMDD(x.expiryISO));
    }
    if (exps.empty()) return {};

    auto it = exps.lower_bound(tgt);
    QuantLib::Date chosen;

    if (it != exps.end() && *it == tgt) {
        chosen = *it;
    } else if (prefer_future) {
        chosen = (it != exps.end()) ? *it : *std::prev(exps.end());
    } else {
        if (it == exps.begin()) {
            chosen = *it;
        } else if (it == exps.end()) {
            chosen = *std::prev(exps.end());
        } else {
            const QuantLib::Date f = *it;
            const QuantLib::Date p = *std::prev(it);
            const long df = f - tgt;
            const long dp = tgt - p;
            chosen = (df <= dp) ? f : p;
        }
    }

    const std::string chosenISO = date_utils::toYYYYMMDD(chosen);

    std::vector<OccSymbol> out;
    out.reserve(256);
    if (type_filter) {
        for (const auto& x : all) {
            if (x.expiryISO == chosenISO && x.type == *type_filter) out.push_back(x);
        }
    } else {
        for (const auto& x : all) {
            if (x.expiryISO == chosenISO) out.push_back(x);
        }
    }
    return out;
}

// public: z filtrem + fallback
std::vector<OccSymbol>
nearestExpiry(const std::vector<OccSymbol>& all,
              const std::string& targetMaturityISO,
              bool prefer_future,
              std::optional<char> type_filter,
              bool fallback_to_any)
{
    if (type_filter && !isValidTypeChar(*type_filter)) return {};

    // 1) Spróbuj z filtrem typu
    auto res = _nearestExpiry_core(all, targetMaturityISO, prefer_future, type_filter);
    if (!res.empty() || !fallback_to_any) return res;

    // 2) Fallback: ignoruj typ (weź cokolwiek jest)
    return _nearestExpiry_core(all, targetMaturityISO, prefer_future, std::nullopt);
}

// public: wersja bez filtra (backward-compatible)
std::vector<OccSymbol>
nearestExpiry(const std::vector<OccSymbol>& all,
              const std::string& targetMaturityISO,
              bool prefer_future)
{
    return nearestExpiry(all, targetMaturityISO, prefer_future, std::nullopt, /*fallback_to_any=*/true);
}

void sortInPlaceByStrike(std::vector<OccSymbol>& v) {
    std::sort(v.begin(), v.end(), [](const OccSymbol& a, const OccSymbol& b){
        if (a.strike_milli != b.strike_milli) return a.strike_milli < b.strike_milli;
        return a.type < b.type; // tylko stabilny porządek w obrębie strike’a
    });
}


int nearestIdxBySpot(const std::vector<OccSymbol>& v, double spot) {
    if (v.empty()) return -1;
    const long long spot_milli = static_cast<long long>(std::llround(spot * 1000.0));

    // lower_bound po strike_milli bez sortowania też „coś” zwróci, ale zakładamy,
    // że profilaktycznie i tak sortujemy wcześniej (pickAroundATM to robi).
    auto it = std::lower_bound(
        v.begin(), v.end(), spot_milli,
        [](const OccSymbol& s, long long x){ return s.strike_milli < x; }
    );

    if (it == v.end())   return static_cast<int>(v.size()) - 1;
    if (it == v.begin()) return 0;

    int i2 = static_cast<int>(std::distance(v.begin(), it));
    int i1 = i2 - 1;

    return (std::llabs(v[i1].strike_milli - spot_milli) <=
            std::llabs(v[i2].strike_milli - spot_milli)) ? i1 : i2;
}


