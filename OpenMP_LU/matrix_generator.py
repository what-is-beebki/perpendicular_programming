#!/usr/bin/env python3
from numpy.random import rand
from numpy import dot
from numpy.linalg import det
#from numpy.ndarray import tofile

SIZE = int(10)
SCALE = 10.0
out_path = 'matrix_A.txt'

def main():
    matrix = rand(SIZE, SIZE).dot(SCALE)

    while det(matrix) == 0:
       matrix = rand(SIZE, SIZE).dot(SCALE)

    #print(matrix)

    f = open(out_path, 'wb')
    f.truncate(0)
    matrix.tofile(f, sep = '', format = '%sf')
    f.close()

    #readable version
    #f = open(out_path, 'w')
    #f.truncate(0)
    #matrix.tofile(f, sep = ' ', format = '%f')
    #f.close()
    return

main()
