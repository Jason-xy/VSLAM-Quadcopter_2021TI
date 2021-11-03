# -*- coding: utf-8 -*-
# 创建时间：2019年7月30日
# 直方图均衡化测试
 
import cv2
import numpy as np
from matplotlib import pyplot as plt
from time import clock
import sys
 
'''
# 进行图像均衡化demo
img_1 = cv2.imread('../images/alphabets/A.jpg', 0)
print(img_1)
equ_1 = cv2.equalizeHist(img_1)
res = np.hstack((img_1,equ_1))
res = cv2.resize(res,(1280,640))
cv2.imshow('hist_test',res)
cv2.waitKey(0)
'''
'''
# 画2D直方图
img_1 = cv2.imread('../images/test_rectangle.jpg')
img_1 = cv2.resize(img_1, (1280, 720))
hsv_1 = cv2.cvtColor(img_1, cv2.COLOR_BGR2HSV)
hist_1 = cv2.calcHist([hsv_1], [0, 1], None, [180, 256], [0, 180, 0, 256])
cv2.imshow('rectangle', img_1)
plt.imshow(hist_1,interpolation = 'nearest')
plt.show()
cv2.waitKey(0)
'''
# 颜色直方图编码
if __name__ == '__main__':
    # 构建 HSV 颜色地图
    hsv_map = np.zeros((180, 256, 3), np.uint8)
 
    h, s = np.indices(hsv_map.shape[:2])
    hsv_map[:,:,0] = h
    hsv_map[:,:,1] = s
    hsv_map[:,:,2] = 255
    hsv_map = cv2.cvtColor(hsv_map, cv2.COLOR_HSV2BGR)
    cv2.imshow('hsv_map', hsv_map)
    cv2.namedWindow('hist', 0)
 
    hist_scale = 10
    def set_scale(val):
        global hist_scale
        hist_scale = val
    cv2.createTrackbar('scale', 'hist', hist_scale, 32, set_scale)
    # cap = cv2.VideoCapture('../video/test_rectangle_1.mp4')
    cap = cv2.VideoCapture(1)
    while True:
        flag, frame = cap.read()
        cv2.imshow('camera', frame)
 
        small = cv2.pyrDown(frame)
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        # 去除孤立的点
        dark = hsv[:, :, 2] < 32
        hsv[dark] = 0
        h = cv2.calcHist( [hsv], [0, 1], None, [180, 256], [0, 180, 0, 256] )
        h = np.clip(h*0.005*hist_scale, 0, 1)
 
        # 从一维变成三维
        # 将得到的直方图和颜色直方图相乘
        vis = hsv_map*h[:, :, np.newaxis] / 255.0
        cv2.imshow('hist', vis)
        ch = 0xFF & cv2.waitKey(1)
        if ch == 27:
            break
cv2.destroyAllWindows()