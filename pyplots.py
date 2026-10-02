import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv('benchmark_stats.csv')

def capacity_ms_mutex_spsc():

    x, y = df['capacity'], df['mean']

    fig, ax = plt.subplots()

    ax.set_xscale('log', base=2)
    ax.plot(x, y, color='purple')
    ax.set_title("mutex-based SPSC ; capacity (log2) vs mean time (ms)")
    ax.set_xlabel("capacity (max # elements)")
    ax.set_ylabel("mean time (ms)")
    fig.savefig('capacity_ms_mutex_spsc.png', dpi=300)


def percentiles_of_capacity_mutex_spsc():

    fig, ax = plt.subplots()

    x = [25, 50, 75, 90, 95, 99]
    cols = ["p25", "median", "p75", "p90", "p95", "p99"]

    for _, row in df.iterrows():
        ax.plot(x, row[cols].values, label=f"capacity {int(row['capacity'])}")

    ax.set_xlabel("percentiles")
    ax.set_ylabel("time (ms)")
    ax.set_title("mutex-based SPSC ; times by percentile for each capacity")
    ax.legend(bbox_to_anchor=(1.05, 1), loc='upper left')
    plt.tight_layout()
    fig.savefig('capacity_percentile_distribution_mutex_spsc.png', bbox_inches='tight')


if __name__ == '__main__':

    capacity_ms_mutex_spsc()
    percentiles_of_capacity_mutex_spsc()
    plt.show()
