# include "simulation/path_simulator.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

SimulationEngine::SimulationEngine(
    size_t numPaths,
    size_t numSteps,
    double spot,
    double rate,
    double volatility,
    double maturity,
    std::shared_ptr<IDiscretizationScheme> scheme
)
    : m_numPaths(numPaths),
      m_numSteps(numSteps),
      m_spot(spot),
      m_rate(rate),
      m_vol(volatility),
      m_maturity(maturity),
      m_scheme(std::move(scheme)) //🚀 perf
{
    m_paths1D.resize(m_numPaths * m_numSteps, 0.0);
}

void SimulationEngine::setTimeSteps(const std::vector<double>& dt) {
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

