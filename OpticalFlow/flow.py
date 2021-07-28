#!/usr/bin/env python
# -*- coding: utf-8 -*-

import cv2 as cv
import numpy as np

cap = cv.VideoCapture(r"C:\Users\Jason\Desktop\test.mp4")

ret, preFrame = cap.read()
flow = preFrame
GPUpreFrame = cv.cuda_GpuMat()
GPUpreFrame.upload(preFrame)
# 转灰度
GPUpreFrameGary = cv.cuda_cvtColor(GPUpreFrame, cv.COLOR_BGR2GRAY)
# 光流可视化
# hsv = np.zeros_like(preFrame)
# hsv[... , 1] = 255

step = 10
while True:
    ret, curFrame = cap.read()
    displayFrame = curFrame
    if ret is not True:
        break
    GPUcurFrame = cv.cuda_GpuMat()
    GPUcurFrame.upload(curFrame)
    GPUcurFrameGary = cv.cuda_cvtColor(GPUcurFrame, cv.COLOR_BGR2GRAY)
    GPUflow = cv.cuda.calcOpticalFlowFarneback(GPUpreFrameGary,GPUcurFrameGary, None, 0.5, 3, 15, 3, 5, 1.2, 0)
    
    GPUpreFrameGary = GPUcurFrameGary
    print(flow)
    # mag, ang = cv.cartToPolar(flow[..., 0], flow[..., 1])
    # hsv[..., 0] = ang * 180 / np.pi / 2
    # hsv[..., 2] = cv.normalize(mag, None, 0, 255, cv.NORM_MINMAX)
    # bgr = cv.cvtColor(hsv, cv.COLOR_HSV2BGR)
    # displayFrame = cv.addWeighted(bgr, 0.2, displayFrame, 0.8, 0)
    # cv.imshow("Frame", displayFrame)
    np.allclose(GPUflow.download(), flow)
    h, w = curFrame.shape[:2]
    y, x = np.mgrid[step / 2:h:step, step / 2:w:step].reshape(2, -1).astype(int)
    fx, fy = flow[y, x].T
    lines = np.vstack([x, y, x + fx, y + fy]).T.reshape(-1, 2, 2)
    lines = np.int32(lines)
    
    line = []
    for l in lines:
        if l[0][0]-l[1][0]>3 or l[0][1]-l[1][1]>3:
            line.append(l)
    
    cv.polylines(curFrame, line, 0, (0,255,255))
    cv.imshow('flow', curFrame)
    ch = cv.waitKey(20) & 0xff
    if ch == 27:
        break
    
cap.release()
