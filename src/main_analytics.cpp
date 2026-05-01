#include <string>

#include <core/schedule_generator.hpp>
#include "iostream"
#include "utils/json_utils.hpp"
#include "cli/args_parser.hpp"
#include "pricing/analytics/forward_contract.hpp"
#include "utils/path_utils.hpp"


int main(int argc, char *argv[]) {
    // =========================================================================
    // 🔧 Resolve project/config paths from argv[0]
    // =========================================================================

    std::string projectPath = resolveProjectPath(argv[0]);
    std::string configPath = resolveMainConfigPath(argv[0]);

    // =========================================================================
    // 📦 Load configuration (main + secrets)
    // =========================================================================
    JsonUtils::load("MainConfig", configPath);
    auto mainPaths = JsonUtils::toStringMap("MainConfig");
    JsonUtils::load("DefaultSchedule", projectPath+mainPaths.at("SCHEDULE_CONFIG"));

    auto scheduleConfigMap = JsonUtils::toStringMap("DefaultSchedule");
    ScheduleGenerator sc;
    sc.configureFromMap(scheduleConfigMap);

    double forwardPrice = forwardContract(30, 0.05, sc, "02-03-2025", "02-07-2025");
    std::cout << "Forward price: " << forwardPrice << std::endl;






}
