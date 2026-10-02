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
    plt.show()

if __name__ == '__main__':

    capacity_ms_mutex_spsc()
