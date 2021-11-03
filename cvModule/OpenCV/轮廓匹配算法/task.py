# 创建时间：2019年7月29日
# 轮廓匹配识别字母、常见简单轮廓等
 
import cv2
import numpy as np
 
 
# 不同特征的形状匹配
# 输入参数：model_img:目标图像矩阵；train_frame:待检测的图像矩阵
# 输出参数：matching_value:匹配值，越小表示匹配度越高
# 输出参数:cX,cY图像中心点坐标;train_frame_after处理后得到的图像矩阵
def contours_matching(model_img,train_frame):
    # 默认图像重心为中心点坐标
    cX = 320
    cY = 120
    # 转换成灰度图
    model_img = cv2.cvtColor(model_img,cv2.COLOR_BGR2GRAY)
    train_frame = cv2.cvtColor(train_frame,cv2.COLOR_BGR2GRAY)
    # 高斯滤波降噪
    model_img = cv2.GaussianBlur(model_img, (5,5), 0)
    train_frame = cv2.GaussianBlur(train_frame,(19,19),0)
    # 处理轮廓
    ret, thresh = cv2.threshold(model_img, 127, 255,0)
    ret, thresh2 = cv2.threshold(train_frame, 127, 255,0)
    model_img_after, contours, hierarchy = cv2.findContours(thresh,2,1)
    cnt1 = contours[0]
    train_frame_after, contours, hierarchy = cv2.findContours(thresh2,2,1)
    cnt2 = contours[0]
    # 计算匹配度
    matching_value = cv2.matchShapes(cnt1,cnt2,1,0.0)
    print('匹配度：', matching_value)
    # 匹配成功;匹配闸值为调整参数
    if matching_value<=0.2:
        # 计算检测到目标物的重心坐标
        M = cv2.moments(cnt2)
        cX = int(M["m10"] / (M["m00"] + 0.0001))
        cY = int(M["m01"] / (M["m00"] + 0.0001))
 
 
    return train_frame_after,matching_value, cX, cY
 
 
# 导入图像
# cap = cv2.VideoCapture('../video/alphabet_1.mp4')
cap = cv2.VideoCapture(1)
# 设置摄像头分辨率
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 240)
 
while cap.isOpened():
 
    ret, train_frame = cap.read()
    model_img = cv2.imread('../images/alphabets/O.jpg')
 
    train_frame_after, mathing_value, x, y = contours_matching(model_img,train_frame)
    if mathing_value<0.2:
        cv2.circle(train_frame_after, (x, y), 2, (0, 255, 0), 8)  # 做出中心坐标
    cv2.imshow('test',train_frame_after)
 
    # 按下esc键退出
    key = cv2.waitKey(delay=2)
    if key == ord("q") or key == 27:
        break
 
cap.release()
cv2.destroyAllWindows()