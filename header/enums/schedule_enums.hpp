// schedule_enums.hpp
#pragma once

/// @file schedule_enums.hpp
/// @brief Defines strongly typed enums for schedule generation configuration.
/// These enums represent configuration options such as calendar types,
/// date adjustment conventions, generation rules, frequencies, and day count conventions.



/// @brief Enum for supported financial calendars
enum class CalendarType {
    UK,         ///< United Kingdom calendar
    US,         ///< United States calendar
    TARGET,     ///< TARGET calendar
    Germany     ///< German calendar (example of extensibility)
};

/// @brief Enum for common scheduling frequencies
enum class Frequency {
    
    Monthly,    ///< Once per month
    Quarterly,  ///< Once every 3 months
    Annually,    ///< Once per year
    Once,         ///< Non-recurring schedule, occurs only between start and end dates
    Daily          ///< every day
};

/// @brief Enum for business day adjustment conventions
enum class DateConvention {
    Following,          ///< Move to the next business day
    ModifiedFollowing,  ///< Move to the next business day unless it crosses month end
    Preceding           ///< Move to the previous business day
};

/// @brief Enum for generation rules used by QuantLib's Schedule
enum class DateGenerationRule {
    Forward,    ///< Generate dates forward from the start date
    Backward    ///< Generate dates backward from the end date
};

/// @brief Enum for supported day count conventions
enum class DayCount {
    Actual365,  ///< Actual/365 Fixed
    Actual360,  ///< Actual/360
    Thirty360   ///< 30/360
};
