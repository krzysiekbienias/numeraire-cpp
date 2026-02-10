// unit_tests/utils/test_date_utils.cpp
#include <gtest/gtest.h>

#include <fstream>
#include <set>
#include <string>

#include <ql/time/date.hpp>


#include "utils/date_utils.hpp"

namespace {
    namespace {
        std::string writeTempHolidaysFile(const std::string& filename,
                                       const std::string& contents) {
            namespace fs = std::filesystem;
            const fs::path dir(TEST_INPUT_DIR);
            fs::create_directories(dir);

            const fs::path path = dir / filename;

            std::ofstream out(path, std::ios::binary);
            if (!out.is_open()) throw std::runtime_error("Cannot write: " + path.string());
            out << contents;
            return path.string();
        }
    } // namespace

    // -----------------------------------------------------------------------------
    // Validation: isValidYYYYMMDD
    // -----------------------------------------------------------------------------
    TEST(DateUtils, IsValidYYYYMMDD_AcceptsValidDates) {
        EXPECT_TRUE(date_utils::isValidYYYYMMDD("2025-01-01"));
        EXPECT_TRUE(date_utils::isValidYYYYMMDD("2024-02-29")); // leap day (2024 is leap year)
        EXPECT_TRUE(date_utils::isValidYYYYMMDD("1999-12-31"));
    }

    TEST(DateUtils, IsValidYYYYMMDD_RejectsBadFormat) {
        EXPECT_FALSE(date_utils::isValidYYYYMMDD(""));
        EXPECT_FALSE(date_utils::isValidYYYYMMDD("2025-1-01"));
        EXPECT_FALSE(date_utils::isValidYYYYMMDD("2025-01-1"));
        EXPECT_FALSE(date_utils::isValidYYYYMMDD("2025/01/01"));
        EXPECT_FALSE(date_utils::isValidYYYYMMDD("01-01-2025"));
        EXPECT_FALSE(date_utils::isValidYYYYMMDD("20250101"));
        EXPECT_FALSE(date_utils::isValidYYYYMMDD("abcd-ef-gh"));
        EXPECT_FALSE(date_utils::isValidYYYYMMDD("2025-01-01 "));
        EXPECT_FALSE(date_utils::isValidYYYYMMDD(" 2025-01-01"));
    }

    TEST(DateUtils, IsValidYYYYMMDD_RejectsNonexistentDates) {
        EXPECT_FALSE(date_utils::isValidYYYYMMDD("2025-02-29")); // not a leap year
        EXPECT_FALSE(date_utils::isValidYYYYMMDD("2025-04-31")); // April has 30 days
        EXPECT_FALSE(date_utils::isValidYYYYMMDD("2025-13-01")); // month out of range
        EXPECT_FALSE(date_utils::isValidYYYYMMDD("2025-00-10")); // month out of range
        EXPECT_FALSE(date_utils::isValidYYYYMMDD("2025-01-00")); // day out of range
    }

    // -----------------------------------------------------------------------------
    // Weekend check
    // -----------------------------------------------------------------------------
    TEST(DateUtils, IsWeekend_DetectsSaturdayAndSunday) {
        // 2025-02-01 is Saturday, 2025-02-02 is Sunday, 2025-02-03 is Monday
        const QuantLib::Date sat(1, QuantLib::February, 2025);
        const QuantLib::Date sun(2, QuantLib::February, 2025);
        const QuantLib::Date mon(3, QuantLib::February, 2025);

        EXPECT_TRUE(date_utils::isWeekend(sat));
        EXPECT_TRUE(date_utils::isWeekend(sun));
        EXPECT_FALSE(date_utils::isWeekend(mon));
    }

    // -----------------------------------------------------------------------------
    // Conversions: toQLDateYYYYMMDD <-> toYYYYMMDD
    // -----------------------------------------------------------------------------
    TEST(DateUtils, ToQLDateYYYYMMDD_RoundTripsWithToYYYYMMDD) {
        const std::string iso = "2025-08-15";
        const QuantLib::Date d = date_utils::toQLDateYYYYMMDD(iso);

        EXPECT_EQ(date_utils::toYYYYMMDD(d), iso);
    }

    TEST(DateUtils, ToYYYYMMDD_StringView_HandlesDDMMYYYYInput) {
        // This test assumes your overload converts "dd-mm-yyyy" -> "yyyy-mm-dd"
        // Adjust if your behavior is different.
        EXPECT_EQ(date_utils::toYYYYMMDD(std::string_view("17-01-2025")), "2025-01-17");
        EXPECT_EQ(date_utils::toYYYYMMDD(std::string_view("01-12-2024")), "2024-12-01");
    }

