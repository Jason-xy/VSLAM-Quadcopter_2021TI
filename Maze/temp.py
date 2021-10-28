# coding=utf-8
import cv2
import numpy as np
# from PIL import Image, ImageDraw, ImageFont
# from skimage.measure import regionprops
import math
 
input_img_file = "/home/jason/cvtask/map3.jpg"

# 轮廓检测
def Contours(img):
    # 灰度化
    img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    kernel = np.ones((5,5),np.uint8)  
    erosion = cv2.erode(img_gray,kernel,iterations = 2)
    # cv2.imshow("erosion",erosion)
    # 二值化
    ret, thresh = cv2.threshold(erosion, 80, 255, cv2.THRESH_BINARY)
    # cv2.imshow("thresh",thresh)
    # 寻找轮廓
    contours, hierarchy = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    # 计算平均面积
    areas = list()
    for i in range(len(contours)):
        area = cv2.contourArea(contours[i], False)
        areas.append(area)
        print("轮廓%d的面积:%d" % (i, area))

    area_avg = np.average(areas)
    print("轮廓平均面积:", area_avg)

    # 筛选超过平均面积的轮廓
    img_contours = []
    img_maze = np.zeros(img.shape, np.uint8)
    for i in range(len(contours)):
        img_temp = np.zeros(img.shape, np.uint8)
        img_contours.append(img_temp)

        area = cv2.contourArea(contours[i], False)
        if area < img.shape[0] * img.shape[1] * 0.7 and area > area_avg/2 : # area > area_avg / 18(显示每一个边缘) 18是根据统计学计算的，现在用的是显示最大连通域（小心特例）
            print("轮廓%d的面积是: %d" % (i, area))
            # print(contours[i])
            cv2.drawContours(img_maze, contours, i, (0, 0, 255), 10)
            # print(contours[i])
    # 锐化
    kernel = np.array([[0, -1, 0], [-1, 5, -1], [0, -1, 0]], np.float32) #定义一个核
    img_maze = cv2.filter2D(img_maze, -1, kernel=kernel)
    # 均值滤波
    img_maze = cv2.blur(img_maze, (3, 5))  # 模板大小为3*5, 模板的大小是可以设定的
    img_maze = cv2.boxFilter(img_maze, -1, (3, 5))
    cv2.imshow("img_maze", img_maze)

    H_rows, W_cols = img_maze.shape[:2]
    mask = np.zeros([H_rows+2, W_cols+2], np.uint8)
    cv2.floodFill(img_maze, mask, (0, 0), (0, 255, 225), cv2.FLOODFILL_FIXED_RANGE)
    return img_maze

# Harris角点
def HarrisDetect(img):
    # 转换成灰度图像
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    # 高斯模糊
    gray = cv2.GaussianBlur(gray, (7, 7), 0)
    # 图像转换为float32
    gray = np.float32(gray)
    # harris角点检测
    dst = cv2.cornerHarris(gray, 5, 5, 0.04)
    # 阈值设定
    img[dst > 0.00001*dst.max()] = [225, 0, 0]
    # cv2.imshow('Harrisimg', img)
    # cv2.imshow('Harris', dst)

    return img

# 求顶点
def aroundCorner(Harris_img, y, x, pix):
    if x - pix > 0: start_x = x - pix
    else: start_x = 0
    if x + pix >= Harris_img.shape[1]: end_x = Harris_img.shape[0]
    else: end_x = x + pix
    if y - pix > 0: start_y = y - pix
    else: start_y = 0
    if y + pix >= Harris_img.shape[0]: end_y = Harris_img.shape[1]
    else: end_y = y + pix

    for i in range(start_x, end_x - 1):
        for j in range(start_y, end_y - 1):
            if j < 720 and i < 1280 and Harris_img[j][i][0]:
                return True
    return False

