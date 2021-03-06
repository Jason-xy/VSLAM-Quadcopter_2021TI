#include <ros/ros.h>
#include <serial/serial.h>
#include "geometry_msgs/Twist.h"
#include "std_msgs/Int8.h"
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
void poseCallback(const geometry_msgs::Twist::ConstPtr& msg);

// 视觉信息回调函数
void cvTaskCallback(const geometry_msgs::Twist::ConstPtr& msg);

// 位置环PID控制
void pidControl_xy(int dif_x, int dif_y);

//功能控制
void funcCallback(const std_msgs::Int8::ConstPtr& msg);

//创建一个serial类
serial::Serial sp;
uint8_t t265_usartBuffer[100] = {0};
uint8_t xy_usartBuffer[100] = {0};
uint8_t func_usartBuffer[100] = {0};
uint8_t t265_usart_state = 0, xy_usart_state = 0, func_usart_state = 0;

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

    // 创建一个Subscriber， 订阅名为/cvTask/moveCommand, 注册回到函数cvTaskCallback
    ros::Subscriber cvTaskfunc_sub = n.subscribe("/cvTask/funcCommand", 1000, funcCallback);
    ROS_INFO_STREAM("Subscrib /cvTask/funcCommand");

    // 设置执行频率
    ros::Rate loop_rate(50);
    loop_rate.sleep();
    // 循环等待回调函数
    ros::spin();

    return 0;
}

double t265_vxcms, t265_vycms, t265_vzcms, t265_pxcm, t265_pycm, t265_pzcm;
void poseCallback(const geometry_msgs::Twist::ConstPtr& msg)
{
    //接收数据
    t265_vxcms = msg->linear.x;
    t265_vycms = msg->linear.y;
    t265_vzcms = msg->linear.z;
    t265_pxcm = msg->angular.x;
    t265_pycm = msg->angular.y;
    t265_pzcm = msg->angular.z;

    if(t265_usart_state == 0){
        //发送串口
        short x_position_cm = (short)(t265_pxcm);
        short y_position_cm = (short)(t265_pycm);
        short z_position_cm = (short)(t265_pzcm);
        short x_velocity_cms = (short)(t265_vxcms);
        short y_velocity_cms = (short)(t265_vycms);
        short z_velocity_cms = (short)(t265_vzcms);

        uint8_t i = 0;
        uint8_t sumcheck = 0, add_on_check =0;
        
        t265_usartBuffer[0] = 0xAA;
        t265_usartBuffer[1] = 0x62;
        t265_usartBuffer[2] = 0x91;
        t265_usartBuffer[3] = 0x0C;
        //x_position_cm
        t265_usartBuffer[5] = x_position_cm >> 8;
        t265_usartBuffer[4] = x_position_cm - (t265_usartBuffer[5] << 8);
        //y_position_cm
        t265_usartBuffer[7] = y_position_cm >> 8;
        t265_usartBuffer[6] = y_position_cm - (t265_usartBuffer[7] << 8);
        //z_position_cm
        t265_usartBuffer[9] = z_position_cm >> 8;
        t265_usartBuffer[8] = z_position_cm - (t265_usartBuffer[9] << 8);
        //x_velocity_cms
        t265_usartBuffer[11] = x_velocity_cms >> 8;
        t265_usartBuffer[10] = x_velocity_cms - (t265_usartBuffer[11] << 8);
        //y_velocity_cms
        t265_usartBuffer[13] = y_velocity_cms >> 8;
        t265_usartBuffer[12] = y_velocity_cms - (t265_usartBuffer[13] << 8);
        //z_velocity_cms
        t265_usartBuffer[15] = z_velocity_cms >> 8;
        t265_usartBuffer[14] = z_velocity_cms - (t265_usartBuffer[15] << 8);

        for(i = 0; i<= 15; i++)
        {
            sumcheck += t265_usartBuffer[i];
            add_on_check += sumcheck;
        }
        sumcheck %= 256;
        add_on_check %= 256;

        t265_usartBuffer[16] = sumcheck;
        t265_usartBuffer[17] = add_on_check;

        sp.write(t265_usartBuffer, 18);
    }
}

