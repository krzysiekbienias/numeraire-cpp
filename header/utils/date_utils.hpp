#pragma once
#include<ql/time/date.hpp>
#include<string>

namespace date_utils {


QuantLib::Date toQLDateDDMMYYYY (const std::string& dateStr);
std::string toStringDDMMYYYY(const QuantLib::Date& date);
std::string toStringYYYYMMDD(const QuantLib::Date& date);
}

