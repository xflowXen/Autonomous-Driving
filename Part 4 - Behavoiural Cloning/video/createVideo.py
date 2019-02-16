import cv2
import os

from cv2 import VideoWriter, VideoWriter_fourcc, imread, resize

fourcc = VideoWriter_fourcc(*'XVID')
vid = None
size = None

for root, dirs, files in os.walk("./track2"):  
    for filename in files:
        img = imread("./track2/" + filename)
        print("./track2/" + filename)
        if vid is None:
            if size is None:
                size = img.shape[1], img.shape[0]
            vid = VideoWriter("./track2.avi", fourcc, float(30), size)
        vid.write(img)
    vid.release()
        
