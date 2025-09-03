#pragma once
#include <string>
#include <vector>
#include <optional>

struct OccSymbol {
    std::string underlying;   // "AAPL"
    std::string expiryISO;    // "YYYY-MM-DD"
    char        type;         // 'C' / 'P'
    long long   strike_milli; // np. 250000 => 250.000
    double      strike;       // np. 250.0
    std::string raw;
};

/**
 * @brief Parses a Polygon/OCC-style option symbol into a structured OccSymbol.
 *
 * Expects symbols in the format:
 *   "O:" + UNDERLYING + YYMMDD + [C|P] + STRIKE_8_DIGITS
 *
 * Where:
 *  - UNDERLYING  : e.g. "AAPL"
 *  - YYMMDD      : expiry date (two-digit year), e.g. "250822" → "2025-08-22"
 *  - [C|P]       : option type (Call or Put)
 *  - STRIKE_8_DIGITS : strike price * 1000, zero-padded to 8 digits
 *                      e.g. "00150000" → 150.000
 *
 * On success, the returned OccSymbol contains:
 *  - underlying      : underlying ticker (e.g., "AAPL")
 *  - expiryISO       : ISO date "YYYY-MM-DD" (e.g., "2025-08-22")
 *  - type            : 'C' or 'P'
 *  - strike_milli    : integer strike * 1000 (e.g., 150000)
 *  - strike          : double strike in natural units (e.g., 150.0)
 *  - raw             : original input symbol
 *
 * @param occ The input OCC symbol string (e.g., "O:AAPL250822C00150000").
 * @return std::optional<OccSymbol> A populated OccSymbol on success,
 *         or std::nullopt if the input does not match the expected format.
 *
 * @note This function does not throw on parse failure; use the optional to detect errors.
 *
 * @code
 * if (auto oc = parseOne("O:AAPL250822C00150000")) {
 *     // use oc->underlying, oc->expiryISO, oc->type, oc->strike, ...
 * } else {
 *     // handle malformed symbol
 * }
 * @endcode
 */

std::optional<OccSymbol> parseOne(const std::string& occ);


std::vector<OccSymbol> parseAllVec(const std::vector<std::string>& symbols);


std::vector<OccSymbol>
nearestExpiry(const std::vector<OccSymbol>& all,
              const std::string& targetMaturityISO,
              bool prefer_future = true);