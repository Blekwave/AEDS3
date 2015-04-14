import random
import sys

num_nodes = int(sys.argv[1])

keys = list(range(0,num_nodes))
random.shuffle(keys)

print(num_nodes*2)

for i in range(0,num_nodes):
    print("INSERT", keys[i], i)

random.shuffle(keys)

for i in range(0,num_nodes):
    print("REMOVE", keys[i])