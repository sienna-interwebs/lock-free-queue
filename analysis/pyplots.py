import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
from scipy.interpolate import griddata

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

def surface_plot_mutex_spsc():

    fig, ax = plt.subplots(subplot_kw={'projection': '3d'})

    log2_vals = np.log2(df['capacity'])

    percentiles = ['p25', 'median', 'p75', 'p90', 'p95', 'p99']

    raw_x = np.array([25, 50, 75, 90, 95, 99])
    raw_y = log2_vals.values
    raw_z = df[percentiles].values

    start = log2_vals.min()
    stop = log2_vals.max()

    points = []
    values = []

    for i, y in enumerate(raw_y):

        for j, x in enumerate(raw_x):

            points.append((x, y))
            values.append(raw_z[i, j])


    x_space = np.linspace(0, 100, num=100)
    y_space = np.linspace(start, stop, num=100)

    X, Y = np.meshgrid(x_space, y_space)
    Z = griddata(points, values, (X, Y), method='cubic')

    surf = ax.plot_surface(X, Y, Z, cmap='viridis', edgecolor='none')
    ax.set_xlabel('percentiles')
    ax.set_ylabel('capacities (log2)')
    ax.set_zlabel('time (ms)')
    fig.savefig('mutex_spsc_surface')







if __name__ == '__main__':

    capacity_ms_mutex_spsc()
    percentiles_of_capacity_mutex_spsc()
    surface_plot_mutex_spsc()
    plt.show()
