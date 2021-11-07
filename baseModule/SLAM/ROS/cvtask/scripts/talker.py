#!/usr/bin/env python
#-*- coding: UTF-8 -*- 
# license removed for brevity
import rospy
from jetcam.csi_camera import CSICamera
from std_msgs.msg import Int8
from geometry_msgs.msg import Twist
import cv2
import jetson.inference
import jetson.utils
import argparse
import sys
import time

# parse the command line
# /home/jason/catkin_ws/src/cvtask/scripts/talker.py --network=ssd-mobilenet-v2 --model=/home/jason/github/jetson-inference/python/training/detection/ssd/models/A/ssd-mobilenet.onnx --labels=/home/jason/github/jetson-inference/python/training/detection/ssd/models/A/labels.txt --input-blob=input_0 --output-cvg=scores --output-bbox=boxes

# input_img_file = "/home/jason/cvtask/map1.jpg"
input_img_file = "csi://0"

# sub node
t265_px = 0.0
t265_py = 0.0
t265_pz = 0.0
def t265callback(msg):
    global t265_px
    global t265_py
    global t265_pz
    t265_px = msg.angular.x
    t265_py = msg.angular.y
    t265_pz = msg.angular.z
    # print(t265_px, t265_py, t265_pz)

# white or gray
def isGreen():
    return True

# findA task
def findA(img, net, state, command, funcNum, time_task1):
    cudaimg = jetson.utils.cudaFromNumpy(img)
    command.linear.x = 0
    command.linear.y = 0
    command.linear.z = 0
    command.angular.x = 0
    command.angular.y = 0
    command.angular.z = 0
    if state == 1:
        detections = net.Detect(cudaimg)
        for detection in detections:
            if detection.Area  < 155520 and detection.ClassID == 1:
                dif_x = 720 / 2 - detection.Center[0]
                dif_y = 720 / 2 - detection.Center[1]
                command.linear.x = 0
                command.linear.y = 0
                command.linear.z = 0
                # 与东北天坐标系对应相反
                command.angular.x = dif_y / 10
                command.angular.y = dif_x / 10
                command.angular.z = 0
                
                print(dif_x, dif_y)
                if abs(dif_x) < 50 and abs(dif_y) < 50:
                    if rospy.Time.from_sec(time.time()).to_sec() - time_task1 > 2:
                        funcNum = 0
                        state = 2
                else:
                    time_task1 = rospy.Time.from_sec(time.time()).to_sec()

    return state, command, funcNum, time_task1

# wait start
def waitStart(command, state, time_task):
    # switch to next state
    print(rospy.Time.from_sec(time.time()).to_sec() - time_task)
    if rospy.Time.from_sec(time.time()).to_sec() - time_task > 2:
        state = 1
        return command, state, time_task
    if abs(t265_pz - 150) > 5:
        time_task = rospy.Time.from_sec(time.time()).to_sec()
    return command, state, time_task

# direct fly to A
def fly2field(x, y, z, command, state, time_task, funcNum, funcTimes):
    # continue send command
    if abs(t265_px - x) > 5 or abs(t265_py - y) > 5 or abs(t265_pz - z) > 5:
        time_task = rospy.Time.from_sec(time.time()).to_sec()
        command.linear.x = x
        command.linear.y = y
        command.linear.z = z
        command.angular.x = 0
        command.angular.y = 0
        command.angular.z = 0

    # switch to next state
    if rospy.Time.from_sec(time.time()).to_sec() - time_task > 2:
        if isGreen() and funcTimes < 2:
            funcNum = 0
            funcTimes += 1
        else:
            state += 1
            funcTimes = 0
        return command, state, time_task, funcNum, funcTimes

    return command, state, time_task, funcNum, funcTimes
    
