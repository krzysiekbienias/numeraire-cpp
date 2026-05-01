#include <string>
#include "core/schedule_generator.hpp"


double forwardContract( double spotPrice,
                        double riskFreeRate,
                        const ScheduleGenerator& schedule,
                        const std::string& startDate,
                        const std::string& terminationDate) {

    double yearFraction = schedule.computeYearFraction(startDate, terminationDate);
    return spotPrice * exp(riskFreeRate * yearFraction);
}
