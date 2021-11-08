#!/usr/bin/env python
#-*- coding: UTF-8 -*- 
# license removed for brevity
import rospy
from jetcam.csi_camera import CSICamera
from std_msgs.msg import Int8
from geometry_msgs.msg import Twist
import cv2
# import jetson.inference
# import jetson.utils
import argparse
import sys
import time
import numpy as np

# parse the command line
# /home/jason/catkin_ws/src/cvtask/scripts/talker.py --network=ssd-mobilenet-v2 --model=/home/jason/github/jetson-inference/python/training/detection/ssd/models/A/ssd-mobilenet.onnx --labels=/home/jason/github/jetson-inference/python/training/detection/ssd/models/A/labels.txt --input-blob=input_0 --output-cvg=scores --output-bbox=boxes
# input_img_file = "/home/jason/cvtask/map1.jpg"

input_img_file = "csi://0"

# sub node
t265_px = 0.0
t265_py = 0.0
t265_pz = 0.0
t265_vx = 0.0
t265_vy = 0.0
def t265callback(msg):
    global t265_px
    global t265_py
    global t265_pz
    global t265_vx
    global t265_vy
    t265_vx = msg.linear.x
    t265_vy = msg.linear.y
    t265_px = msg.angular.x + 10
    t265_py = msg.angular.y - 15
    t265_pz = msg.angular.z
    # print(t265_px, t265_py, t265_pz)

# white or green
def isGreen(img):
    #is_center_green
    width=10
    ROI=[int(360-width/2),int(330-width/2),width,width] #x,y,h,w
    Green_threshold={'Lower': np.array([20, 72, 138]), 'Upper': np.array([60, 187, 198])}
    
    
    gs_img = cv2.GaussianBlur(img, (5, 5), 0)                   # 高斯模糊         
    hsv = cv2.cvtColor(gs_img, cv2.COLOR_BGR2HSV)                 # 转化成HSV图像
    erode_hsv = cv2.erode(hsv, None, iterations=2)                  # 腐蚀 粗的变细
    inRange_hsv = cv2.inRange(erode_hsv,Green_threshold['Lower'], Green_threshold['Upper'])
    # cv2.imshow("inRange",inRange_hsv)
    # print(inRange_hsv[360,360])

    count=0
    #255 wihte   0 black
    for i in range(ROI[2]):
        for j in range(ROI[3]):
            if inRange_hsv[ROI[0]+j,ROI[1]+i]==255 : 
                count=count+1

    if (count/(ROI[2]*ROI[3]))>0.9:
        return 1
    else:
        return 0
    
def talker():
    cap = CSICamera(capture_device=0, width=720, height=720)
    pub = rospy.Publisher('cvTask/moveCommand', Twist, queue_size=100)
    funcpub = rospy.Publisher('cvTask/funcCommand', Int8, queue_size=100)
    funcNum = Int8()
    command = Twist()
    command.linear.x = 0 
    command.linear.y = 0
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
        # out.write(img)
        command.linear.x = 0
        command.linear.y = 0
        command.linear.z = 0
        command.angular.x = 0
        command.angular.y = 0
        command.angular.z = 0
        # 等到到达巡航高度
        funcNum = isGreen(img)
        # cv2.imshow("image", img)
        pub.publish(command)
        funcpub.publish(funcNum)
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
