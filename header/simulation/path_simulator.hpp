# pragma once
#include<vector>
#include "discretization_scheme.hpp"

class SimulationEngine {
public:
    SimulationEngine(
        size_t numPaths,
        const std::vector<double>& dt,
        double spot,
        double drift,           // under P measure
        double volatility,
        std::shared_ptr<IDiscretizationScheme> scheme
    );

    void run();
    const std::vector<double>& getPaths() const;
    void setTimeSteps(const std::vector<double>& dt);

    void exportToCSV(const std::string& filepath) const;
    void exportToJson(const std::string& filepath) const;

private:
    size_t m_numPaths;
    size_t m_numSteps;
    double m_spot;
    double m_rate;
    double m_vol;
    double m_maturity;
    std::shared_ptr<IDiscretizationScheme> m_scheme;
    std::vector<double> m_paths1D; // 1D vector: path_i_step_j = i * numSteps + j
};