def pointDetect(Harris_img, img):
    # 求图像大小
    shape = img.shape
    height = shape[0]
    width = shape[1]

    upLeftX = 0
    upLeftY = 0
    downLeftX = 0
    downLeftY = 0
    upRightX = 0
    upRightY = 0
    downRightX = 0
    downRightY = 0
    step = 5

    # 求左上顶点
    for j in range(0, int(height/2), step):
        for i in range(0, int(width/2), step):
            if upLeftX == 0 and upLeftY == 0 and aroundCorner(Harris_img, j, i, step) and img[j][i][0] == 225:
                # print(Harris_img[j][i][0])
                upLeftX = i
                upLeftY = j
                break
        if upLeftX or upLeftY:
            break

    # 求右上顶点
    for j in range(0, int(height/2), step):
        for i in range(width-1, int(width/2), -step):
            if upRightX == 0 and upRightY == 0 and aroundCorner(Harris_img, j, i, step) and img[j][i][0] == 225:
                upRightX = i
                upRightY = j
                break
        if upRightX or upRightY:
            break

    # 求左下顶点
    for i in range(0, int(width/2), step):
        for j in range(int(height-1), int(height/2), -step):
            if downLeftX == 0 and downLeftY == 0 and aroundCorner(Harris_img, j, i, step) and img[j][i][0] == 225:
                downLeftX = i
                downLeftY = j
                break
        if downLeftX or downLeftY:
            break

    # 求右下顶点
    for i in range(width-1, int(width/2), -step):
        for j in range(height-1, int(height/2), -step):
            if downRightX == 0 and downRightY == 0 and aroundCorner(Harris_img, j, i, step) and img[j][i][0] == 225:
                downRightX = i
                downRightY = j
                break
        if downRightY or downRightY:
            break

    img[upLeftY][upLeftX][0] = 0
    img[upLeftY][upLeftX][1] = 255
    img[upLeftY][upLeftX][2] = 0

    print("左上坐标：", upLeftY, upLeftX)

    img[upRightY][upRightX][0] = 0
    img[upRightY][upRightX][1] = 255
    img[upRightY][upRightX][2] = 0

    print("右上坐标：", upRightY, upRightX)

    img[downRightY][downRightX][0] = 0
    img[downRightY][downRightX][1] = 255
    img[downRightY][downRightX][2] = 0

    print("右下坐标：", downRightY, downRightX)

    img[downLeftY][downLeftX][0] = 0
    img[downLeftY][downLeftX][1] = 255
    img[downLeftY][downLeftX][2] = 0

    print("左下坐标：", downLeftY, downLeftX)

    # 图像膨胀
    img = cv2.dilate(img, None)

    # 描边
    cv2.line(img, (upLeftX, upLeftY), (upRightX, upRightY), (255, 0, 0), 1)
    cv2.line(img, (upRightX, upRightY), (downRightX, downRightY), (255, 0, 0), 1)
    cv2.line(img, (downRightX, downRightY), (downLeftX, downLeftY), (255, 0, 0), 1)
    cv2.line(img, (downLeftX, downLeftY), (upLeftX, upLeftY), (255, 0, 0), 1)
    cv2.imshow('result', img)
    pts = np.float32([[upLeftX, upLeftY], [upRightX, upRightY], [downLeftX, downLeftY], [downRightX, downRightY]])
    return pts

# 填充地图
def MakeMap(img):
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    H_rows, W_cols = img.shape[:2]
    # print(H_rows, W_cols)
    Hrate = math.floor(H_rows / 6)
    Wrate = math.floor(W_cols / 6)
    # print(Hrate, Wrate)
    # 漫水填充
    # mask = np.zeros([H_rows+2, W_cols+2], np.uint8)
    # cv2.floodFill(img, mask, (30, 600), (0, 0, 225), cv2.FLOODFILL_FIXED_RANGE)
    # cv2.imshow("fill", img)
    # 封闭图像检测
    num_labels, labels, stats, centroids = cv2.connectedComponentsWithStats(gray, connectivity=8)
    # print(labels)
    road = 0
    map = np.zeros((6, 6), dtype=np.int)
    for i in range(0,6):
        for j in range(0,6):
            if img[i * Hrate + math.floor(Hrate / 2)][j * Wrate + math.floor(Wrate / 2)][2] == 225:
                map[i][j] = 1
    return map
    # print(map)
    # 标记区域
    # props = regionprops(labels)
    # im = Image.open(r"C:\Users\Jason\Desktop\fixed.png")
    # draw = ImageDraw.Draw(im)
    # for i in props:
    #     draw.text((i.centroid[-1], i.centroid[0]), "{0}".format(props.index(i)), fill=(255,225,225))#,font=ttfont)
    # im.show()
    # cv2.waitKey() & 0xFF == ord('q') # 等待按键


def find_road(map,location,start_x,satrt_y):
    for i in range(0,4):
        if location[start_y][start_x][i] == 0:
            if i==0:
                ret=find_way(start_x,start_y-1,1,location,map)
                if ret==-1:
                    location[start_y][start_x][i] = 1
            elif i==1:
                ret=find_way(start_x,start_y+1,0,location,map)
                if ret==-1:
                    location[start_y][start_x][i] = 1
            elif i==2:
                ret=find_way(start_x-1,start_y,3,location,map)
                if ret==-1:
                    location[start_y][start_x][i] = 1
            elif i==3:
                ret=find_way(start_x+1,start_y,2,location,map)
                if ret==-1:
                    location[start_y][start_x][i] = 1
    

    
