
from random import randint

# generate a randomized csv file
with open('sample.csv', 'w') as sample_file:
    for i in range(500):
        a, b = randint(0, 500), randint(0, 1000)
        sample_file.write('%d,%d\n' %(a, b))
