#!/usr/bin/env bash
# fetch_market_data.sh — wrapper for main_cache_market_data binary

BIN="./bin/main_cache_market_data"   # ścieżka do binarki

if [[ ! -x "$BIN" ]]; then
  echo "❌ Binary not found: $BIN"
  echo "   Compile project first or adjust BIN path."
  exit 1
fi

echo "▶ Running Market Data Collector..."
echo "   Command: $BIN $*"
"$BIN" "$@"
