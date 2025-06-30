# include "simulation/path_simulator.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

SimulationEngine::SimulationEngine(
    size_t numPaths,
    const std::vector<double>& dt,
    double spot,
    double drift,
    double volatility,
    std::shared_ptr<IDiscretizationScheme> scheme
)
    : m_numPaths(numPaths),
      m_numSteps(dt.size()),
      m_spot(spot),
      m_rate(drift),
      m_vol(volatility),
      m_scheme(std::move(scheme)),
      m_paths1D(numPaths * dt.size(), 0.0)
{
    m_scheme->setTimeSteps(dt);
}

void SimulationEngine::run() {
    for (size_t i = 0; i < m_numPaths; ++i) {
        double current = m_spot;
        for (size_t j = 0; j < m_numSteps; ++j) {
            double next = m_scheme->simulateNextStep(current, j);
            m_paths1D[i * m_numSteps + j] = next;
            current = next;
        }
    }
}

const std::vector<double>& SimulationEngine::getPaths() const {
    return m_paths1D;
}

void SimulationEngine::exportToCSV(const std::string& filepath) const {
    std::ofstream out(filepath);
    for (size_t i = 0; i < m_numPaths; ++i) {
        for (size_t j = 0; j < m_numSteps; ++j) {
            out << m_paths1D[i * m_numSteps + j];
            if (j < m_numSteps - 1) out << ",";
        }
        out << "\n";
    }
}

void SimulationEngine::exportToJson(const std::string& filepath) const {
    nlohmann::json jsonPaths;
    for (size_t i = 0; i < m_numPaths; ++i) {
        std::vector<double> path;
        for (size_t j = 0; j < m_numSteps; ++j) {
            path.push_back(m_paths1D[i * m_numSteps + j]);
        }
        jsonPaths["path_" + std::to_string(i)] = path;
    }
    std::ofstream out(filepath);
    out << jsonPaths.dump(4);
}

