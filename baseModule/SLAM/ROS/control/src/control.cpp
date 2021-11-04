#include <ros/ros.h>
#include <serial/serial.h>
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/Imu.h"
#include "nav_msgs/Odometry.h"
#include <iostream>
#include <math.h>

// 数据类型定义
typedef signed char             int8_t;
typedef short int               int16_t;
typedef int                     int32_t;
typedef unsigned char           uint8_t;
typedef unsigned short int      uint16_t;
typedef unsigned int            uint32_t;

// 接收到订阅的消息后，会进入消息回调函数
// 坐标信息回调函数
void poseCallback(const geometry_msgs::twist::ConstPtr& msg);

// 视觉信息回调函数
void cvTaskCallback(const geometry_msgs::twist::ConstPtr& msg);

// 位置环PID控制
void pidControl_x(double start, double end);
void pidControl_y(double start, double end);

//创建一个serial类
serial::Serial sp;

int main(int argc, char **argv)
{
    // 初始化ROS节点
    ros::init(argc, argv, "position_control");
    // 创建节点句柄
    ros::NodeHandle n;

    // 创建timeout
    serial::Timeout time_out = serial::Timeout::simpleTimeout(100);
    // 设置要打开的串口名称
    sp.setPort("/dev/ttyTHS1");
    // 设置串口通信的波特率
    sp.setBaudrate(115200);
    // 串口设置timeout
    sp.setTimeout(time_out);

    try
    {   // 打开串口
        sp.open();
    }
    catch(serial::IOException& e)
    {
        ROS_ERROR_STREAM("Unable to open port.");
        return -1;
    }
    
    // 判断串口是否打开成功
    if(sp.isOpen())
    {
        ROS_INFO_STREAM("/dev/ttyTHS1 is opened.");
    }
    else
    {
        return -1;
    }

    // 创建一个Subscriber，订阅名为/t265_pose的topic，注册回调函数poseCallback
    ros::Subscriber pose_sub = n.subscribe("/t265_pose", 1000, poseCallback);
    ROS_INFO_STREAM("Subscrib /t265_pose");

    // 创建一个Subscriber， 订阅名为/cvTask/moveCommand, 注册回到函数cvTaskCallback
    ros::Subscriber cvTask_sub = n.subscribe("/cvTask/moveCommand", 1000, cvTaskCallback);
    ROS_INFO_STREAM("Subscrib /cvTask/moveCommand");

    // 设置执行频率
    ros::Rate loop_rate(50);

    // 循环等待回调函数
    ros::spin();
    return 0;
}

double t265_vxcms, t265_vycms, t265_vzcms, t265_pxcm, t265_pycm, t265_pzcm;
void poseCallback(const geometry_msgs::twist::ConstPtr& msg)
{
    //接收数据
    t265_vxcms = msg->linear.x;
    t265_vycms = msg->linear.y;
    t265_vzcms = msg->linear.z;
    t265_pxcm = msg->angular.x;
    t265_pycm = msg->angular.y;
    t265_pzcm = msg->angular.z;

    //发送串口
    short x_position_cm = (short)(t265_pxcm);
    short y_position_cm = (short)(t265_pycm);
    short z_position_cm = (short)(t265_pzcm);
    short x_velocity_cms = (short)(t265_vxcms);
    short y_velocity_cms = (short)(t265_vycms);
    short z_velocity_cms = (short)(t265_vzcms);

    uint8_t i = 0;
    uint8_t sumcheck = 0, add_on_check =0;
    uint8_t usartBuffer[100] = {0};
    
    usartBuffer[0] = 0xAA;
    usartBuffer[1] = 0x62;
    usartBuffer[2] = 0x91;
    usartBuffer[3] = 0x0C;
    //x_position_cm
    usartBuffer[5] = x_position_cm >> 8;
    usartBuffer[4] = x_position_cm - (usartBuffer[5] << 8);
    //y_position_cm
    usartBuffer[7] = y_position_cm >> 8;
    usartBuffer[6] = y_position_cm - (usartBuffer[7] << 8);
    //z_position_cm
    usartBuffer[9] = z_position_cm >> 8;
    usartBuffer[8] = z_position_cm - (usartBuffer[9] << 8);
    //x_velocity_cms
    usartBuffer[11] = x_velocity_cms >> 8;
    usartBuffer[10] = x_velocity_cms - (usartBuffer[11] << 8);
    //y_velocity_cms
    usartBuffer[13] = y_velocity_cms >> 8;
    usartBuffer[12] = y_velocity_cms - (usartBuffer[13] << 8);
    //z_velocity_cms
    usartBuffer[15] = z_velocity_cms >> 8;
    usartBuffer[14] = z_velocity_cms - (usartBuffer[15] << 8);

    for(i = 0; i<= 15; i++)
    {
        sumcheck += usartBuffer[i];
        add_on_check += sumcheck;
    }
    sumcheck %= 256;
    add_on_check %= 256;

    usartBuffer[16] = sumcheck;
    usartBuffer[17] = add_on_check;
    sp.write(usartBuffer, 18);
}

