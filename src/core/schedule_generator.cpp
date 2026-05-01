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



ScheduleGenerator::ScheduleGenerator() {
    setCalendar(CalendarType::UK);
    setFrequency(Frequency::Monthly);
    setDateCorrectionRule(DateConvention::Following);
    setDateGenerationRule(DateGenerationRule::Forward);
    setDayCounter(DayCount::Actual365);
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
    setCalendar(enum_utils::toCalendarType(cfg.at("CALENDAR")));
    setFrequency(enum_utils::toFrequency(cfg.at("FREQUENCY")));
    setDateCorrectionRule(enum_utils::toDateConvention(cfg.at("BUSINESS_DAY_CONVENTION")));
    setDateGenerationRule(enum_utils::toDateGenerationRule(cfg.at("DATE_GENERATION_RULE")));
    setDayCounter(enum_utils::toDayCount(cfg.at("DAY_COUNT")));
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


double ScheduleGenerator::computeYearFraction(const std::string& startDate, const std::string& endDate) const {
    QuantLib::Date start = date_utils::toQLDateDDMMYYYY(startDate);
    QuantLib::Date end   = date_utils::toQLDateDDMMYYYY(endDate);
    double yf = m_dayCounter.yearFraction(start, end);
    Logger::get()->info("📐 Year fraction from {} to {}: {}", startDate, endDate, yf);
    return yf;
}

std::vector<double> ScheduleGenerator::computeYearFractionVector(const QuantLib::Schedule& schedule) const {
    std::vector<double> dts;
    dts.reserve(schedule.size() > 0 ? schedule.size() - 1 : 0);
    for (size_t i = 0; i + 1 < schedule.size(); ++i) {
        dts.push_back(m_dayCounter.yearFraction(schedule[i], schedule[i + 1]));
    }
    return dts;
}

QuantLib::Date ScheduleGenerator::adjust(const QuantLib::Date& d) const {
    return m_calendar.adjust(d, m_convention);
}

bool ScheduleGenerator::isBusinessDay(const QuantLib::Date& d) const {
    return m_calendar.isBusinessDay(d);
}