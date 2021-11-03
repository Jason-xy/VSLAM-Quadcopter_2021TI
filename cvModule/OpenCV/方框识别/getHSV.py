# -*- coding : utf-8 -*-
# 创建时间：2019年7月20
# 功能：调试出最佳的识别黑色兴趣区域的参数值
import cv2
import numpy as np
 
# cap = cv2.VideoCapture(0)  # 获取摄像头图像
# cap = cv2.VideoCapture('../video/test_rectangle_1.mp4')
 
img = cv2.imread("./getHSV.png")
img = cv2.resize(img,(960,600))
hsv_frame = cv2.cvtColor(img,cv2.COLOR_BGR2HSV)
 
def nothing(x):
    pass
 
 
def createbars():
    """
    实现创建六个滑块的作用，分别控制H、S、V的最高值与最低值
    H:色调；S：饱和度；V：亮度
    """
    cv2.createTrackbar("H_l", "image", 0, 180, nothing)
    cv2.createTrackbar("H_h", "image", 0, 180, nothing)
    cv2.createTrackbar("S_l", "image", 0, 255, nothing)
    cv2.createTrackbar("S_h", "image", 0, 255, nothing)
    cv2.createTrackbar("V_l", "image", 0, 255, nothing)
    cv2.createTrackbar("V_h", "image", 0, 255, nothing)
 
 
cv2.namedWindow("image")
createbars()  # 创建六个滑块
 
lower = np.array([0, 0, 0])  # 设置初始值
upper = np.array([0, 0, 0])
while True:
    # ret, frame = cap.read()
    # hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)  # 将图片由BGR颜色空间转化成HSV空间，HSV可以更好地分割颜色图形
    hsv_frame = cv2.GaussianBlur(hsv_frame,(9,9),0)
    lower[0] = cv2.getTrackbarPos("H_l", "image")  # 获取"H_l"滑块的实时值
    upper[0] = cv2.getTrackbarPos("H_h", "image")  # 获取"H_h"滑块的实时值
    lower[1] = cv2.getTrackbarPos("S_l", "image")
    upper[1] = cv2.getTrackbarPos("S_h", "image")
    lower[2] = cv2.getTrackbarPos("V_l", "image")
    upper[2] = cv2.getTrackbarPos("V_h", "image")
 
    mask = cv2.inRange(hsv_frame, lower, upper)  # cv2.inrange()函数通过设定的最低、最高阈值获得图像的掩膜
    # mask = cv2.inRange(hsv_img, lower, upper)  # cv2.inrange()函数通过设定的最低、最高阈值获得图像的掩膜
    # cv2.imshow("img", frame)
    cv2.imshow("img",img)
    cv2.imshow("mask", mask)
    if cv2.waitKey(1) & 0xff == 27:
        break
 
cv2.destroyAllWindows()