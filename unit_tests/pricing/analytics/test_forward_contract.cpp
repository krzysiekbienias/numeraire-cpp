// unit_tests/pricing/analytics/test_forward_contract.cpp
#include <gtest/gtest.h>
#include <cmath>

#include "core/schedule_generator.hpp"
#include "pricing/analytics/forward_contract.hpp"

namespace {

ScheduleGenerator makeSchedule() {
    ScheduleGenerator sc;
    sc.setCalendar(CalendarType::UK);
    sc.setDayCounter(DayCount::Actual365);
    return sc;
}

// -----------------------------------------------------------------------------
// Basic pricing
// -----------------------------------------------------------------------------
TEST(ForwardContract, KnownValue_OneYear_Actual365) {
    auto sc = makeSchedule();
    // T = 1.0 exactly (non-leap year), F = 100 * exp(0.05 * 1) = 105.1271...
    double result = forwardContract(100.0, 0.05, sc, "01-01-2025", "01-01-2026");
    EXPECT_NEAR(result, 100.0 * std::exp(0.05 * 1.0), 1e-6);
}

TEST(ForwardContract, ZeroRate_ReturnsSpotPrice) {
    auto sc = makeSchedule();
    double result = forwardContract(50.0, 0.0, sc, "01-01-2025", "01-01-2026");
    EXPECT_NEAR(result, 50.0, 1e-10);
}

TEST(ForwardContract, NegativeRate_Discounts) {
    auto sc = makeSchedule();
    // F < S when r < 0
    double result = forwardContract(100.0, -0.05, sc, "01-01-2025", "01-01-2026");
    EXPECT_NEAR(result, 100.0 * std::exp(-0.05 * 1.0), 1e-6);
    EXPECT_LT(result, 100.0);
}

TEST(ForwardContract, ShortTenor_QuarterYear) {
    auto sc = makeSchedule();
    // ~0.25 year (90/365)
    double result = forwardContract(100.0, 0.05, sc, "01-01-2025", "01-04-2025");
    double T = sc.computeYearFraction("01-01-2025", "01-04-2025");
    EXPECT_NEAR(result, 100.0 * std::exp(0.05 * T), 1e-6);
}

// -----------------------------------------------------------------------------
// No static caching — calling twice with different dates gives different results
// -----------------------------------------------------------------------------
TEST(ForwardContract, TwoCallsDifferentDates_GiveDifferentResults) {
    auto sc = makeSchedule();
    double f1 = forwardContract(100.0, 0.05, sc, "01-01-2025", "01-01-2026");
    double f2 = forwardContract(100.0, 0.05, sc, "01-01-2025", "01-01-2028");
    EXPECT_NE(f1, f2);
    EXPECT_GT(f2, f1);
}

// -----------------------------------------------------------------------------
// Spot scaling — F scales linearly with S
// -----------------------------------------------------------------------------
TEST(ForwardContract, SpotScaling_LinearRelationship) {
    auto sc = makeSchedule();
    double f1 = forwardContract(100.0, 0.05, sc, "01-01-2025", "01-01-2026");
    double f2 = forwardContract(200.0, 0.05, sc, "01-01-2025", "01-01-2026");
    EXPECT_NEAR(f2, 2.0 * f1, 1e-10);
}

} // namespace
