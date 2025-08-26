import pandas_datareader.data as web
import datetime
import json
import os

# === Mapowanie tenorów na ID FRED ===
FRED_SERIES_IDS = {
    "1M": "DGS1MO",
    "3M": "DGS3MO",
    "6M": "DGS6MO",
    "1Y": "DGS1",
    "2Y": "DGS2",
    "3Y": "DGS3",
    "5Y": "DGS5",
    "7Y": "DGS7",
    "10Y": "DGS10"
}

def fetch_fred_rate(series_id: str, date: datetime.date, api_key: str) -> float:
    try:
        df = web.DataReader(series_id, "fred", date, date, api_key=api_key)
        return round(df.iloc[0, 0] / 100.0, 6)  # Przekształcamy z % na wartość dziesiętną
    except Exception as e:
        print(f"❌ Błąd pobierania {series_id} dla {date}: {e}")
        return None

def prepare_ir_curve_json(date: datetime.date, api_key: str, output_dir: str = "../cached_market_data"):
    output = []
    for tenor, fred_id in FRED_SERIES_IDS.items():
        rate = fetch_fred_rate(fred_id, date, api_key)
        if rate is not None:
            instr_type = "deposit" if tenor in {"1M", "3M", "6M"} else "swap"
            output.append({
                "type": instr_type,
                "tenor": tenor,
                "rate": rate,
                "day_count": "Actual/365"
            })

    # === Zapis do pliku JSON ===
    os.makedirs(output_dir, exist_ok=True)
    file_path = os.path.join(output_dir, f"{date.strftime('%d-%m-%Y')}_ir_curve.json")

    with open(file_path, "w") as f:
        json.dump(output, f, indent=4)

    print(f"✅ Zapisano dane do: {file_path}")
    return file_path

# === PRZYKŁADOWE UŻYCIE ===
if __name__ == "__main__":
    fred_api_key = "1e762f897c6154c9c4285238f972033e"
    my_date = datetime.date(2025, 1, 2)
    prepare_ir_curve_json(my_date, fred_api_key)