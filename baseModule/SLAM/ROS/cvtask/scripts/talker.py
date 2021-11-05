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


def talker():
    cap = CSICamera(capture_device=0, width=720, height=720)
    net = jetson.inference.detectNet("ssd-mobilenet-v2", argv=["--model=/home/jason/github/jetson-inference/python/training/detection/ssd/models/A/ssd-mobilenet.onnx", "--labels=/home/jason/github/jetson-inference/python/training/detection/ssd/models/A/labels.txt", "--input-blob=input_0", "--output-cvg=scores", "--output-bbox=boxes"], threshold = 0.5)
    pub = rospy.Publisher('cvTask/moveCommand', Twist, queue_size=100)
    funcpub = rospy.Publisher('cvTask/funcCommand', Int8, queue_size=100)
    funcNum = Int8()
    command = Twist()
    command.linear.x = 1
    command.linear.y = 1
    rospy.init_node('cvTask', anonymous=True)
    rate = rospy.Rate(50) # 50hz
    state = 1
    funcNum = -1
    time_task1 = rospy.Time.from_sec(time.time()).to_sec()
    while not rospy.is_shutdown():
        img = cap.read()
        img = cv2.flip(img, 1)
        # img = cv2.flip(img, 0)
        state, command, funcNum, time_task1 = findA(img, net, state, command, funcNum, time_task1)
        cv2.imshow("image", img)
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