    TEST(DateUtils, ToQLDateDDMMYYYY_RoundTripsWithToStringDDMMYYYY) {
        const std::string ddmmyyyy = "17-01-2025";
        const QuantLib::Date d = date_utils::toQLDateDDMMYYYY(ddmmyyyy);

        EXPECT_EQ(date_utils::toStringDDMMYYYY(d), ddmmyyyy);
    }

    // -----------------------------------------------------------------------------
    // loadBankHolidaysISO
    // -----------------------------------------------------------------------------
    TEST(DateUtils, LoadBankHolidaysISO_ParsesValidLinesAndSkipsCommentsAndEmpty) {
        const std::string path = writeTempHolidaysFile(
            "holidays_test_1.txt",
            R"(# comment line
2025-01-01

2025-12-25
# another comment
2025-05-01
)");

        const std::set<QuantLib::Date> hol = date_utils::loadBankHolidaysISO(path);

        EXPECT_EQ(hol.size(), 3u);
        EXPECT_TRUE(hol.contains(date_utils::toQLDateYYYYMMDD("2025-01-01")));
        EXPECT_TRUE(hol.contains(date_utils::toQLDateYYYYMMDD("2025-05-01")));
        EXPECT_TRUE(hol.contains(date_utils::toQLDateYYYYMMDD("2025-12-25")));
    }

    TEST(DateUtils, LoadBankHolidaysISO_SkipsInvalidLines) {
        const std::string path = writeTempHolidaysFile(
            "holidays_test_2.txt",
            R"(2025-01-01
not-a-date
2025-02-30
2025-12-25
)");

        const std::set<QuantLib::Date> hol = date_utils::loadBankHolidaysISO(path);

        // should keep only valid ones
        EXPECT_EQ(hol.size(), 2u);
        EXPECT_TRUE(hol.contains(date_utils::toQLDateYYYYMMDD("2025-01-01")));
        EXPECT_TRUE(hol.contains(date_utils::toQLDateYYYYMMDD("2025-12-25")));
    }

    TEST(DateUtils, LoadBankHolidaysISO_DeduplicatesDates) {
        const std::string path = writeTempHolidaysFile(
            "holidays_test_3.txt",
            R"(2025-01-01
2025-01-01
2025-01-01
)");

        const std::set<QuantLib::Date> hol = date_utils::loadBankHolidaysISO(path);
        EXPECT_EQ(hol.size(), 1u);
        EXPECT_TRUE(hol.contains(date_utils::toQLDateYYYYMMDD("2025-01-01")));
    }

    // -----------------------------------------------------------------------------
    // getThirdFriday
    // -----------------------------------------------------------------------------
    TEST(DateUtils, GetThirdFriday_ReturnsThirdFridayWhenNotHoliday) {
        // March 2025: Fridays are 7, 14, 21, 28 -> third Friday is 21
        std::set<QuantLib::Date> holidays;

        const QuantLib::Date third = getThirdFriday(2025, QuantLib::March, holidays);

        EXPECT_EQ(third, QuantLib::Date(21, QuantLib::March, 2025));
        EXPECT_EQ(third.weekday(), QuantLib::Friday);
    }

    TEST(DateUtils, GetThirdFriday_MovesBackwardIfThirdFridayIsHoliday) {
        // March 2025 third Friday is 21st; mark it as holiday => should move back to 20th
        std::set<QuantLib::Date> holidays;
        holidays.insert(QuantLib::Date(21, QuantLib::March, 2025));

        const QuantLib::Date adjusted = getThirdFriday(2025, QuantLib::March, holidays);

        EXPECT_EQ(adjusted, QuantLib::Date(20, QuantLib::March, 2025));
        EXPECT_NE(adjusted.weekday(), QuantLib::Friday); // should now be Thursday
    }

    TEST(DateUtils, GetThirdFriday_MovesBackMultipleDaysIfConsecutiveHolidays) {
        // If 21st, 20th are holidays => move to 19th
        std::set<QuantLib::Date> holidays;
        holidays.insert(QuantLib::Date(21, QuantLib::March, 2025));
        holidays.insert(QuantLib::Date(20, QuantLib::March, 2025));

        const QuantLib::Date adjusted = getThirdFriday(2025, QuantLib::March, holidays);

        EXPECT_EQ(adjusted, QuantLib::Date(19, QuantLib::March, 2025));
    }

    TEST(DateUtils, GetThirdFriday_DoesNotAdjustIfThirdFridayNotHolidayEvenIfWeekendInHolidaysSet) {
        // Ensure only the third Friday being holiday triggers adjustment.
        // Add some unrelated holiday; should not affect.
        std::set<QuantLib::Date> holidays;
        holidays.insert(QuantLib::Date(17, QuantLib::March, 2025)); // Monday

        const QuantLib::Date third = getThirdFriday(2025, QuantLib::March, holidays);

        EXPECT_EQ(third, QuantLib::Date(21, QuantLib::March, 2025));
        EXPECT_EQ(third.weekday(), QuantLib::Friday);
    }
}

