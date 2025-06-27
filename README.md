# 💹 Numeraire++

**Numeraire++** is a modern C++ pricing engine designed for fast and modular valuation of financial derivatives — starting with equity options. The architecture is built with extensibility, performance, and clean abstraction in mind, making it suitable for both backtesting and production-level applications.

## ✨ Key Highlights

- 🔧 Modular design using abstract factories and product-pricer separation
- 📅 Robust schedule generation with QuantLib-compatible conventions
- 💰 Support for vanilla and digital option pricing using the Black-Scholes model
- 🧠 Historical and real-time market data integration via Polygon.io
- 📦 Efficient caching of market environments for repeatable simulations
- 🧪 Unit-tested core components and pricing logic using Google Test

## 🗂️ Project Structure (high-level)

## 🚀 Getting Started

The project uses CMake for build automation and supports both Xcode and Ninja builds. To fetch market data and price a portfolio:

```bash
./run_cache_market_data.sh 17-01-2025     # Fetch and cache market data
./bin/portfolio_pricing                   # Run pricing using cached data

🧩 Next Steps

We plan to expand the framework to support:
    •    Multi-asset support (rates, FX, crypto)
    •    Heston and Monte Carlo models
    •    Advanced risk metrics and sensitivities
    •    Interactive Python bindings for research

Stay tuned, or contribute to help push it further 🚀