double cv_pxcm, cv_pycm, cv_pzcm;
void cvTaskCallback(const geometry_msgs::twist::ConstPtr& msg)
{
    const int pidThreshold = 5;
    static int state;
    cv_pxcm = msg->linear.x;
    cv_pycm = msg->linear.y;
    cv_pzcm = msg->linear.z;
    if(abs(cv_pxcm - t265_pxcm) > pidThreshold && state == 0)
    {
        pidControl_x(t265_pxcm, cv_pxcm);
    }
    else
    {
        state = 1;
    }
    if(abs(cv_pycm - t265_pycm) > pidThreshold && state == 1)
    {
        pidControl_y(t265_pycm, cv_pycm);
    }
    else
    {
        state = 0;
    }
}

const double P = 1, I = 0.01, D = 0.1;
void pidControl_x(double start, double end)
{
    const int maxSpeed = 20;
    static int dif, speed, dir;
    dif = end - start;
    speed = P * dif - D * t265_vxcms;
    if(speed > maxSpeed) speed = maxSpeed;

    //发送串口
    uint8_t i = 0;
    uint8_t sumcheck = 0, add_on_check =0;
    uint8_t usartBuffer[100] = {0};
    
    usartBuffer[0] = 0xAA;
    usartBuffer[1] = 0x62;
    usartBuffer[2] = 0x80;
    usartBuffer[3] = 0x0A;

    //dif
    usartBuffer[5] = abs(dif) >> 8;
    usartBuffer[4] = abs(dif) - (usartBuffer[5] << 8);

    //speed
    usartBuffer[7] = abs(speed) >> 8;
    usartBuffer[6] = abs(speed) - (usartBuffer[7] << 8);

    //dir
    if(speed > 0) dir = 0;
    else dir = 180;
    usartBuffer[9] = dir >> 8;
    usartBuffer[8] = dir - (usartBuffer[9] << 8);

    for(i = 0; i<= 9; i++)
    {
        sumcheck += usartBuffer[i];
        add_on_check += sumcheck;
    }
    sumcheck %= 256;
    add_on_check %= 256;

    usartBuffer[10] = sumcheck;
    usartBuffer[11] = add_on_check;
    sp.write(usartBuffer, 12);

}
void pidControl_y(double start, double end)
{
    const int maxSpeed = 20;
    static int dif, speed, dir;
    dif = end - start;
    speed = P * dif - D * t265_vycms;
    if(speed > maxSpeed) speed = maxSpeed;

    //发送串口
    uint8_t i = 0;
    uint8_t sumcheck = 0, add_on_check =0;
    uint8_t usartBuffer[100] = {0};
    
    usartBuffer[0] = 0xAA;
    usartBuffer[1] = 0x62;
    usartBuffer[2] = 0x80;
    usartBuffer[3] = 0x0A;

    //dif
    usartBuffer[5] = abs(dif) >> 8;
    usartBuffer[4] = abs(dif) - (usartBuffer[5] << 8);

    //speed
    usartBuffer[7] = abs(speed) >> 8;
    usartBuffer[6] = abs(speed) - (usartBuffer[7] << 8);

    //dir
    if(speed > 0) dir = 270;
    else dir = 90;
    usartBuffer[9] = dir >> 8;
    usartBuffer[8] = dir - (usartBuffer[9] << 8);

    for(i = 0; i<= 9; i++)
    {
        sumcheck += usartBuffer[i];
        add_on_check += sumcheck;
    }
    sumcheck %= 256;
    add_on_check %= 256;

    usartBuffer[10] = sumcheck;
    usartBuffer[11] = add_on_check;
    sp.write(usartBuffer, 12);
}
