#include "utils/enum_mappers.hpp"
#include "enums/schedule_enums.hpp"
#include "utils/logger.hpp"


namespace enum_utils {

CalendarType toCalendarType(const std::string& str) {
    auto log=Logger::get();
    if (str == "UK") return CalendarType::UK;
    if (str == "US") return CalendarType::US;
    if (str == "TARGET") return CalendarType::TARGET;
    log->error("Invalid calendar type string: '{}'",str);
    }

std::string toString(CalendarType type) {
    switch (type) {
        case CalendarType::UK: return "UK";
        case CalendarType::US: return "US";
        case CalendarType::TARGET: return "TARGET";
    }
    return "Unknown";
}


// Frequency
Frequency toFrequency(const std::string& str) {
    auto log=Logger::get();
    if (str == "monthly") return Frequency::Monthly;
    if (str == "quarterly") return Frequency::Quarterly;
    if (str == "annually") return Frequency::Annually;
    if (str=="once"||str=="Once") return Frequency::Once;
    if (str=="daily"||str=="Daily") return Frequency::Daily;
    log->error("Invalid frequency type: '{}'",str);
    throw std::invalid_argument("Invalid Frequency: " + str);
}

std::string toString(Frequency type) {
    switch (type) {
        case Frequency::Monthly: return "monthly";
        case Frequency::Quarterly: return "quarterly";
        case Frequency::Annually: return "annually";
        case Frequency::Once: return "once";
        case Frequency::Daily: return "daily";
            
    }
    return "Unknown";
}

// DateConvention
DateConvention toDateConvention(const std::string& str) {
    if (str == "Following") return DateConvention::Following;
    if (str == "ModifiedFollowing") return DateConvention::ModifiedFollowing;
    if (str == "Preceding") return DateConvention::Preceding;
    throw std::invalid_argument("Invalid DateConvention: " + str);
}

std::string toString(DateConvention type) {
    switch (type) {
        case DateConvention::Following: return "Following";
        case DateConvention::ModifiedFollowing: return "ModifiedFollowing";
        case DateConvention::Preceding: return "Preceding";
    }
    return "Unknown";
}

// DateGenerationRule
DateGenerationRule toDateGenerationRule(const std::string& str) {
    if (str == "Forward") return DateGenerationRule::Forward;
    if (str == "Backward") return DateGenerationRule::Backward;
    throw std::invalid_argument("Invalid DateGenerationRule: " + str);
}

std::string toString(DateGenerationRule type) {
    switch (type) {
        case DateGenerationRule::Forward: return "Forward";
        case DateGenerationRule::Backward: return "Backward";
    }
    return "Unknown";
}

// DayCount
DayCount toDayCount(const std::string& str) {
    if (str == "Actual365") return DayCount::Actual365;
    if (str == "Actual360") return DayCount::Actual360;
    if (str == "30/360") return DayCount::Thirty360;
    throw std::invalid_argument("Invalid DayCount: " + str);
}

std::string toString(DayCount type) {
    switch (type) {
        case DayCount::Actual365: return "Actual365";
        case DayCount::Actual360: return "Actual360";
        case DayCount::Thirty360: return "30/360";
    }
    return "Unknown";
}

// Repeat for other enums...

}
