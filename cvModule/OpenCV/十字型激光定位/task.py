# 创建时间：2019年8月2日
# 功能：激光中心十字检测,计算中心点坐标
 
import cv2
import math
import numpy as np
 
# 激光十字检测计算直线参数函数
def calculate_line(x1, y1, x2, y2):
    if x1 > x2:
        x1, y1, x2, y2 = x2, y2, x1, y1
    k = (y1 - y2) /((x1 - x2)+0.0001)
    b = (y2 * x1 - y1 * x2) / ((x1 - x2)+0.0001)
 
    return k, b
 
# 激光十字检测计算交点
def calculate_intersection(line1, line2):
    a1 = line1['y2'] - line1['y1']
    b1 = line1['x1'] - line1['x2']
    c1 = line1['x2'] * line1['y1'] - line1['x1'] * line1['y2']
 
    a2 = line2['y2'] - line2['y1']
    b2 = line2['x1'] - line2['x2']
    c2 = line2['x2'] * line2['y1'] - line2['x1'] * line2['y2']
 
    if (a1 * b2 - a2 * b1) != 0 and (a2 * b1 - a1 * b2) != 0:
        cross_x = int((b1 * c2 - b2 * c1) / (a1 * b2 - a2 * b1))
        cross_y = int((c1 * a2 - c2 * a1) / (a1 * b2 - a2 * b1))
        if cross_x<0 or cross_y<0:
            print('检测错误！')
            return 320, 120   # 默认返回图像中心点位置
        return (cross_x, cross_y)
    print('未检测到目标！')
    return 320, 120  # 默认返回图像中心点位置
 
# 激光计算主函数
# 输入参数：img输入图像矩阵
# 输出参数：中心点坐标cx,cy;img_after:输出图像矩阵
def laser_detect(img):
 
    img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    img_gray = cv2.GaussianBlur(img_gray, (3, 3), 0)  # 高斯滤波
    img = cv2.inRange(img_gray, 185, 255)  # 二值化处理
    # # 全局降噪
    # dst = cv2.fastNlMeansDenoising(img, 10, 10, 7, 21)
    # cv2.imshow('gobal cotrol', dst)
    kernel = np.ones((5, 5), np.uint8)  # 定义卷积核
    img = cv2.dilate(img, kernel)  # 图像膨胀处理
    line_segs = cv2.HoughLinesP(img, rho=2,theta=0.1, threshold=100)
    for lseg in line_segs:
        x1,y1,x2,y2 = lseg[0]
        # 计算权重,线段长度作为权重
        weight = math.sqrt(math.pow(x1-x2, 2) + math.pow(y1-y2, 2))
        # print('x1: {}, y1: {}, x2: {}, y2: {}, weight: {}'.format(x1, y1, x2, y2, weight))
 
    lines = []
    # 最小权值
    min_weight = 20
    # 相同k之间最大的差距
    max_k_distance = 0.3
 
    for lseg in line_segs:
        # 获取线段端点值
        x1, y1, x2, y2 = lseg[0]
        if x1 > x2:
            x1, y1, x2, y2 = x2, y2, x1, y1
 
        # 计算权重
        weight = math.sqrt(math.pow(x1 - x2, 2) + math.pow(y1 - y2, 2))
 
        if weight != 0 and weight > min_weight:
            # 计算K与b
            k, b = calculate_line(x1, y1, x2, y2)
            # print('k: {:.2f}, b: {:.2f}, weight: {:.2f}'.format(k, b, weight))
 
            if len(lines) == 0:
                # 初次填充line
                line = {}
                line['cur_k'] = k
                line['cur_b'] = b
                line['k_sum'] = k * weight
                line['b_sum'] = b * weight
                line['weight_sum'] = weight
                line['x1'] = x1
                line['y1'] = y1
                line['x2'] = x2
                line['y2'] = y2
                lines.append(line)
                continue
 
            # 根据k的差异做加权
            # 首先获取lines数组里面k举例最近的那个
            neighbor_line = min(lines, key=lambda line: abs(line['cur_k'] - k))
 
            if abs(neighbor_line['cur_k'] - k) < max_k_distance:
                # 小于最大k差值，认为是同一条线
                neighbor_line['weight_sum'] += weight
                neighbor_line['k_sum'] += k * weight
                neighbor_line['b_sum'] += b * weight
                neighbor_line['cur_k'] = neighbor_line['k_sum'] / neighbor_line['weight_sum']
                neighbor_line['cur_b'] = neighbor_line['b_sum'] / neighbor_line['weight_sum']
 
                if neighbor_line['x1'] > x1:
                    neighbor_line['x1'] = x1
                    neighbor_line['y1'] = y1
 
                if neighbor_line['x2'] < x2:
                    neighbor_line['x2'] = x2
                    neighbor_line['y2'] = y2
 
            else:
                # 添加另外一条线
                # 初次填充line
                line = {}
                line['cur_k'] = k
                line['cur_b'] = b
                line['k_sum'] = k * weight
                line['b_sum'] = b * weight
                line['weight_sum'] = weight
                line['x1'] = x1
                line['y1'] = y1
                line['x2'] = x2
                line['y2'] = y2
                lines.append(line)
 
    # 根据权重对lines数组进行排序， 取前两个（lines的长度有可能大于2）
    sorted_lines = sorted(lines, key=lambda line: line['weight_sum'])[::-1]
    line1 = sorted_lines[0]
    line2 = sorted_lines[1]
 
    cx, cy = calculate_intersection(line1, line2)
    print('position:', cx, cy)
 
    img_after = cv2.cvtColor(img, cv2.COLOR_GRAY2BGR)
 
    return img_after, cx, cy
 
 
# cap = cv2.VideoCapture('../video/laser_2.mp4')
cap = cv2.VideoCapture(0)
# 设置分辨率
cap.set(3, 640)
cap.set(4, 240)
 
while cap.isOpened():
    ret, frame = cap.read()
 
    img_after, x, y = laser_detect(frame)  # 激光十字处理
    # 显示及作图
    cv2.circle(img_after, (x, y), 1, (0, 0, 255), thickness=10)
    cv2.imshow('laser', img_after)
 
    # 按下esc键退出
    key = cv2.waitKey(delay=2)
    if key == ord("q") or key == 27:
        break
 
cap.release()
cv2.destroyAllWindows()