#!/usr/bin/env bash
set -euo pipefail

# Użycie:
#   ./dump_option_tickers.sh TICKER VAL_DATE [OUTROOT]
# Przykład:
#   ./dump_option_tickers.sh AAPL 12-08-2025 ./options_index

TICKER="${1:?Usage: $0 TICKER VAL_DATE [OUTROOT]}"
VAL_DATE="${2:?Usage: $0 TICKER VAL_DATE [OUTROOT]}"
OUTROOT="${3:-./options_index}"

API_KEY="${API_KEY:-2I8_POlvy5wQLpOUZIwIsl7LH9Hm8yPp}"

command -v curl >/dev/null || { echo "❌ curl required"; exit 1; }
command -v jq   >/dev/null || { echo "❌ jq required"; exit 1; }

OUTDIR="${OUTROOT}/${VAL_DATE}"
mkdir -p "$OUTDIR"

# VAL_DATE (DD-MM-YYYY) -> YYYY-MM-DD
if date -j -f "%d-%m-%Y" "$VAL_DATE" "+%Y-%m-%d" >/dev/null 2>&1; then
  AS_OF_ISO="$(date -j -f "%d-%m-%Y" "$VAL_DATE" "+%Y-%m-%d")"   # macOS
else
  AS_OF_ISO="$(date -d "$(echo "$VAL_DATE" | awk -F- '{print $3"-"$2"-"$1}')" +%F)"  # Linux
fi

# Zbierz WSZYSTKIE tickery opcji "as_of" (aktywnych) – z paginacją po next_url
ALL=""
URL="https://api.polygon.io/v3/reference/options/contracts?underlying_ticker=${TICKER}&expired=false&as_of=${AS_OF_ISO}&sort=expiration_date&order=asc&limit=1000&apiKey=${API_KEY}"
while [ -n "$URL" ]; do
  RESP="$(curl -s "$URL")"
  ALL+="$(
    echo "$RESP" | jq -r '.results[]?.ticker'
  )"$'\n'
  NEXT="$(echo "$RESP" | jq -r '.next_url // empty')"
  if [ -n "$NEXT" ]; then
    if [[ "$NEXT" == *"apiKey="* ]]; then
      URL="$NEXT"
    else
      SEP='?'; [[ "$NEXT" == *\?* ]] && SEP='&'
      URL="${NEXT}${SEP}apiKey=${API_KEY}"
    fi
  else
    URL=""
  fi
done

# Dedup + sort i zapisz jako czysta lista JSON (array of strings)
printf "%s\n" "$ALL" \
| grep -v '^[[:space:]]*$' \
| sort -u \
| jq -R -s 'split("\n") | map(select(length>0))' \
> "${OUTDIR}/${TICKER}_${VAL_DATE}.json"

echo "✅ Zapisano: ${OUTDIR}/${TICKER}_${VAL_DATE}.json"
