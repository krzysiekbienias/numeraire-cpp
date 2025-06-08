#pragma once

#include <ql/time/schedule.hpp>
#include <ql/time/calendar.hpp>
#include <ql/time/businessdayconvention.hpp>
#include <ql/time/daycounter.hpp>
#include <string>

#include "enums/schedule_enums.hpp"

class ScheduleGenerator {
public:
    ScheduleGenerator();

    // Public API: string-based
    void setCalendar(const std::string& calendar);
    void setFrequency(const std::string& frequency);
    void setDateCorrectionRule(const std::string& convention);
    void setDateGenerationRule(const std::string& rule);
    void setDayCounter(const std::string& dayCounter);

    // Enum-based setters
    void setCalendar(CalendarType calendar);
    void setFrequency(Frequency frequency);
    void setDateCorrectionRule(DateConvention convention);
    void setDateGenerationRule(DateGenerationRule rule);
    void setDayCounter(DayCount dayCounter);
    
    
    
    
    
    void configureFromMap(const std::unordered_map<std::string, std::string>& cfg);

    QuantLib::Schedule generate(const std::string& startDate, const std::string& endDate) const;
    
    
    //geters
    CalendarType getCalendarType() const;
    Frequency getFrequency() const;
    DateConvention getDateConvention() const;
    DateGenerationRule getDateRule() const;
    
    QuantLib::DayCounter getDayCounter() const;
    DayCount getDayCountEnum() const;
    
    

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
