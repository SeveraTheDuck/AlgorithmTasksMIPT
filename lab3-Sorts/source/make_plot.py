import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import sys

# argv[0] = program
# argv[1] = output folder
# argv[2] = plot title
# argv[3] = num of sorts
ARGC_SHIFT = 4

colors = ['b', 'g', 'r', 'c','m', 'y', 'k', 'w']

for i in range(ARGC_SHIFT, int(sys.argv[3]) + ARGC_SHIFT):
    data_file_name = sys.argv[i]
    sort_name = data_file_name[data_file_name.find('/') + 1 : data_file_name.find('.')]
    

    data = pd.read_csv(data_file_name, sep='\s+', header=None)
    data = pd.DataFrame(data)

    x = data[0]
    y = data[1]
    plt.plot (x, y, colors[i - ARGC_SHIFT], label=sort_name)

plt.title(sys.argv[2])
plt.xlabel("Elem number")
plt.ylabel("Time sec")
plt.legend(loc="upper left")
plt.savefig("plots/" + sys.argv[2] + ".png")