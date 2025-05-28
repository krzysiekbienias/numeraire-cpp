#include<string>
#include<ql/time/date.hpp>
#include"utils/logger.hpp"


namespace date_utils {

QuantLib::Date toQLDateDDMMYYYY (const std::string& dateStr){
    int d, m, y;
    char sep1, sep2;

    std::istringstream iss(dateStr);
    if (!(iss >> d >> sep1 >> m >> sep2 >> y) || sep1 != '-' || sep2 != '-') {
        Logger::get()->error("❌ Invalid date format. Expected format: dd-mm-yyyy → Got: " + dateStr);
        throw std::invalid_argument("❌ Invalid date format. Expected format: dd-mm-yyyy → Got: " + dateStr);
    }

    // Basic sanity check
    if (d < 1 || d > 31 || m < 1 || m > 12 || y < 1900 || y > 2100) {
        std::ostringstream oss;
        Logger::get()->error("❌ Date components out of range (dd-mm-yyyy): " + dateStr);
        oss << "❌ Date components out of range (dd-mm-yyyy): " << dateStr;
        throw std::invalid_argument(oss.str());
    }

    return QuantLib::Date(d, QuantLib::Month(m), y);
}

}






