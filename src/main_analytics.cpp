#include <iostream>
#include <string>

#include <core/schedule_generator.hpp>
#include "utils/json_utils.hpp"
#include "utils/path_utils.hpp"
#include "cli/analytics_args.hpp"
#include "pricing/analytics/forward_contract.hpp"


int main(int argc, char *argv[]) {
    analytics_cli::Args args;
    try {
        args = analytics_cli::parse(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << "❌ " << e.what() << std::endl;
        analytics_cli::printGeneralHelp();
        return 1;
    }

    if (args.product.empty() || (args.showHelp && args.product.empty())) {
        analytics_cli::printGeneralHelp();
        return 0;
    }

    if (args.showHelp) {
        analytics_cli::printProductHelp(args.product);
        return 0;
    }

    std::string projectPath = resolveProjectPath(argv[0]);
    std::string configPath  = resolveMainConfigPath(argv[0]);

    JsonUtils::load("MainConfig", configPath);
    auto mainPaths = JsonUtils::toStringMap("MainConfig");
    JsonUtils::load("DefaultSchedule", projectPath + mainPaths.at("SCHEDULE_CONFIG"));

    ScheduleGenerator sc;
    sc.configureFromMap(JsonUtils::toStringMap("DefaultSchedule"));

    try {
        if (args.product == "forward_contract") {
            double spot  = analytics_cli::requireDouble(args, "spot");
            double rate  = analytics_cli::requireDouble(args, "rate");
            std::string start = analytics_cli::require(args, "start");
            std::string end   = analytics_cli::require(args, "end");

            double result = forwardContract(spot, rate, sc, start, end);
            std::cout << "Forward price: " << result << std::endl;

        } else {
            std::cerr << "❌ Unknown product: " << args.product << std::endl;
            analytics_cli::printGeneralHelp();
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "❌ " << e.what() << std::endl;
        analytics_cli::printProductHelp(args.product);
        return 1;
    }
}
