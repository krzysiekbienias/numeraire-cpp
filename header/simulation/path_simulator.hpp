# pragma once
#include<vector>
#include "discretization_scheme.hpp"

class SimulationEngine {
public:
    SimulationEngine(
        size_t numPaths,
        const std::vector<double>& dt,
                        
        double spot,
        double drift,
        double volatility,
        std::shared_ptr<IDiscretizationScheme> scheme,
        std::vector<std::string> dates
                     
    );

    void run();
    const std::vector<double>& getPaths() const;
    
    void setDates(const std::vector<std::string> & dates);
    void setTimeSteps(const std::vector<double>& dt); // jeśli chcesz zostawić
    void exportToCSV(const std::string& filepath) const;
    void exportToJson(const std::string& filepath) const;

private:
    size_t m_numPaths;
    size_t m_numSteps;
    double m_spot;
    double m_rate;
    double m_vol;
    std::shared_ptr<IDiscretizationScheme> m_scheme;
    std::vector<double> m_paths1D;

    std::vector<double> m_dts;  // ⬅️ tutaj dodaj
    std::vector<std::string> m_dates;
};