double cv_pxcm, cv_pycm, cv_pzcm;
void cvTaskCallback(const geometry_msgs::Twist::ConstPtr& msg)
{
    const int pidThreshold = 5;
    static int state, dif_x, dif_y;
    // if (msg->linear.x == 0 && msg->linear.y == 0 && msg->linear.z == 0)
    // {
    //     dif_x = msg->angular.x;
    //     dif_y = msg->angular.y;
    //     ROS_INFO("[dif_x]: %d\t [dif_y]: %d");
    //     if(abs(dif_x) > pidThreshold && state == 0)
    //     {
    //         pidControl_x(dif_x);
    //     }
    //     else
    //     {
    //         pidControl_x(0);
    //         state = 1;
    //     }
    //     if(abs(dif_y) > pidThreshold && state == 1)
    //     {
    //         pidControl_y(dif_y);
    //     }
    //     else
    //     {
    //         pidControl_y(0);
    //         state = 0;
    //     }
    // }
    // else
    // {
    //     cv_pxcm = msg->linear.x;
    //     cv_pycm = msg->linear.y;
    //     cv_pzcm = msg->linear.z;
    //     ROS_INFO("[cv_pxcm]: %d\t [cv_pycm]: %d\t [cv_pzcm]: %d");
    //     pidControl_xy(cv_pxcm - t265_pxcm, cv_pycm - t265_pycm);
    //     // if(abs(cv_pxcm - t265_pxcm) > pidThreshold && state == 0)
    //     // {
    //     //     pidControl_x(cv_pxcm - t265_pxcm);
    //     // }
    //     // else
    //     // {
    //     //     state = 1;
    //     // }
    //     // if(abs(cv_pycm - t265_pycm) > pidThreshold && state == 1)
    //     // {
    //     //     pidControl_y(cv_pycm - t265_pycm);
    //     // }
    //     // else
    //     // {
    //     //     state = 0;
    //     // }
    // }
}

void pidControl_xy(int dif_x, int dif_y)
{
    const int maxSpeed = 20, P = 0.2, D= 0.1;
    static int speed, dir, dif;
    dif = sqrt(dif_x * dif_x + dif_y * dif_y);
    speed = P * dif - D * sqrt(t265_vxcms * t265_vxcms + t265_vycms * t265_vycms);
    if(dif_y > 0 && dif_x > 0)
        dir = 360 - atan((float)dif_y / (float)dif_x) * 57.3;
    else if(dif_y > 0 && dif_x < 0)
        dir = 180 - atan((float)dif_y / (float)dif_x) * 57.3;
    else if(dif_y < 0 && dif_x > 0)
        dir = -atan((float)dif_y / (float)dif_x) * 57.3;
    else if(dif_y < 0 && dif_x < 0)
        dir = 180 - atan((float)dif_y / (float)dif_x) * 57.3;
    if(speed > maxSpeed) speed = maxSpeed;

    if(xy_usart_state == 0)
    {
        //发送串口
        uint8_t i = 0;
        uint8_t sumcheck = 0, add_on_check =0;
        
        xy_usartBuffer[0] = 0xAA;
        xy_usartBuffer[1] = 0x62;
        xy_usartBuffer[2] = 0x80;
        xy_usartBuffer[3] = 0x06;

        //dif
        xy_usartBuffer[5] = abs(dif) >> 8;
        xy_usartBuffer[4] = abs(dif) - (xy_usartBuffer[5] << 8);

        //speed
        xy_usartBuffer[7] = abs(speed) >> 8;
        xy_usartBuffer[6] = abs(speed) - (xy_usartBuffer[7] << 8);

        //dir
        xy_usartBuffer[9] = dir >> 8;
        xy_usartBuffer[8] = dir - (xy_usartBuffer[9] << 8);

        for(i = 0; i<= 9; i++)
        {
            sumcheck += xy_usartBuffer[i];
            add_on_check += sumcheck;
        }
        sumcheck %= 256;
        add_on_check %= 256;

        xy_usartBuffer[10] = sumcheck;
        xy_usartBuffer[11] = add_on_check;

        ROS_INFO("[Dir XY]:dif %d\tspeed %d\t dir %d\t", dif, speed, dir);
        xy_usart_state = 1;
    }
}

//0:激光 1：LED
void funcCallback(const std_msgs::Int8::ConstPtr& msg){
    static int8_t funcNum;
    funcNum = msg->data;

    if(func_usart_state == 0)
    {
        //发送串口
        uint8_t i = 0;
        uint8_t sumcheck = 0, add_on_check =0;
        
        func_usartBuffer[0] = 0xAA;
        func_usartBuffer[1] = 0x62;
        func_usartBuffer[2] = 0x92;
        func_usartBuffer[3] = 0x01;

        func_usartBuffer[4] = funcNum;

        for(i = 0; i<= 4; i++)
        {
            sumcheck += func_usartBuffer[i];
            add_on_check += sumcheck;
        }
        sumcheck %= 256;
        add_on_check %= 256;

        func_usartBuffer[5] = sumcheck;
        func_usartBuffer[6] = add_on_check;
        
        ROS_INFO("[FuncNum]: %d",funcNum);
        sp.write(func_usartBuffer, 7);
    }
}

