import numpy as np
import matplotlib.pyplot as plt
import os
from pathlib import Path


input_dir = "..\\..\\out\\"
output_dir = "..\\..\\out\\"
input_prefix = "statistics_"
output_prefix = "graph_"
path = Path(__file__).parent / input_dir

# draw_graph_for("out/statistics_01.txt", "01")
# will plot results and output to [output_dir]/[output_prefix][infix].png
def draw_graph_for(fname, infix):
    point_count = 0
    x_cutoff = []
    y_time_serial = []
    y_time_parallel = []
    y_speedup = []

    with open(fname) as f:
        for i, line in enumerate(f):
            if i == 0:
                point_count = int(line)
                continue

            parts = line.split()

            x_cutoff.append(int(parts[0]))
            y_time_serial.append(float(parts[1]) * 1000)
            y_time_parallel.append(float(parts[2]) * 1000)
            y_speedup.append(float(parts[3]))


    plt.plot(x_cutoff, y_time_serial, linestyle="-", color="gray", label="serial::convex_hull")
    plt.plot(x_cutoff, y_time_parallel, linestyle="-", color="black", label="parallel::convex_hull")
    plt.legend()

    plt.xlabel("cutoff")
    plt.ylabel("execution time [ms]")
    plt.title(str(point_count) + " points")
    plt.minorticks_on()
    plt.grid(color='lightgray', linestyle='dashed', linewidth=1, which='both')

    print("SAVING ", str(Path(__file__).parent / output_dir) + "\\" + output_prefix + infix + ".png")
    plt.savefig(str(Path(__file__).parent / output_dir) + "\\" + output_prefix + infix + ".png")

    plt.clf()

for filename in os.listdir(path):   
    f = os.path.join(input_dir, filename)
    if os.path.isfile(f):
        if filename.startswith(input_prefix) and filename.endswith(".txt"):
            infix = filename.split(input_prefix)[1]
            infix = infix.split(".txt")[0]
            print(f)
            draw_graph_for(f, infix)