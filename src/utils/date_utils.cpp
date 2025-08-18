#include<string>
#include<ql/time/date.hpp>
#include"utils/logger.hpp"
#include <iomanip>
#include <sstream>
#include <set>


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


QuantLib::Date toQLDateYYYYMMDD(const std::string& dateStr) {
    int y, m, d;
    char sep1, sep2;

    std::istringstream iss(dateStr);
    if (!(iss >> y >> sep1 >> m >> sep2 >> d) || sep1 != '-' || sep2 != '-') {
        Logger::get()->error("❌ Invalid date format. Expected format: yyyy-mm-dd → Got: " + dateStr);
        throw std::invalid_argument("Invalid date format: " + dateStr);
    }

    if (d < 1 || d > 31 || m < 1 || m > 12 || y < 1900 || y > 2100) {
        Logger::get()->error("❌ Date components out of range (yyyy-mm-dd): " + dateStr);
        throw std::invalid_argument("Date components out of range: " + dateStr);
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


std::string toStringYYMMDD(const QuantLib::Date& date) {
    std::ostringstream oss;

    int yearYY = date.year() % 100;  // last two digits
    int month = static_cast<int>(date.month());
    int day = date.dayOfMonth();

    oss << std::setw(2) << std::setfill('0') << yearYY
        << std::setw(2) << std::setfill('0') << month
        << std::setw(2) << std::setfill('0') << day;

    return oss.str();  // e.g., "250417"
}


std::string toStringDDMMYYYY(const QuantLib::Date& date) {
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << date.dayOfMonth()
        << "-" << std::setw(2) << std::setfill('0') << static_cast<int>(date.month())
        << "-" << date.year();
    return oss.str();
}

}

// it also makes adjustment if third holiday is a bank holiday.
QuantLib::Date getThirdFriday(int year, QuantLib::Month month, const std::set<QuantLib::Date>& holidays) {
        using namespace QuantLib;
        Date date(1, month, year);

        int fridayCount = 0;
        while (true) {
            if (date.weekday() == Friday) {
                fridayCount++;
                if (fridayCount == 3) {
                    while (holidays.count(date)) {
                        date = date - 1;
                    }
                    return date;
                }
            }
            date = date + 1;
        }
    }




