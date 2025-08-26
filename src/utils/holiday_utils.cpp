#include "utils/holiday_utils.hpp"
#include <set>
#include <string>
#include <ql/time/date.hpp>
#include <fstream>
#include <sstream>
#include "utils/date_utils.hpp"
#include "utils/logger.hpp"


std::set<QuantLib::Date> loadHolidaysFromCsv(const std::string& csvPath){
    std::set<QuantLib::Date> holidays;
            std::ifstream file(csvPath);
            std::string line;

            if (!file.is_open()) {
                Logger::get()->error("❌ Could not open holidays file: {}", csvPath);
                return holidays;
            }

            while (std::getline(file, line)) {
                try {
                    QuantLib::Date d = date_utils::toQLDateYYYYMMDD(line);
                    holidays.insert(d);
                } catch (const std::exception& e) {
                    Logger::get()->warn("⚠️ Skipping invalid holiday date: {}", line);
                }
            }

            return holidays;
        }