def talker():
    cap = CSICamera(capture_device=0, width=720, height=720)
    # net = jetson.inference.detectNet("ssd-mobilenet-v2", argv=["--model=/home/jason/github/jetson-inference/python/training/detection/ssd/models/A/ssd-mobilenet.onnx", "--labels=/home/jason/github/jetson-inference/python/training/detection/ssd/models/A/labels.txt", "--input-blob=input_0", "--output-cvg=scores", "--output-bbox=boxes"], threshold = 0.5)
    pub = rospy.Publisher('cvTask/moveCommand', Twist, queue_size=100)
    funcpub = rospy.Publisher('cvTask/funcCommand', Int8, queue_size=100)
    sub = rospy.Subscriber('/t265_pose', Twist, t265callback)
    funcNum = Int8()
    command = Twist()
    command.linear.x = 0
    command.linear.y = 0
    rospy.init_node('cvTask', anonymous=True)
    rate = rospy.Rate(50) # 50hz
    state = 0
    funcNum = 1
    time_task = rospy.Time.from_sec(time.time()).to_sec()
    while not rospy.is_shutdown():
        img = cap.read()
        img = cv2.flip(img, 1)
        command.linear.x = 0
        command.linear.y = 0
        command.linear.z = 0
        command.angular.x = 0
        command.angular.y = 0
        command.angular.z = 0
        funcNum = 1
        # img = cv2.flip(img, 0)
        # state, command, funcNum, time_task1 = findA(img, net, state, command, funcNum, time_task1)
        # 等到到达巡航高度
        if state == 0:
            command, state, time_task = waitStart(command, state, time_task)
        # 飞行至A点（21）
        elif state == 1:
            command, state, time_task, funcNum, funcTimes = fly2field(50, 200, 150, command, state, time_task, funcNum, funcTimes)
        # 飞行至 28
        elif state == 2:
            command, state, time_task, funcNum, funcTimes = fly2field(50, 250, 150, command, state, time_task, funcNum, funcTimes)
        # 飞行至 27
        elif state == 3:
            command, state, time_task, funcNum, funcTimes = fly2field(100, 250, 150, command, state, time_task, funcNum, funcTimes)
        # 飞行至 26
        elif state == 4:
            command, state, time_task, funcNum, funcTimes = fly2field(150, 250, 150, command, state, time_task, funcNum, funcTimes)
        # 飞行至 25
        elif state == 5:
            command, state, time_task, funcNum, funcTimes = fly2field(200, 250, 150, command, state, time_task, funcNum, funcTimes)   
        # 飞行至 24
        elif state == 6:
            command, state, time_task, funcNum, funcTimes = fly2field(250, 250, 150, command, state, time_task, funcNum, funcTimes)
        # 飞行至 23
        elif state == 7:
            command, state, time_task, funcNum, funcTimes = fly2field(300, 250, 150, command, state, time_task, funcNum, funcTimes)
        # 飞行至 22
        elif state == 8:
            command, state, time_task, funcNum, funcTimes = fly2field(350, 250, 150, command, state, time_task, funcNum, funcTimes)
        # 飞行至 15
        elif state == 9:
            command, state, time_task, funcNum, funcTimes = fly2field(350, 200, 150, command, state, time_task, funcNum, funcTimes)
        # 飞行至 11
        elif state == 10:
            command, state, time_task, funcNum, funcTimes = fly2field(350, 150, 150, command, state, time_task, funcNum, funcTimes)
        # 飞行至 白1
        elif state == 11:
            command, state, time_task, funcNum, funcTimes = fly2field(350, 100, 150, command, state, time_task, funcNum, funcTimes)
        # 飞行至 5
        elif state == 12:
            command, state, time_task, funcNum, funcTimes = fly2field(350, 50, 150, command, state, time_task, funcNum, funcTimes)
        # 飞行至 1
        elif state == 13:
            command, state, time_task, funcNum, funcTimes = fly2field(350, 0, 150, command, state, time_task, funcNum, funcTimes)
        # 飞行至 2
        elif state == 14:
            command, state, time_task, funcNum, funcTimes = fly2field(300, 0, 150, command, state, time_task, funcNum, funcTimes)
        # 飞行至 6
        elif state == 15:
            command, state, time_task, funcNum, funcTimes = fly2field(300, 50, 150, command, state, time_task, funcNum, funcTimes)
        # 飞行至 白2
        elif state == 16:
            command, state, time_task, funcNum, funcTimes = fly2field(300, 100, 150, command, state, time_task, funcNum, funcTimes)
        # 飞行至 12
        elif state == 17:
            command, state, time_task, funcNum, funcTimes = fly2field(300, 150, 150, command, state, time_task, funcNum, funcTimes)
        # 飞行至 16
        elif state == 18:
            command, state, time_task, funcNum, funcTimes = fly2field(300, 200, 150, command, state, time_task, funcNum, funcTimes)
        # 飞行至 17
        elif state == 19:
            command, state, time_task, funcNum, funcTimes = fly2field(250, 200, 150, command, state, time_task, funcNum, funcTimes)
        # 飞行至 13
        elif state == 20:
            command, state, time_task, funcNum, funcTimes = fly2field(250, 150, 150, command, state, time_task, funcNum, funcTimes)
        # 飞行至 9
        elif state == 21:
            command, state, time_task, funcNum, funcTimes = fly2field(250, 100, 150, command, state, time_task, funcNum, funcTimes)
        # 飞行至 7
        elif state == 22:
            command, state, time_task, funcNum, funcTimes = fly2field(250, 50, 150, command, state, time_task, funcNum, funcTimes)
        # 飞行至 3
        elif state == 23:
            command, state, time_task, funcNum, funcTimes = fly2field(250, 0, 150, command, state, time_task, funcNum, funcTimes)
        # 飞行至 4
        elif state == 24:
            command, state, time_task, funcNum, funcTimes = fly2field(200, 0, 150, command, state, time_task, funcNum, funcTimes)
        # 飞行至 8
        elif state == 25:
            command, state, time_task, funcNum, funcTimes = fly2field(200, 50, 150, command, state, time_task, funcNum, funcTimes)
        # 飞行至 10
        elif state == 26:
            command, state, time_task, funcNum, funcTimes = fly2field(200, 100, 150, command, state, time_task, funcNum, funcTimes)
        # 飞行至 14
        elif state == 27:
            command, state, time_task, funcNum, funcTimes = fly2field(200, 150, 150, command, state, time_task, funcNum, funcTimes)
        # 飞行至 18
        elif state == 28:
            command, state, time_task, funcNum, funcTimes = fly2field(200, 200, 150, command, state, time_task, funcNum, funcTimes)
        # 飞行至 19
        elif state == 29:
            command, state, time_task, funcNum, funcTimes = fly2field(150, 200, 150, command, state, time_task, funcNum, funcTimes)
        # 飞行至 20
        elif state == 30:
            command, state, time_task, funcNum, funcTimes = fly2field(100, 200, 150, command, state, time_task, funcNum, funcTimes)
        # 飞行至 21
        elif state == 31:
            command, state, time_task, funcNum, funcTimes = fly2field(50, 200, 150, command, state, time_task, funcNum, funcTimes)
        else:
            break
        # cv2.imshow("image", img)
        pub.publish(command)
        funcpub.publish(funcNum)
        rate.sleep()

        kk = cv2.waitKey(1)
        if kk == ord('q'):  # 按下 q 键，退出
            break
    cap.release()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass
