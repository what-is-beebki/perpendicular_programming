#/usr/bin/env python3
import cv2
import argparse

def save_img_as_matrix(img, path):
    f = open(path, 'w')
    f.truncate(0)
    img.tofile(f, sep = ' ', format = '%s')
    f.close()
    return

def main():
    parser = argparse.ArgumentParser(prog='Img2Mat', description='converts the image into a text file')
    parser.add_argument('source', type=str)
    parser.add_argument('dest', type=str)
    args = parser.parse_args()

    img = cv2.imread(args.source)
    img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    save_img_as_matrix(img, args.dest)
    cv2.imshow('figures', img)
    cv2.waitKey(0)
    return

main()
