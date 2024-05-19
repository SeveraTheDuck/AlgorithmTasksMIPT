import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import sys

# argv[0] = program
# argv[1] = output folder
# argv[2] = plot title
# argv[3] = num of algorythms
# argv[4]
ARGC_SHIFT = 5

colors = ['b', 'g', 'r', 'c','m', 'y', 'k', 'w']

for i in range(ARGC_SHIFT, int(sys.argv[3]) + ARGC_SHIFT):
    data_file_name = sys.argv[i]
    algo_name = data_file_name[data_file_name.find('/') + 1 : data_file_name.find('_')]
    

    data = pd.read_csv(data_file_name, sep='\s+', header=None)
    data = pd.DataFrame(data)

    x = data[0]
    y = data[1]
    means_x = []
    means_y = []
    for j in range(0, len(y) - int(sys.argv[4]) + 1, int(sys.argv[4])):
        means_x.append(np.mean(x[j : j + int(sys.argv[4])]))
        means_y.append(np.mean(y[j : j + int(sys.argv[4])]))

    # print (means_y)

    plt.plot (means_x, means_y, colors[i - ARGC_SHIFT], label=algo_name)

plt.title(sys.argv[2])
plt.xlabel("Elem number")
plt.ylabel("Time sec")
plt.legend(loc="upper left")
plt.savefig("plots/" + sys.argv[2] + ".png")