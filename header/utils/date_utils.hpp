#pragma once

namespace date_utils {


QuantLib::Date toQLDateDDMMYYYY (const std::string& dateStr);
std::string toStringDDMMYYYY(const QuantLib::Date& date);
}

