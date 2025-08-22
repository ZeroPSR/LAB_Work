import pandas as pd
import matplotlib.pyplot as plt
import glob

# Find all CSV files in the current directory
csv_files = glob.glob("*.csv")

for csv_file in csv_files:
    df = pd.read_csv(csv_file)
    x = df.iloc[:, 0]
    plt.figure(figsize=(10, 6))
    for col in df.columns[1:]:
        plt.plot(x, df[col], marker='o', label=col)
    plt.xlabel('Input Size')
    plt.ylabel('Execution Time (ms)')
    plt.title(f'Input Size vs Timings ({csv_file})')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(csv_file.replace('.csv', '.png'))
    plt.show()
