#!/usr/bin/env python
#-*- coding: UTF-8 -*- 
# license removed for brevity
import rospy
from jetcam.csi_camera import CSICamera
from std_msgs.msg import Int8
import cv2
import time
import numpy as np

input_img_file = "csi://0"

# white or green
def isGreen(img):
    #is_center_green
    width=20
    ROI=[int(360-width/2),int(350-width/2),width,width] #x,y,h,w
    Green_threshold={'Lower': np.array([0, 70, 40]), 'Upper': np.array([65, 255, 255])}
    

    gs_img = cv2.GaussianBlur(img, (5, 5), 0)                   # 高斯模糊         
    hsv = cv2.cvtColor(gs_img, cv2.COLOR_BGR2HSV)                 # 转化成HSV图像
    # erode_hsv = cv2.erode(hsv, None, iterations=2)                  # 腐蚀 粗的变细
    inRange_hsv = cv2.inRange(hsv,Green_threshold['Lower'], Green_threshold['Upper'])
    # cv2.imshow("inRange",inRange_hsv)
    # print(inRange_hsv[360,360])

    count=0
    #255 wihte   0 black
    for i in range(ROI[2]):
        for j in range(ROI[3]):
            if inRange_hsv[ROI[0]+j,ROI[1]+i]==255 : 
                count=count+1
    cv2.rectangle(img, (ROI[0], ROI[1]), ((ROI[0] + ROI[2]), (ROI[1] + ROI[3])), (255, 0, 0), 2)
    # cv2.imshow("roi", img)
    if (count/(ROI[2]*ROI[3]))>0.9:
        return 1
    else:
        return 0
    
def talker():
    cap = CSICamera(capture_device=0, width=720, height=720)
    funcpub = rospy.Publisher('cvTask/funcCommand', Int8, queue_size=100)
    funcNum = Int8()
    rospy.init_node('cvTask', anonymous=True)
    rate = rospy.Rate(50) # 50hz
    # fourcc = cv2.VideoWriter_fourcc(*'XVID')
    # out = cv2.VideoWriter('output.avi',fourcc,20.0,(720,720))
    state = 0
    funcNum = 0
    funcTimes = 0
    time_task = rospy.Time.from_sec(time.time()).to_sec()
    while not rospy.is_shutdown():
        img = cap.read()
        img = cv2.flip(img, 1)
        funcNum = isGreen(img)
        # cv2.imshow("image", img)
        funcpub.publish(funcNum)
        print(funcNum)
        rate.sleep()
        kk = cv2.waitKey(1)
        if kk == ord('q'):  # 按下 q 键，退出
            break
    cap.release()
    # out.release()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass
