# include "simulation/path_simulator.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <iostream>

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
    m_dts=dt;
}

void SimulationEngine::run() {
    size_t stepsPlusOne = m_numSteps + 1;
    m_paths1D.resize(m_numPaths * stepsPlusOne, 0.0);

    for (size_t i = 0; i < m_numPaths; ++i) {
        double current = m_spot;
        m_paths1D[i * stepsPlusOne + 0] = current;  // 🔥 dodaj t₀

        for (size_t j = 0; j < m_numSteps; ++j) {
            double dt = m_dts[j];
            current = m_scheme->nextStep(current, dt, m_rate, m_vol);
            m_paths1D[i * stepsPlusOne + (j + 1)] = current;  // 📈 przesunięcie +1
        }
    }

    m_numSteps = stepsPlusOne;  // aktualizujemy info o liczbie kroków (dla eksportu)
}

const std::vector<double>& SimulationEngine::getPaths() const {
    return m_paths1D;
}

void SimulationEngine::exportToCSV(const std::string& filepath) const {
    std::ofstream out(filepath);
    if (!out.is_open()) {
        throw std::runtime_error("❌ Cannot open file for writing: " + filepath);
    }

    // 🔠 Nagłówek: t0, t1, ..., tN
    for (size_t j = 0; j < m_numSteps; ++j) {
        out << "t" << j;
        if (j < m_numSteps - 1) out << ",";
    }
    out << "\n";

    // 🧪 Każda ścieżka
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

