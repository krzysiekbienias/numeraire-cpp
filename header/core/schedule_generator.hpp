#pragma once

#include <ql/time/schedule.hpp>
#include <ql/time/calendar.hpp>
#include <ql/time/businessdayconvention.hpp>
#include <ql/time/daycounter.hpp>
#include <string>
#include <vector>

#include "enums/schedule_enums.hpp"

/// Builds QuantLib payment schedules and computes year fractions for
/// fixed-income and derivatives pricing.
///
/// Defaults (set in the constructor): UK calendar, Monthly frequency,
/// Following business-day convention, Forward date generation, Actual/365.
/// All settings can be overridden individually via setters or in bulk via
/// configureFromMap().
class ScheduleGenerator {
public:
    /// Constructs a ScheduleGenerator with sensible defaults:
    /// UK calendar, Monthly frequency, Following convention,
    /// Forward date generation and Actual/365 day counter.
    ScheduleGenerator();

    /// Sets the holiday calendar used for business-day adjustments.
    /// Supported values: UK, US (NYSE), TARGET, Germany.
    void setCalendar(CalendarType calendar);

    /// Sets the coupon / payment frequency (Daily, Monthly, Quarterly,
    /// Annually, Once).
    void setFrequency(Frequency frequency);

    /// Sets the business-day convention applied when a scheduled date falls
    /// on a non-business day (Following, ModifiedFollowing, Preceding).
    void setDateCorrectionRule(DateConvention convention);

    /// Sets the direction in which schedule dates are generated
    /// (Forward from start date or Backward from end date).
    void setDateGenerationRule(DateGenerationRule rule);

    /// Sets the day-count convention used for year-fraction calculations
    /// (Actual/365, Actual/360, 30/360).
    void setDayCounter(DayCount dayCounter);

    /// Configures all schedule parameters at once from a string map.
    /// Expected keys: "CALENDAR", "FREQUENCY", "BUSINESS_DAY_CONVENTION",
    /// "DATE_GENERATION_RULE", "DAY_COUNT".
    /// Throws std::out_of_range if a required key is missing.
    void configureFromMap(const std::unordered_map<std::string, std::string>& cfg);

    /// Generates a QuantLib::Schedule between two dates.
    /// @param startDate  Start date in DD-MM-YYYY format, e.g. "01-01-2024"
    /// @param endDate    End date in DD-MM-YYYY format,   e.g. "31-12-2024"
    /// @return           QuantLib::Schedule with dates adjusted according to
    ///                   the configured calendar, frequency and convention.
    QuantLib::Schedule generate(const std::string& startDate, const std::string& endDate) const;

    /// Returns the currently configured calendar type enum.
    CalendarType getCalendarType() const;

    /// Returns the currently configured frequency enum.
    Frequency getFrequency() const;

    /// Returns the currently configured business-day convention enum.
    DateConvention getDateConvention() const;

    /// Returns the currently configured date-generation rule enum.
    DateGenerationRule getDateRule() const;

    /// Returns the underlying QuantLib day-counter object.
    QuantLib::DayCounter getDayCounter() const;

    /// Returns the currently configured day-count enum.
    DayCount getDayCountEnum() const;

    /// Computes the year fraction between two dates using the configured
    /// day-count convention.
    /// @param startDate  Start date in DD-MM-YYYY format.
    /// @param endDate    End date in DD-MM-YYYY format.
    /// @return           Year fraction as a double (e.g. 0.5 for ~6 months).
    double computeYearFraction(const std::string& startDate, const std::string& endDate) const;

    /// Computes a vector of consecutive year fractions for all periods in a
    /// schedule. The returned vector has schedule.size() - 1 elements.
    /// @param schedule  A previously generated QuantLib::Schedule.
    /// @return          Vector of year fractions, one per period.
    std::vector<double> computeYearFractionVector(const QuantLib::Schedule& schedule) const;

    /// Adjusts a date to the nearest business day using the configured
    /// calendar and business-day convention.
    /// @param d  Input QuantLib date.
    /// @return   Adjusted QuantLib date.
    QuantLib::Date adjust(const QuantLib::Date& d) const;

    /// Returns true if the given date is a business day in the configured
    /// calendar.
    bool isBusinessDay(const QuantLib::Date& d) const;

private:
    CalendarType m_calendarType;
    Frequency m_frequencyEnum;
    DateConvention m_dateConvention;
    DateGenerationRule m_dateRule;
    DayCount m_dayCountEnum;

    QuantLib::Calendar m_calendar;
    QuantLib::Frequency m_frequency;
    QuantLib::BusinessDayConvention m_convention;
    QuantLib::DateGeneration::Rule m_rule;
    QuantLib::DayCounter m_dayCounter;
};
