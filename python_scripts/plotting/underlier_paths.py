
import os
import pandas as pd
import matplotlib.pyplot as plt


def plot_paths_from_file(filename_stem: str, csv_folder: str = "csv_paths/equity_simulations",max_paths:int=30):
    """
    Plots Monte Carlo paths from a CSV file where each row represents a path
    and each column represents a time step (usually a date).

    Parameters
    ----------
    filename_stem : str
        File name stem without .csv extension (e.g. 'tradeId_1_17-01-2025_Euler_paths').

    csv_folder : str
        Folder where CSV files are stored (default is 'csv_paths').

    Raises
    ------
    FileNotFoundError
        If the specified file does not exist.

    Returns
    -------
    None
    """

    filename = filename_stem if filename_stem.endswith(".csv") else f"{filename_stem}.csv"
    csv_path = os.path.join(csv_folder, filename)

    if not os.path.exists(csv_path):
        raise FileNotFoundError(f"❌ File not found: {csv_path}")

    df = pd.read_csv(csv_path)
    df_T = df.T  # transpose: rows = time steps, columns = paths

    print(f"✅ Loaded {df_T.shape[1]} paths × {df_T.shape[0]} time steps from: {csv_path}")

    for i, col in enumerate(df_T.columns):
        if i >= max_paths:
            break
        plt.plot(df_T[col], alpha=0.5, linewidth=1)

    plt.title(filename_stem)
    plt.xlabel("Date")
    plt.ylabel("Simulated Price")
    plt.xticks(rotation=45)
    plt.gca().xaxis.set_major_locator(plt.MaxNLocator(10))
    plt.grid(True)
    plt.tight_layout()
    plt.show()


if __name__ == '__main__':
    pass