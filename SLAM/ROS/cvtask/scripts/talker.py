#!/usr/bin/env python
# license removed for brevity
import rospy
from std_msgs.msg import Int8
import cv2

def mapPic2Array(img):
    mapArray = [[]]
    return mapArray
 
def talker():
    pub = rospy.Publisher('cvTask/moveCommand', Int8, queue_size=10)
    rospy.init_node('cvTask', anonymous=True)
    rate = rospy.Rate(10) # 10hz
    while not rospy.is_shutdown():
        hello_str = "hello world %s" % rospy.get_time()
        rospy.loginfo(hello_str)
        pub.publish(hello_str)
        rate.sleep()
 
if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass