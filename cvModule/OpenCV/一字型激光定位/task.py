# 创建时间：2019年8月3日
# 直线中心点检测
# 二值化会受到不同光线的影响
 
import cv2
import numpy as np
 
# 一字型激光定位
# 输入参数：img输入图像矩阵；
# 输出参数：result处理后的图像矩阵；cx，cy直线的中心坐标
def laser_line(img):
 
    img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    img_gray = cv2.GaussianBlur(img_gray, (1, 1), 0)  # 高斯滤波
    # cv2.imshow('Guassian',img_gray)
    kernel = np.ones((5, 5), np.uint8)  # 定义卷积核
    img_gray = cv2.dilate(img, kernel)  # 图像膨胀处理
    # cv2.imshow('swell', img)
    edges = cv2.Canny(img_gray, 60, 150, apertureSize=3)  # 计算边缘
    # cv2.imshow('canny', edges)
 
    # 霍夫直线检测
    lines = cv2.HoughLines(edges, 1, np.pi/180, 200)
    result = img_gray.copy()
    # 概率霍夫直线检测，直线容易被分割
    # minLineLength = 20  # 最小直线长度
    # maxLineGap = 5  # 最大线段间隙
    # lines = cv2.HoughLinesP(edges, 1, np.pi / 180, 100, minLineLength, maxLineGap)
    # for x1, y1, x2, y2 in lines[0]:
    #     cv2.line(result, (x1, y1), (x2, y2), (0, 255, 0), 2)
    print('lines', lines)
    if lines is None:
        print('未识别直线!')
        return result, 320, 120  # 返回默认图像中心
    # 定义数组存储
    LX1 = []
    LX2 = []
    LY1 = []
    LY2 = []
    # 将极坐标系坐标转换成直角坐标系并作图
    for line in lines:
        rho, theta = line[0]  # line[0]存储的是点到直线的极径和极角，其中极角是弧度表示的
        a = np.cos(theta)   # theta是弧度
        b = np.sin(theta)
        x0 = a * rho
        y0 = b * rho
        x1 = int(x0 + 1000 * (-b))  # 直线起点横坐标
        y1 = int(y0 + 1000 * a)   # 直线起点纵坐标
        x2 = int(x0 - 1000 * (-b))  # 直线终点横坐标
        y2 = int(y0 - 1000 * a)   # 直线终点纵坐标
        LX1.append(x1)
        LY1.append(y1)
        LX2.append(x2)
        LY2.append(y2)
 
    # 使用检测到直线的均值进行近似
    x1 = int(sum(LX1)/len(lines))
    x2 = int(sum(LX2)/len(lines))
    y1 = int(sum(LY1)/len(lines))
    y2 = int(sum(LY2)/len(lines))
    print('x1, x2, y1, y2',x1, x2, y1, y2)
    cv2.line(result, (x1, y1), (x2, y2), (0, 0, 255), 2)
    # 直线中心位置
    cx = int((x1 + x2)/2)
    cy = int((y1 +y2)/2)
    print('position', cx, cy)
    cv2.circle(result, (cx, cy), 10, (0, 255, 0), -1)
 
    return result, cx, cy
 
 
# cap = cv2.VideoCapture('../video/line.mp4')
cap = cv2.VideoCapture(1)
# # 设置分辨率
# cap.set(3, 640)
# cap.set(4, 240)
 
while cap.isOpened():
    ret, frame = cap.read()
 
    result, x, y = laser_line(frame)  # 激光十字处理
    # 显示及作图
 
    cv2.imshow('laser', result)
 
    # 按下esc键退出
    key = cv2.waitKey(delay=2)
    if key == ord("q") or key == 27:
        break
 
cap.release()
cv2.destroyAllWindows()
 