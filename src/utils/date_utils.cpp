#include<string>
#include<ql/time/date.hpp>
#include"utils/logger.hpp"
#include <iomanip>
#include <sstream>


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

std::string toStringYYYYMMDD(const QuantLib::Date& date) {
    std::ostringstream oss;
    oss << date.year() << "-";
    oss << std::setw(2) << std::setfill('0') << static_cast<int>(date.month()) << "-";
    oss << std::setw(2) << std::setfill('0') << date.dayOfMonth();
    return oss.str();
}

}






