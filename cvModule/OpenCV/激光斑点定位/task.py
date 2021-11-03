# 创建时间：2019年8月3日
# 激光斑点定位
 
import cv2
import numpy as np
 
# 激光点中心定位
# 输入参数：img输入图像矩阵
# 输出参数：img_with_keypoints输出图像矩阵；cx,cy激光点位置
def laser_point(img):
    img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    img = cv2.inRange(img, 230, 255)  # 二值化
    # 图像膨胀
    kernel = np.ones((5, 5), np.uint8)  # 定义卷积核
    img = cv2.dilate(img, kernel)
 
    # 设置Blob检测参数
    params = cv2.SimpleBlobDetector_Params()
    # 设置颜色
    params.blobColor = 255
    # 设置闸值
    params.minThreshold = 1
    params.maxThreshold = 20
    # 设置面积
    params.filterByArea = True
    params.minArea = 10
    params.maxArea = 500
    # 设置圆性
    params.filterByCircularity = True
    params.minCircularity = 0.8
    # 设置惯量比
    params.filterByInertia = True
    params.minInertiaRatio = 0.2
 
    # 判断opencv版本再进行实例化
    ver = (cv2.__version__).split('.')
    if int(ver[0]) < 3:
        detector = cv2.SimpleBlobDetector(params)
    else:
        detector = cv2.SimpleBlobDetector_create(params)
 
    # Blob检测
    keypoints = detector.detect(img)
 
    img_with_keypoints = cv2.drawKeypoints(img, keypoints, np.array([]), (0, 255, 0),
                                          cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
    # 目标坐标处理
    if(keypoints):
        for i in range (0, len(keypoints)):
            x = keypoints[i].pt[0]
            y = keypoints[i].pt[1]
            cx = int(x)
            cy = int(y)
            print('position', cx, cy)
    else:
        print('未能正常检测')
        cx = 320
        cy = 120
 
    return img_with_keypoints, cx, cy
 
 
# cap = cv2.VideoCapture('../video/point.mp4')
cap = cv2.VideoCapture(1)
# # 设置分辨率
# cap.set(3, 640)
# cap.set(4, 240)
 
while cap.isOpened():
    ret, frame = cap.read()
 
    img_with_keypoints, x, y = laser_point(frame)  
    cv2.imshow('laser', img_with_keypoints)
 
    # 按下esc键退出
    key = cv2.waitKey(delay=2)
    if key == ord("q") or key == 27:
        break
 
cap.release()
cv2.destroyAllWindows()
 