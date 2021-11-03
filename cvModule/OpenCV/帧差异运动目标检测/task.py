import cv2
import numpy as np
 
camera = cv2.VideoCapture(1)
 
es = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (10, 10))
kernel = np.ones((5, 5), np.uint8)
background = None
 
while (True):
    ret, frame = camera.read()
    if background is None:
        # 将第一帧图像设为背景
        # 灰度和平滑处理
        background = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        background = cv2.GaussianBlur(background, (21, 21), 0)
        continue
 
    # 灰度和平滑处理
    gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    gray_frame = cv2.GaussianBlur(gray_frame, (21, 21), 0)
    # 计算背景帧和读取帧的差异
    diff = cv2.absdiff(background, gray_frame)
    # 二值化
    diff = cv2.threshold(diff, 25, 255, cv2.THRESH_BINARY)[1]
    diff = cv2.dilate(diff, es, iterations=2)   # 膨胀处理
    cnts, hierarchy = cv2.findContours(diff.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
 
    for c in cnts:
        # 矩形框只显示大于闸值的点
        if cv2.contourArea(c) < 1500:
            continue
        # 计算矩形框
        (x, y, w, h) = cv2.boundingRect(c)
        cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 255, 0), 2)
 
    cv2.imshow("contours", frame)
    cv2.imshow("dif", diff)
    # 按下esc键退出
    key = cv2.waitKey(delay=2)
    if key == ord("q") or key == 27:
        break
 
cv2.destroyAllWindows()
camera.release()