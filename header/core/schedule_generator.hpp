#pragma once

#include <ql/time/schedule.hpp>
#include <ql/time/calendar.hpp>
#include <ql/time/businessdayconvention.hpp>
#include <ql/time/daycounter.hpp>
#include <string>
#include <vector>

#include "enums/schedule_enums.hpp"

class ScheduleGenerator {
public:
    ScheduleGenerator();

    void setCalendar(CalendarType calendar);
    void setFrequency(Frequency frequency);
    void setDateCorrectionRule(DateConvention convention);
    void setDateGenerationRule(DateGenerationRule rule);
    void setDayCounter(DayCount dayCounter);
    
    
    
    
    
    void configureFromMap(const std::unordered_map<std::string, std::string>& cfg);

    /// Generates a QuantLib::Schedule between two dates.
    /// @param startDate  Start date in DD-MM-YYYY format, e.g. "01-01-2024"
    /// @param endDate    End date in DD-MM-YYYY format,   e.g. "31-12-2024"
    QuantLib::Schedule generate(const std::string& startDate, const std::string& endDate) const;
    
    
    //getters
    CalendarType getCalendarType() const;
    Frequency getFrequency() const;
    DateConvention getDateConvention() const;
    DateGenerationRule getDateRule() const;
    
    QuantLib::DayCounter getDayCounter() const;
    DayCount getDayCountEnum() const;

    // Year fraction calculations
    double computeYearFraction(const std::string& startDate, const std::string& endDate) const;
    std::vector<double> computeYearFractionVector(const QuantLib::Schedule& schedule) const;

    // Introspection / behavior helpers (test-friendly)
    QuantLib::Date adjust(const QuantLib::Date& d) const;
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
