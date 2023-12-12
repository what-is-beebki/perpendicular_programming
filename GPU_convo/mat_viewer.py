#/usr/bin/env python3
from cv2 import imshow, waitKey
from numpy import fromfile, reshape, array, uint8
import argparse

def main():
    parser = argparse.ArgumentParser(prog='Img2Mat', description='converts the image into a text file')
    parser.add_argument('source', type=str, default = 'conv_matrix.txt')
    parser.add_argument('size_x', type=int, default = 8)
    parser.add_argument('size_y', type=int, default = 8)
    args = parser.parse_args()

    f = open(args.source)
    img = fromfile(f, dtype=int, count=-1, sep=' ')
    f.close()
    img = reshape(img, (args.size_y, args.size_x), 'C')
    img = array(img, dtype = uint8)
    #print(img)
    imshow('figures', img)
    waitKey(0)

    return

main()
