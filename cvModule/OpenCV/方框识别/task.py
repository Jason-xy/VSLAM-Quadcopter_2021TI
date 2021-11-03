# -*- coding:utf-8 -*-
# 创建时间：2019年7月18日
# 功能：用前置摄像头检测黑色矩形，返回矩形中心点坐标
# 实现方法：使用黑色为依据识别
# 注：矩形框宽100cm，高70cm；边框为黑色，边框宽度不大于6cm，
#    矩形框下边框距地面110cm
# 修改：2019年7月23日：1.重新调整好参数
#                     2.输出识别物体中心坐标
#                     3.添加错误识别函数；
 
 
import cv2
import numpy as np
import time
 
 
# 初始化函数
def Init():
    # cap = cv2.VideoCapture(0)
    cap = cv2.VideoCapture('./1.mp4')
    cap.set(3,320)  # 设置摄像头输出宽
    cap.set(4,240)  # 设置摄像头输出高
    # 设置摄像头分辨率
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 240)
    print("start reading video...")
    time.sleep(2.0)
    print("start working")
 
    return cap
 
cap = Init()  # 初始化
 
while cap.isOpened():
    ret, frame = cap.read()
    frame_copy = frame.copy()
    hsv_frame = cv2.cvtColor(frame,cv2.COLOR_BGR2HSV)  # 将每一帧图片转化HSV空间颜色
    hsv_frame_after = cv2.GaussianBlur(hsv_frame,(19,19),0) # 高斯滤波进行平滑处理
 
    # 依据之前的脚本获取的阈值设置最高值与最低值
    lower = np.array([76, 19, 56])
    upper = np.array([124, 73, 125])
 
    mask = cv2.inRange(hsv_frame_after, lower, upper)  # 构建掩膜
    # thresh = cv2.threshold(hsv_frame_after, 60, 255, cv2.THRESH_BINARY)[1]  # 进行闸值化
    conts, hier = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)  # 找出边界
    cv2.drawContours(frame, conts, -1, (0, 255, 0), 3)  # 画出边框
    # 计算出中心坐标并输出
    cnts = conts[0]
    M = cv2.moments(cnts)
    cX = int(M["m10"] / (M["m00"] + 0.0001))
    cY = int(M["m01"] / (M["m00"] + 0.0001))
    if cX != 0 and cY != 0:
        print('中心坐标',cX,cY)
    if cX == 0 or cY == 0:
        print('识别有误！')
 
    dst = cv2.bitwise_and(frame, frame, mask=mask)  # 对每一帧进行位与操作，滤去除要目标物体之外的图像
 
    cv2.imshow('the original video',frame_copy)
    cv2.imshow('test',dst)
 
    # 按下esc键退出
    key = cv2.waitKey(delay=2)
    if key == ord("q") or key == 27:
        break
 
cap.release()
cv2.destroyAllWindows()