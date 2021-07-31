#include <ros/ros.h>
#include <serial/serial.h>
#include "geometry_msgs/PoseStamped.h"
#include <iostream>

typedef signed char             int8_t;
typedef short int               int16_t;
typedef int                     int32_t;

typedef unsigned char           uint8_t;
typedef unsigned short int      uint16_t;
typedef unsigned int            uint32_t;

//创建一个serial类
serial::Serial sp;

// 接收到订阅的消息后，会进入消息回调函数
void poseCallback(const geometry_msgs::PoseStamped::ConstPtr& msg)
{
    // 将接收到的消息打印出来
    ROS_INFO("SLAM Pose: \nx:%0.6f\ny:%0.6f\nz:%0.6f\n", msg->pose.position.x, msg->pose.position.y, msg->pose.position.z);
    //发送串口
    short x_position_cm = (short)(msg->pose.position.x * 100);
    short y_position_cm = (short)(msg->pose.position.y * 100);
    short z_position_cm = (short)(msg->pose.position.z * 100);

    uint8_t i = 0;
    uint8_t sumcheck = 0, add_on_check =0;
    uint8_t usartBuffer[100] = {0};
    
    usartBuffer[0] = 0xAA;
    usartBuffer[1] = 0x62;
    usartBuffer[2] = 0x91;
    usartBuffer[3] = 0x06;
    //x_position_cm
    usartBuffer[5] = x_position_cm >> 8;
    usartBuffer[4] = x_position_cm - (usartBuffer[5] << 8);
    //y_position_cm
    usartBuffer[7] = y_position_cm >> 8;
    usartBuffer[6] = y_position_cm - (usartBuffer[7] << 8);
    //z_position_cmh
    usartBuffer[9] = z_position_cm >> 8;
    usartBuffer[8] = z_position_cm - (usartBuffer[9] << 8);

    for(i = 0; i<= 9; i++)
    {
        sumcheck += usartBuffer[i];
        add_on_check += sumcheck;
    }
    sumcheck %= 256;
    add_on_check %= 256;

    usartBuffer[10] = sumcheck;
    usartBuffer[11] = add_on_check;
    //发送数据
    sp.write(usartBuffer, 12);
}
 
int main(int argc, char **argv)
{
   // 初始化ROS节点
    ros::init(argc, argv, "slam_pose_transfer");
     // 创建节点句柄
    ros::NodeHandle n;

    //创建timeout
    serial::Timeout time_out = serial::Timeout::simpleTimeout(100);
    //设置要打开的串口名称
    sp.setPort("/dev/ttyTHS1");
    //设置串口通信的波特率
    sp.setBaudrate(115200);
    //串口设置timeout
    sp.setTimeout(time_out);

    try
    {   //打开串口
        sp.open();
    }
    catch(serial::IOException& e)
    {
        ROS_ERROR_STREAM("Unable to open port.");
        return -1;
    }
    
    //判断串口是否打开成功
    if(sp.isOpen())
    {
        ROS_INFO_STREAM("/dev/ttyUSB0 is opened.");
    }
    else
    {
        return -1;
    }

    // 创建一个Subscriber，订阅名为/orb_slam2_rgbd/pose的topic，注册回调函数poseCallback
    ros::Subscriber pose_sub = n.subscribe("/orb_slam2_rgbd/pose", 1000, poseCallback);
    printf("Subscrib /orb_slam2_rgbd/pose\n");
    // 循环等待回调函数
    ros::spin();
    return 0;
}