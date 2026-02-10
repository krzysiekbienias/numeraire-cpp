// unit_tests/cli/test_cli_parser.cpp
#include <gtest/gtest.h>
#include <string>
#include <vector>

#include "cli/args_parser.hpp"

// Helper: parse tokens exactly like production does after argv -> vector<string> capture.
// IMPORTANT: this assumes the input does NOT include program name.
static cli::ParseResult parseTokens(const std::vector<std::string>& tokens) {
    return cli::parse(tokens);
}

TEST(CliArgsParser, ParsesSingleDateAndSpotPhase) {
    auto res = parseTokens({"--date", "2025-11-07", "--phase", "spot"});

    ASSERT_TRUE(res.error.empty()) << res.error;
    ASSERT_TRUE(res.args.has_value());

    const auto& parsed = *res.args;
    EXPECT_EQ(parsed.dateISO, "2025-11-07");
    EXPECT_TRUE(parsed.startISO.empty());
    EXPECT_TRUE(parsed.endISO.empty());
    EXPECT_FALSE(parsed.showHelp);

    EXPECT_TRUE(cli::has(parsed.phases, cli::Phase::Spot));
    EXPECT_FALSE(cli::has(parsed.phases, cli::Phase::OptionSymbols));
    EXPECT_FALSE(cli::has(parsed.phases, cli::Phase::OptionValues));
}

TEST(CliArgsParser, ParsesRangeAndSpotPhase) {
    auto res = parseTokens({"--start", "2025-11-08", "--end", "2025-11-20", "--phase", "spot"});

    ASSERT_TRUE(res.error.empty()) << res.error;
    ASSERT_TRUE(res.args.has_value());

    const auto& parsed = *res.args;
    EXPECT_TRUE(parsed.dateISO.empty());
    EXPECT_EQ(parsed.startISO, "2025-11-08");
    EXPECT_EQ(parsed.endISO, "2025-11-20");
    EXPECT_FALSE(parsed.showHelp);

    EXPECT_TRUE(cli::has(parsed.phases, cli::Phase::Spot));
}

TEST(CliArgsParser, ParsesOptionSymbolsAlias) {
    auto res = parseTokens({"--date", "2025-11-07", "--phase", "option-symbols"});

    ASSERT_TRUE(res.error.empty()) << res.error;
    ASSERT_TRUE(res.args.has_value());

    const auto& parsed = *res.args;
    EXPECT_TRUE(cli::has(parsed.phases, cli::Phase::OptionSymbols));
    EXPECT_FALSE(cli::has(parsed.phases, cli::Phase::Spot));
    EXPECT_FALSE(cli::has(parsed.phases, cli::Phase::OptionValues));
}

TEST(CliArgsParser, ParsesOptionValuesAlias) {
    auto res = parseTokens({"--date", "2025-11-07", "--phase", "option-values"});

    ASSERT_TRUE(res.error.empty()) << res.error;
    ASSERT_TRUE(res.args.has_value());

    const auto& parsed = *res.args;
    EXPECT_TRUE(cli::has(parsed.phases, cli::Phase::OptionValues));
    EXPECT_FALSE(cli::has(parsed.phases, cli::Phase::Spot));
    EXPECT_FALSE(cli::has(parsed.phases, cli::Phase::OptionSymbols));
}

TEST(CliArgsParser, ParsesAllPhase) {
    auto res = parseTokens({"--date", "2025-11-07", "--phase", "all"});

    ASSERT_TRUE(res.error.empty()) << res.error;
    ASSERT_TRUE(res.args.has_value());

    const auto& parsed = *res.args;
    EXPECT_TRUE(cli::has(parsed.phases, cli::Phase::Spot));
    EXPECT_TRUE(cli::has(parsed.phases, cli::Phase::OptionSymbols));
    EXPECT_TRUE(cli::has(parsed.phases, cli::Phase::OptionValues));
}

TEST(CliArgsParser, HelpFlagSetsShowHelp) {
    auto res = parseTokens({"--help"});

    ASSERT_TRUE(res.error.empty()) << res.error;
    ASSERT_TRUE(res.args.has_value());

    const auto& parsed = *res.args;
    EXPECT_TRUE(parsed.showHelp);
}

TEST(CliArgsParser, UnknownPhaseGivesError) {
    auto res = parseTokens({"--date", "2025-11-07", "--phase", "banana"});

    EXPECT_FALSE(res.error.empty());
    EXPECT_FALSE(res.args.has_value());
}

TEST(CliArgsParser, MissingDateValueGivesError) {
    auto res = parseTokens({"--date", "--phase", "spot"});

    EXPECT_FALSE(res.error.empty());
    EXPECT_FALSE(res.args.has_value());
}

TEST(CliArgsParser, MissingRangeEndGivesError) {
    auto res = parseTokens({"--start", "2025-11-08", "--phase", "spot"});

    EXPECT_FALSE(res.error.empty());
    EXPECT_FALSE(res.args.has_value());
}

TEST(CliArgsParser, UnknownFlagGivesError) {
    auto res = parseTokens({"--date", "2025-11-07", "--phaze", "spot"});

    EXPECT_FALSE(res.error.empty());
    EXPECT_FALSE(res.args.has_value());
}
