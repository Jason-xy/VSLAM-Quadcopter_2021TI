# 创建时间：2019年8月5日
# 稀疏光流追踪检测到的角点
 
import numpy as np
import cv2
 
cap = cv2.VideoCapture(1)
 
# ShiTomasi角点检测的参数
feature_params = dict(maxCorners=100,
                      qualityLevel=0.3,
                      minDistance=7,
                      blockSize=7)
# 设置Lucas kanade光流参数
lk_params = dict(winSize=(15, 15),
                 maxLevel=2,  # 图像金字塔层数
                 criteria=(cv2.TERM_CRITERIA_EPS | cv2.TERM_CRITERIA_COUNT, 10, 0.03))
 
# 创建随机颜色
color = np.random.randint(0, 255, (100, 3))
# 处理第一帧图像并发现角点
ret, old_frame = cap.read()
old_gray = cv2.cvtColor(old_frame, cv2.COLOR_BGR2GRAY)
p0 = cv2.goodFeaturesToTrack(old_gray, mask=None, **feature_params)  # 确定图像上的角点
# 创建掩膜
mask = np.zeros_like(old_frame)
 
while True:
    ret, frame = cap.read()
    frame_gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    # calculate optical flow 能够获取点的新位置
    p1, st, err = cv2.calcOpticalFlowPyrLK(old_gray, frame_gray, p0, None, **lk_params)
 
    if p1 is None or p0 ==[]:
        print('No flow find!')
        break
    # 筛选出我们需要的点
    good_new = p1[st == 1]
    good_old = p0[st == 1]
    # 画出轨迹
    for i, (new, old) in enumerate(zip(good_new, good_old)):
        a, b = new.ravel()
        c, d = old.ravel()
        mask = cv2.line(mask, (int(a), int(b)), (int(c), int(d)), color[i].tolist(), 2)
        frame = cv2.circle(frame, (int(a), int(b)), 5, color[i].tolist(), -1)
 
    img = cv2.add(frame, mask)
    cv2.imshow('frame', img)
 
    # 更新图像
    old_gray = frame_gray.copy()
    p0 = good_new.reshape(-1, 1, 2)
 
    # 按下esc键退出
    key = cv2.waitKey(delay=2)
    if key == ord("q") or key == 27:
        break
 
cap.release()
cv2.destroyAllWindows()