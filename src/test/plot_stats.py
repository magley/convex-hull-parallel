import numpy as np
import matplotlib.pyplot as plt
import os
from pathlib import Path


input_dir = "..\\..\\out\\"
output_dir = "..\\..\\out\\"
input_prefix = "stats_cutoff"
output_prefix = "graph_cutoff"
path = Path(__file__).parent / input_dir

# draw_graph_for("out/statistics_01.txt", "01")
# will plot results and output to [output_dir]/[output_prefix][infix].png
def draw_graph_for(fname, infix):
    min_parallel = 99999
    min_index = -1


    point_count = 0
    x_cutoff = []
    y_time_serial = []
    y_time_parallel = []
    y_speedup = []

    with open(fname) as f:
        for i, line in enumerate(f):
            parts = line.split()

            point_count = int(parts[0])
            x_cutoff.append(int(parts[1]))
            y_time_serial.append(float(parts[2]) * 1000)
            y_time_parallel.append(float(parts[3]) * 1000)
            y_speedup.append(float(parts[4]))

            if float(parts[3]) * 1000 < min_parallel:
                min_parallel = float(parts[3])  * 1000
                min_index = i - 1


    plt.plot(x_cutoff, y_time_serial, linestyle="-", color="gray", label="serial::convex_hull")
    plt.plot(x_cutoff, y_time_parallel, linestyle="-", color="black", label="parallel::convex_hull")

    #poly = np.polyfit(x_cutoff, y_speedup,10)
    #poly_y = np.poly1d(poly)(x_cutoff)
    #plt.plot(x_cutoff,poly_y, color="black")
    #plt.scatter(x_cutoff,y_speedup, color="gray")


    star_label = f"Optimal time\ncutoff = {x_cutoff[min_index]}\nserial = {y_time_serial[min_index]:.3f}ms\nparallel = {y_time_parallel[min_index]:.3f}ms"
    plt.plot(x_cutoff[min_index], min_parallel, marker="*", markersize=15, color="dimgray", label=star_label)
    plt.legend(loc='upper center', bbox_to_anchor=(0.5, 1.25), ncol=3, shadow=True)

    plt.xlabel("cutoff")
    plt.ylabel("execution time [ms]")
    plt.minorticks_on()
    plt.grid(color='lightgray', linestyle='dashed', linewidth=1, which='both')

    print("SAVING ", str(Path(__file__).parent / output_dir) + "\\" + output_prefix + infix + ".png")
    plt.savefig(str(Path(__file__).parent / output_dir) + "\\" + output_prefix + infix + ".png", bbox_inches="tight")

    plt.clf()

def plot_test_case():
    for filename in os.listdir(path):   
        f = os.path.join(input_dir, filename)
        if os.path.isfile(f):
            if filename.startswith(input_prefix) and filename.endswith(".txt"):
                infix = filename.split(input_prefix)[1]
                infix = infix.split(".txt")[0]
                print(f)
                draw_graph_for(f, infix)

def plot_speedup():
    data = []
    with open(str(path) + "\\stats_speedup.txt") as f:
        for i, line in enumerate(f):
            parts = line.split()
            d = {}
            d['size'] = int(parts[0])
            d['cutoff'] = int(parts[1])
            d['serial'] = float(parts[2])
            d['parallel'] = float(parts[3])
            d['speedup'] = float(parts[4])
            data.append(d)
    x = [d['size'] for d in data]
    y = [d['speedup'] for d in data]

    poly = np.polyfit(x, y, 3)
    poly_y = np.poly1d(poly)(x)
    plt.grid(color='lightgray', linestyle='dashed', linewidth=1, which='both')
    plt.minorticks_on()
    plt.scatter(x, y, color="black", label="points", s=1)

    plt.xlabel("point count")
    plt.ylabel("speedup")
    
    plt.savefig(str(Path(__file__).parent / output_dir) + "\\" + output_prefix + "speedup2.png", bbox_inches="tight")
    plt.clf()

def plot_best_cutoff():
    data = []
    with open(str(path) + "\\stats_best_cutoff.txt") as f:
        for i, line in enumerate(f):
            parts = line.split()
            d = {}
            d['points'] = int(parts[0])
            d['best_cutoff'] = int(parts[1])
            data.append(d)
    x = [d['points'] for d in data]
    y = [d['best_cutoff'] for d in data]

    plt.minorticks_on()
    plt.scatter(x, y, color="black", label="regression", s=1)
    plt.xlabel("point count")
    plt.ylabel("best cutoff value")  
    plt.savefig(str(Path(__file__).parent / output_dir) + "\\" + output_prefix + "best_cutoff.png", bbox_inches="tight")
    plt.clf()

#plot_test_case()
#plot_speedup() #plt.ylim(top=7)
#plot_best_cutoff()