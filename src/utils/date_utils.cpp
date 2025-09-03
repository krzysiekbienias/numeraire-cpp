#include<string>
#include<ql/time/date.hpp>
#include"utils/logger.hpp"
#include <iomanip>
#include <sstream>
#include <set>
#include <fstream>


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


std::string toYYYYMMDD(const QuantLib::Date& date) {
    std::ostringstream oss;
    oss << date.year() << "-";
    oss << std::setw(2) << std::setfill('0') << static_cast<int>(date.month()) << "-";
    oss << std::setw(2) << std::setfill('0') << date.dayOfMonth();
    return oss.str();
}

static inline bool isdigit_uc(char c) {
    return std::isdigit(static_cast<unsigned char>(c));
}



std::string toYYYYMMDD(std::string_view s) {
// Jeśli już ISO: "YYYY-MM-DD" -> zwróć jak jest
if (s.size() == 10 && s[4] == '-' && s[7] == '-' &&
    isdigit_uc(s[0]) && isdigit_uc(s[1]) && isdigit_uc(s[2]) && isdigit_uc(s[3]) &&
    isdigit_uc(s[5]) && isdigit_uc(s[6]) && isdigit_uc(s[8]) && isdigit_uc(s[9])) {
    return std::string{s};
    }

// Oczekiwany input: "DD-MM-YYYY" -> konwersja na ISO
if (s.size() == 10 && s[2] == '-' && s[5] == '-' &&
    isdigit_uc(s[0]) && isdigit_uc(s[1]) &&
    isdigit_uc(s[3]) && isdigit_uc(s[4]) &&
    isdigit_uc(s[6]) && isdigit_uc(s[7]) && isdigit_uc(s[8]) && isdigit_uc(s[9])) {
    // "DD-MM-YYYY" -> "YYYY-MM-DD"
    return std::string(s.substr(6,4)) + "-" +
           std::string(s.substr(3,2)) + "-" +
           std::string(s.substr(0,2));
    }

// Nieznany format -> zwróć bez zmian (albo tu dodaj log, jeśli chcesz)
return std::string{s};
}

bool isValidYYYYMMDD(const std::string& s) {
    // Szybki check formatu
    if (s.size() != 10 || s[4] != '-' || s[7] != '-') return false;
    for (int i : {0,1,2,3,5,6,8,9}) if (s[i] < '0' || s[i] > '9') return false;

    // Parsuj i zrób round-trip przez QuantLib,
    // korzystając z istniejącego toYYYYMMDD (bez żadnych pomocników)
    try {
        const auto ql = toQLDateYYYYMMDD(s);     // masz tę funkcję
        return toYYYYMMDD(ql) == s;        // masz tę funkcję
    } catch (...) {
        return false;
    }
}


namespace {
std::string trim(const std::string& s) {
    size_t a = 0, b = s.size();
    while (a < b && std::isspace(static_cast<unsigned char>(s[a]))) ++a;
    while (b > a && std::isspace(static_cast<unsigned char>(s[b-1]))) --b;
    return s.substr(a, b - a);
}
}



std::set<QuantLib::Date> loadBankHolidaysISO(const std::string& holidaysPath) {
    std::set<QuantLib::Date> out;

    std::ifstream in(holidaysPath);
    if (!in) {
        Logger::get()->warn("⚠️ Cannot open holidays CSV: {}", holidaysPath);
        return out; // pusty zbiór = brak dodatkowych świąt
    }

    std::string line;
    size_t lineno = 0;
    while (std::getline(in, line)) {
        ++lineno;
        auto t = trim(line);
        if (t.empty() || t[0] == '#') continue;

        if (!isValidYYYYMMDD(t)) {
            Logger::get()->warn("⚠️ Invalid date in holidays CSV at line {}: '{}'", lineno, t);
            continue;
        }
        // parse ISO -> QL
        int y = std::stoi(t.substr(0,4));
        int m = std::stoi(t.substr(5,2));
        int d = std::stoi(t.substr(8,2));
        out.emplace(d, static_cast<QuantLib::Month>(m), y);
    }

    Logger::get()->info("📅 Loaded {} bank holidays from {}", out.size(), holidaysPath);
    return out;
}


bool isWeekend(const QuantLib::Date& d) {
    using namespace QuantLib;
    Weekday wd = d.weekday();
    return (wd == Saturday || wd == Sunday);
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




