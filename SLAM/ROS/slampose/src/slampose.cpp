#include <ros/ros.h>
 #include "geometry_msgs/PoseStamped.h"
 
// 接收到订阅的消息后，会进入消息回调函数
void poseCallback(const geometry_msgs::PoseStamped::ConstPtr& msg)
 
{
    // 将接收到的消息打印出来
    ROS_INFO("SLAM Pose: \nx:%0.6f\ny:%0.6f\nz:%0.6f\n", msg->pose.position.x, msg->pose.position.y, msg->pose.position.z);
}
 
int main(int argc, char **argv)
 
{
   // 初始化ROS节点
    ros::init(argc, argv, "slam_pose_subscriber");
     // 创建节点句柄
    ros::NodeHandle n;
    // 创建一个Subscriber，订阅名为/orb_slam2_rgbd/pose的topic，注册回调函数poseCallback
    ros::Subscriber pose_sub = n.subscribe("/orb_slam2_rgbd/pose", 1000, poseCallback);
    printf("Subscrib /orb_slam2_rgbd/pose\n");
    // 循环等待回调函数
    ros::spin();
    return 0;
}