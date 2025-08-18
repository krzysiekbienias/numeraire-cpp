#pragma once
#include<ql/time/date.hpp>
#include<string>

namespace date_utils {

//Convertion
QuantLib::Date toQLDateDDMMYYYY (const std::string& dateStr);
QuantLib::Date toQLDateYYYYMMDD(const std::string& dateStr);
std::string toStringDDMMYYYY(const QuantLib::Date& date);
std::string toStringYYYYMMDD(const QuantLib::Date& date);
std::string toStringYYMMDD(const QuantLib::Date& date);

//Extracting
}
QuantLib::Date getThirdFriday(int year, QuantLib::Month month, const std::set<QuantLib::Date>& holidays);

