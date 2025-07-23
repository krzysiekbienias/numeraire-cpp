#!/bin/bash

# === Konfiguracja ===
START_DATE="$1"
END_DATE="$2"

if [ -z "$START_DATE" ] || [ -z "$END_DATE" ]; then
  echo "❌ Usage: ./cache_market_data_for_range.sh <start-DD-MM-YYYY> <end-DD-MM-YYYY>"
  exit 1
fi

BIN_PATH="./bin/cache_market_data"
CACHE_DIR="./cached_market_data"
HOLIDAYS_FILE="./holidays.csv"

# 🔃 Funkcja: konwersja daty na timestamp
date_to_epoch() {
  date -j -f "%d-%m-%Y" "$1" "+%s"
}

# 🔃 Funkcja: konwersja timestamp na datę (DD-MM-YYYY)
epoch_to_date_ddmmyyyy() {
  date -j -f "%s" "$1" "+%d-%m-%Y"
}

# 🔃 Funkcja: konwersja timestamp na datę ISO (YYYY-MM-DD)
epoch_to_date_iso() {
  date -j -f "%s" "$1" "+%Y-%m-%d"
}

# 📅 Wczytaj święta z CSV do associative array
declare -A HOLIDAYS
if [ -f "$HOLIDAYS_FILE" ]; then
  while IFS= read -r line || [ -n "$line" ]; do
    trimmed=$(echo "$line" | xargs)
    if [[ "$trimmed" =~ ^[0-9]{4}-[0-9]{2}-[0-9]{2}$ ]]; then
      HOLIDAYS["$trimmed"]=1
    fi
  done < "$HOLIDAYS_FILE"
fi

# 🔁 Iteruj po dniach
START_EPOCH=$(date_to_epoch "$START_DATE")
END_EPOCH=$(date_to_epoch "$END_DATE")
ONE_DAY=86400

CURRENT_EPOCH="$START_EPOCH"
while [ "$CURRENT_EPOCH" -le "$END_EPOCH" ]; do
  CURRENT_DATE=$(epoch_to_date_ddmmyyyy "$CURRENT_EPOCH")
  CURRENT_DATE_ISO=$(epoch_to_date_iso "$CURRENT_EPOCH")
  WEEKDAY=$(date -j -f "%d-%m-%Y" "$CURRENT_DATE" "+%u") # 1=Mon, 7=Sun

  CACHE_FILE="${CACHE_DIR}/${CURRENT_DATE}_market.json"

  if [ "$WEEKDAY" -ge 6 ]; then
    echo "⏭️  Weekend ($CURRENT_DATE), skipping."
  elif [[ ${HOLIDAYS["$CURRENT_DATE_ISO"]+_} ]]; then
    echo "🏖️  Holiday ($CURRENT_DATE_ISO), skipping."
  elif [ -f "$CACHE_FILE" ]; then
    echo "🟡 Cache already exists for $CURRENT_DATE, skipping."
  else
    echo "📦 Caching market data for $CURRENT_DATE..."
    "$BIN_PATH" "$CURRENT_DATE"
  fi

  CURRENT_EPOCH=$((CURRENT_EPOCH + ONE_DAY))
done
