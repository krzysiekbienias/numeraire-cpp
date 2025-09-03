#pragma once
#include<ql/time/date.hpp>
#include<string>
#include <set>
namespace date_utils {

//Validation

/**
 * @brief Validate that a string is a real calendar date in "yyyy-mm-dd".
 *
 * Fast format check followed by a round-trip via QuantLib:
 * parse with toQLDateYYYYMMDD() and compare with toYYYYMMDD().
 *
 * @param s Candidate string, e.g. "2025-08-15".
 * @return true if @p s is a valid ISO date; false otherwise.
 */
bool isValidYYYYMMDD(const std::string& s);
/**
* @brief Check if a QuantLib::Date falls on a weekend (Saturday/Sunday).
*
* @param d QuantLib date.
* @return true if Saturday or Sunday; false otherwise.
*/
bool isWeekend(const QuantLib::Date& d);


//helper
/**
* @brief Load bank holidays from a simple text/CSV file with ISO dates.
*
* File format:
*   - One "yyyy-mm-dd" per line.
*   - Empty lines and lines starting with '#' are ignored.
*
* Lines that fail validation are logged and skipped.
* If the file cannot be opened, an empty set is returned (warning is logged).
*
* @param holidaysPath Path to the holidays file.
* @return std::set<QuantLib::Date> containing all parsed holiday dates.
*/
std::set<QuantLib::Date> loadBankHolidaysISO(const std::string& holidaysPath);


//Convertion
QuantLib::Date toQLDateDDMMYYYY (const std::string& dateStr);
QuantLib::Date toQLDateYYYYMMDD(const std::string& dateStr);
std::string toStringDDMMYYYY(const QuantLib::Date& date);
std::string toYYYYMMDD(const QuantLib::Date& date);
//overloaded version
std::string toYYYYMMDD(std::string_view date);
std::string toStringYYMMDD(const QuantLib::Date& date);

//Extracting
}
/**
 * @brief Get the 3rd Friday of a given month/year, adjusted backwards if it is a bank holiday.
 *
 * Starting from the 1st of the month, the function finds the third Friday.
 * If that date appears in @p holidays, it moves backward day-by-day until
 * a non-holiday date is reached (which may end up being a Thursday/Wednesday, etc.).
 *
 * @param year 4-digit year.
 * @param month QuantLib::Month (1..12).
 * @param holidays Set of dates considered bank holidays.
 * @return QuantLib::Date of the (possibly adjusted) 3rd Friday.
 *
 * @note Weekend handling: if the unadjusted 3rd Friday is a normal business day
 *       (i.e., not in @p holidays), it is returned as-is. Adjustment only
 *       occurs when the 3rd Friday is present in @p holidays.
 */
QuantLib::Date getThirdFriday(int year, QuantLib::Month month, const std::set<QuantLib::Date>& holidays);

