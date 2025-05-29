#pragma once

#include <string>
#include <unordered_map>
#include <stdexcept>

#include "enums/schedule_enums.hpp"  // Adjust to your actual path



namespace enum_utils {

    // --- CalendarType ---
    CalendarType toCalendarType(const std::string& str);
    std::string toString(CalendarType type);

    // --- Frequency ---
    Frequency toFrequency(const std::string& str);
    std::string toString(Frequency freq);

    // --- DateConvention ---
    DateConvention toDateConvention(const std::string& str);
    std::string toString(DateConvention dc);

    // --- DateGenerationRule ---
    DateGenerationRule toDateGenerationRule(const std::string& str);
    std::string toString(DateGenerationRule rule);

    // --- DayCount ---
    DayCount toDayCount(const std::string& str);
    std::string toString(DayCount dc);

}
