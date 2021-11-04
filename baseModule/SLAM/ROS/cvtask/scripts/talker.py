#!/usr/bin/env python
# license removed for brevity
import rospy
from std_msgs.msg import Int8
from geometry_msgs.msg import Twist
import cv2

# input_img_file = "/home/jason/cvtask/map1.jpg"
input_img_file = "csi://0"

def talker():
    pub = rospy.Publisher('cvTask/moveCommand', Twist, queue_size=100)
    command = Twist()
    command.linear.x = 1
    command.linear.y = 1
    rospy.init_node('cvTask', anonymous=True)
    rate = rospy.Rate(50) # 10hz
    while not rospy.is_shutdown():
        hello_str = "hello world %s" % rospy.get_time()
        rospy.loginfo(hello_str)
        pub.publish(command)
        rate.sleep()
 
if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass