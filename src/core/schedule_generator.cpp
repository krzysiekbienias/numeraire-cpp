#include "core/schedule_generator.hpp"
#include "utils/enum_mappers.hpp"
#include "utils/logger.hpp"
#include "utils/date_utils.hpp"

#include <ql/time/calendars/unitedkingdom.hpp>
#include <ql/time/calendars/unitedstates.hpp>
#include <ql/time/calendars/target.hpp>
#include <ql/time/calendars/germany.hpp>
#include <ql/time/daycounters/actual365fixed.hpp>
#include <ql/time/daycounters/actual360.hpp>
#include <ql/time/daycounters/thirty360.hpp>
#include <stdexcept>
#include <sstream>



ScheduleGenerator::ScheduleGenerator()
    : m_calendarType(CalendarType::UK),
      m_frequencyEnum(Frequency::Monthly),
      m_dateConvention(DateConvention::Following),
      m_dateRule(DateGenerationRule::Forward),
      m_dayCountEnum(DayCount::Actual365) {
    setCalendar(m_calendarType);
    setFrequency(m_frequencyEnum);
    setDateCorrectionRule(m_dateConvention);
    setDateGenerationRule(m_dateRule);
    setDayCounter(m_dayCountEnum);
}

void ScheduleGenerator::setCalendar(const std::string& calendar) {
    setCalendar(enum_utils::toCalendarType(calendar));
}

void ScheduleGenerator::setFrequency(const std::string& frequency) {
    setFrequency(enum_utils::toFrequency(frequency));
}

void ScheduleGenerator::setDateCorrectionRule(const std::string& convention) {
    setDateCorrectionRule(enum_utils::toDateConvention(convention));
}

void ScheduleGenerator::setDateGenerationRule(const std::string& rule) {
    setDateGenerationRule(enum_utils::toDateGenerationRule(rule));
}

void ScheduleGenerator::setDayCounter(const std::string& dayCounter) {
    setDayCounter(enum_utils::toDayCount(dayCounter));
}

void ScheduleGenerator::setCalendar(CalendarType calendar) {
    m_calendarType = calendar;
    switch (calendar) {
        case CalendarType::UK:      m_calendar = QuantLib::UnitedKingdom(); break;
        case CalendarType::US:      m_calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::NYSE); break;
        case CalendarType::TARGET:  m_calendar = QuantLib::TARGET(); break;
        case CalendarType::Germany: m_calendar = QuantLib::Germany(); break;
    }
}

void ScheduleGenerator::setFrequency(Frequency freq) {
    m_frequencyEnum = freq;
    switch (freq) {
        case Frequency::Monthly:   m_frequency = QuantLib::Monthly; break;
        case Frequency::Quarterly: m_frequency = QuantLib::Quarterly; break;
        case Frequency::Annually:  m_frequency = QuantLib::Annual; break;
        case Frequency::Once:  m_frequency = QuantLib::Once; break;
        case Frequency::Daily:  m_frequency = QuantLib::Daily; break;
    }
}

void ScheduleGenerator::setDateCorrectionRule(DateConvention convention) {
    m_dateConvention = convention;
    switch (convention) {
        case DateConvention::Following:         m_convention = QuantLib::Following; break;
        case DateConvention::ModifiedFollowing: m_convention = QuantLib::ModifiedFollowing; break;
        case DateConvention::Preceding:         m_convention = QuantLib::Preceding; break;
    }
}

void ScheduleGenerator::setDateGenerationRule(DateGenerationRule rule) {
    m_dateRule = rule;
    switch (rule) {
        case DateGenerationRule::Forward:  m_rule = QuantLib::DateGeneration::Forward; break;
        case DateGenerationRule::Backward: m_rule = QuantLib::DateGeneration::Backward; break;
    }
}

void ScheduleGenerator::setDayCounter(DayCount dc) {
    m_dayCountEnum = dc;
    switch (dc) {
        case DayCount::Actual365:  m_dayCounter = QuantLib::Actual365Fixed(); break;
        case DayCount::Actual360:  m_dayCounter = QuantLib::Actual360(); break;
        case DayCount::Thirty360:  m_dayCounter = QuantLib::Thirty360(QuantLib::Thirty360::USA); break;
    }
}


void ScheduleGenerator::configureFromMap(const std::unordered_map<std::string, std::string>& cfg) {
    setCalendar(cfg.at("CALENDAR"));
    setFrequency(cfg.at("FREQUENCY"));
    setDateCorrectionRule(cfg.at("BUSINESS_DAY_CONVENTION"));
    setDateGenerationRule(cfg.at("DATE_GENERATION_RULE"));
    setDayCounter(cfg.at("DAY_COUNT"));
}

QuantLib::Schedule ScheduleGenerator::generate(const std::string& start, const std::string& end) const {
    return QuantLib::Schedule(
        date_utils::toQLDateDDMMYYYY(start),
        date_utils::toQLDateDDMMYYYY(end),
        QuantLib::Period(m_frequency),
        m_calendar,
        m_convention,
        m_convention,
        m_rule,
        false
    );
}

//geters

CalendarType ScheduleGenerator::getCalendarType() const {
    return m_calendarType;
}

Frequency ScheduleGenerator::getFrequency() const {
    return m_frequencyEnum;
}

DateConvention ScheduleGenerator::getDateConvention() const {
    return m_dateConvention;
}

DateGenerationRule ScheduleGenerator::getDateRule() const {
    return m_dateRule;
}

QuantLib::DayCounter ScheduleGenerator::getDayCounter() const { return m_dayCounter;}

DayCount ScheduleGenerator::getDayCountEnum() const {
    return m_dayCountEnum;
}
