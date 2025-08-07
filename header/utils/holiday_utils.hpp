#pragma once
#include <set>
#include <string>
#include <ql/time/date.hpp>

std::set<QuantLib::Date> loadHolidaysFromCsv(const std::string& csvPath);