def find_way(x,y,det,location,map):

    if map[y][x] ==0:
        return -1
    elif map[y][x]==3:
        return 1

    for i in range(0,4):
        if i==det:
            continue
        if location[y][x][i] == 0:
            if i==0:
                ret=find_way(x,y-1,1,location,map)
                if ret==-1:
                    location[y][x][i] = 1
            elif i==1:
                ret=find_way(x,y+1,0,location,map)
                if ret==-1:
                    location[y][x][i] = 1
            elif i==2:
                ret=find_way(x-1,y,3,location,map)
                if ret==-1:
                    location[y][x][i] = 1
            elif i==3:
                ret=find_way(x+1,y,2,location,map)
                if ret==-1:
                    location[y][x][i] = 1
    sum=location[y][x][0]+location[y][x][1]+location[y][x][2]+location[y][x][3]
    if sum==3:
        return -1
    
    return 1
#上：0 下：1 左：2 右：3

def make_action(location):
    action=[]
    x=start_x
    y=start_y
    while map[y][x]!=3:
        for i in range(0,4):
            if location[y][x][i]==0:
                if i==0:
                    action.append(i)
                    y=y-1
                    location[y][x][1]=1
                elif i==1:
                    action.append(i)
                    print(x,y)
                    y=y+1
                    location[y][x][0]=1
                elif i==2:
                    action.append(i)
                    x=x-1
                    location[y][x][3]=1
                elif i==3:
                    action.append(i)
                    x=x+1
                    location[y][x][2]=1
            if  map[y][x]==3:
                break
    return action


def make_location(map,location):
    temp=2
    up=0
    down=1
    left=2
    right=3
    for i in range(5,-1,-1):
        for j in range(5,-1,-1):
            if map[i][j]==1:
                if j==5:
                    if i==5:
                        if map[i-1][j]+map[i][j-1] ==1:
                            map[i][j]=temp
                            temp=temp+1
                        location[i][j][down]=location[i][j][right]=1
                    elif i==0:
                        if map[i+1][j]+map[i][j-1] ==1:
                            map[i][j]=temp
                            temp=temp+1
                        location[i][j][up]=location[i][j][right]=1
                    else:
                        if map[i+1][j]+map[i][j-1]+map[i-1][j] ==1:
                            map[i][j]=temp
                            temp=temp+1
                        location[i][j][right]=1
                elif j==0:
                    if i==5:
                        if map[i-1][j]+map[i][j+1] ==1:
                            map[i][j]=temp
                            temp=temp+1
                        location[i][j][down]=location[i][j][left]=1
                    elif i==0:
                        if map[i+1][j]+map[i][j+1] ==1:
                            map[i][j]=temp
                            temp=temp+1
                        location[i][j][up]=location[i][j][left]=1
                    else:
                        if map[i+1][j]+map[i][j+1]+map[i-1][j] ==1:
                            map[i][j]=temp
                            temp=temp+1  
                        location[i][j][left]=1
                elif i==5:
                    if map[i][j+1]+map[i][j-1]+map[i-1][j] ==1:
                        map[i][j]=temp
                        temp=temp+1
                    location[i][j][down]=1
                elif i==0:
                    if map[i][j+1]+map[i][j-1]+map[i+1][j] ==1:
                        map[i][j]=temp
                        temp=temp+1
                    location[i][j][up]=1

            else:
                location[i][j]=[1,1,1,1]           
    
    for i in range(5,-1,-1):
        for j in range(5,-1,-1):
            if map[i][j]==2:
                start_x=j
                start_y=i
                break
    if temp!=4:
        state=-1
    else:
        state=1
    return state,map,location,start_x,start_y

if __name__ == '__main__':
    np.set_printoptions(threshold=np.inf)
    image = cv2.imread(input_img_file)
    image = cv2.resize(image, (1280, 720), fx=1, fy=1, interpolation=cv2.INTER_NEAREST)
    image = Contours(image)
    imagebak = image.copy()
    H_rows, W_cols = image.shape[:2]
    
    # 四角检测
    pts1 = pointDetect(HarrisDetect(image), image)
    pts2 = np.float32([[0, 0],[W_cols,0],[0, H_rows],[W_cols,H_rows],])
    # 生成透视变换矩阵；进行透视变换
    M = cv2.getPerspectiveTransform(pts1, pts2)
    
    warp = cv2.warpPerspective(imagebak, M, (W_cols,H_rows))
    cv2.imshow("warp",warp)
    # 构建地图输出矩阵
    map = MakeMap(warp)
    print(map)

    #寻路
    location=np.zeros((6, 6,4), dtype=np.int)
    state,map,location,start_x,start_y = make_location(map,location)#标记起点终点并构建寻路数据
    if state!=-1:
        find_road(map,location,start_x,start_y)#寻路
        action=make_action(location)#根据路线构建行动
        print(action)

    cv2.waitKey(0)
    cv2.destroyAllWindows()