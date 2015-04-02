import random
import sys

num_nodes = int(sys.argv[1])

keys = list(range(0,num_nodes))
random.shuffle(keys)

print(num_nodes)

for i in range(0,num_nodes):
    print("INSERT", keys[i], i)
