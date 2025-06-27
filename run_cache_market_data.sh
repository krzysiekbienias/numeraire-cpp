#!/bin/bash

# === Market Data Caching Launcher ===

# Sprawdź czy podano datę
if [ -z "$1" ]; then
  echo "❌ Please provide a valuation date in format: DD-MM-YYYY"
  echo "Usage: ./run_cache_market_data.sh <DD-MM-YYYY>"
  exit 1
fi

DATE="$1"
echo "📦 Caching market data for valuation date: $DATE"

# Ścieżka do binarki (Debug lub Release, w zależności od budowy w Xcode/CMake)
EXECUTABLE="./bin/cache_market_data"

# Sprawdź czy binarka istnieje
if [ ! -f "$EXECUTABLE" ]; then
  echo "❌ Executable not found: $EXECUTABLE"
  echo "💡 Make sure you built the project via Xcode or CMake and the target 'cache_market_data' exists."
  exit 1
fi

# Uruchomienie binarki z podaną datą jako argument
"$EXECUTABLE" "$DATE"
