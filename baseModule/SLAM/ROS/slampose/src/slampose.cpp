#include <ros/ros.h>
#include <serial/serial.h>
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/Imu.h"
#include "nav_msgs/Odometry.h"
#include <iostream>
#include <math.h>
#include <Eigen/Eigen> 
#include <Eigen/Geometry>  
#include <Eigen/Core>  
#include <vector>
using namespace Eigen;  
// /camera/accel/sample /camera/gyro/sample sensor_msgs/Imu 
/*
std_msgs/Header header
  uint32 seq
  time stamp
  string frame_id
geometry_msgs/Quaternion orientation
  float64 x
  float64 y
  float64 z
  float64 w
float64[9] orientation_covariance
geometry_msgs/Vector3 angular_velocity
  float64 x
  float64 y
  float64 z
float64[9] angular_velocity_covariance
geometry_msgs/Vector3 linear_acceleration
  float64 x
  float64 y
  float64 z
float64[9] linear_acceleration_covariance
*/
// /camera/odom/sample nav_msgs/Odometry
/*
std_msgs/Header header
  uint32 seq
  time stamp
  string frame_id
string child_frame_id
geometry_msgs/PoseWithCovariance pose
  geometry_msgs/Pose pose
    geometry_msgs/Point position
      float64 x
      float64 y
      float64 z
    geometry_msgs/Quaternion orientation
      float64 x
      float64 y
      float64 z
      float64 w
  float64[36] covariance
geometry_msgs/TwistWithCovariance twist
  geometry_msgs/Twist twist
    geometry_msgs/Vector3 linear
      float64 x
      float64 y
      float64 z
    geometry_msgs/Vector3 angular
      float64 x
      float64 y
      float64 z
  float64[36] covariance
*/

typedef signed char             int8_t;
typedef short int               int16_t;
typedef int                     int32_t;

typedef unsigned char           uint8_t;
typedef unsigned short int      uint16_t;
typedef unsigned int            uint32_t;

#define RAD_TO_ANGLE 57.2957795f
// //创建一个serial类
// serial::Serial sp;
Eigen::Quaterniond q;
double pitch, roll, yaw;
double now_time, pre_time, dif_time;
double now_pxcm, now_pycm, now_pzcm;
double pre_pxcm, pre_pycm, pre_pzcm;
double dif_pxcm, dif_pycm, dif_pzcm;
double v_xcms = 0, v_ycms = 0, v_zcms = 0;
double v_xcmsE = 0, v_ycmsE = 0, v_zcmsE = 0;
geometry_msgs::Twist t265_msg;
ros::Publisher pub_t265;
//数据滤波
#define a  0.7 //低通滤波系数
#define SCALE 0.9
// 接收到订阅的消息后，会进入消息回调函数
//坐标信息回调函数
void poseCallback(const nav_msgs::Odometry::ConstPtr& msg)
{
    //计算坐标
    now_pxcm = msg->pose.pose.position.x * 100 * 0.87;
    now_pycm = msg->pose.pose.position.y * 100 * 0.89;
    now_pzcm = msg->pose.pose.position.z * 100 * SCALE;
    //获取四元数
    q.w() = msg->pose.pose.orientation.w;
    q.x() = msg->pose.pose.orientation.x;
    q.y() = msg->pose.pose.orientation.y;
    q.z() = msg->pose.pose.orientation.z;
    // 计算欧拉角
    pitch = asin(2.0f * (q.w() * q.y() - q.x() * q.z())) * RAD_TO_ANGLE;
    roll = -atan2(2.0f * (q.w() * q.x() + q.y() * q.z()), q.w() * q.w() - q.x() * q.x() - q.y() * q.y() + q.z() * q.z()) * RAD_TO_ANGLE;
    yaw = atan2(2.0f * (q.w() * q.z() + q.x() * q.y()), q.w() * q.w() + q.x() * q.x() - q.y() * q.y() - q.z() * q.z()) * RAD_TO_ANGLE;
    ROS_INFO("SLAM Pose Angle: \nroll:%0.6f\npitch:%0.6f\nyaw:%0.6f\n", roll, pitch, yaw);
    //验证数据置信度
    if(msg->pose.covariance[0] >= 0.05){
      //数据无效，输出0
      now_pxcm = pre_pxcm;
      now_pycm = pre_pycm;
      now_pzcm = pre_pzcm;
    }
    // 将接收到的消息打印出来
    ROS_INFO("SLAM Pose: \nx:%0.6f\ny:%0.6f\nz:%0.6f\n", now_pxcm, now_pycm, now_pzcm);
    dif_pxcm = now_pxcm - pre_pxcm;
    dif_pycm = now_pycm - pre_pycm;
    dif_pzcm = now_pzcm - pre_pzcm;

    //更新数据
    pre_pxcm = now_pxcm;
    pre_pycm = now_pycm;
    pre_pzcm = now_pzcm;

    //计算时间差
    now_time = msg->header.stamp.nsec;
    dif_time = (now_time - pre_time)/1000000000.0f;
    pre_time = now_time;
    // printf("sec:%lf\n", msg->header.stamp.nsec);
    // printf("dif:%lf\n", dif_time);

    //计算速度cm/s 低通滤波
    v_xcmsE = (1 - a) * (dif_pxcm / dif_time) + a * v_xcmsE;
    v_ycmsE = (1 - a) * (dif_pycm / dif_time) + a * v_ycmsE;
    v_zcmsE = (1 - a) * (dif_pzcm / dif_time) + a * v_zcmsE;

    //坐标变换地理坐标系到机体坐标系
    yaw = yaw / RAD_TO_ANGLE;
    v_xcms = v_xcmsE * cos(yaw) + v_ycmsE * sin(yaw);
    v_ycms = -v_xcmsE * sin(yaw) + v_ycmsE * cos(yaw);


    //限幅
    if(v_xcms > 100) v_xcms = 100;
    if(v_ycms > 100) v_ycms = 100;
    if(v_zcms > 100) v_zcms = 100;
    if(v_xcms < -100) v_xcms = -100;
    if(v_ycms < -100) v_ycms = -100;
    if(v_zcms < -100) v_zcms = -100;
    ROS_INFO("SLAM Velocity: \nx:%0.6f\ny:%0.6f\nz:%0.6f\n", v_xcms, v_ycms, v_zcms);

    //发布数据
    t265_msg.linear.x = v_xcms;
    t265_msg.linear.y = v_ycms;
    t265_msg.linear.z = v_zcms;
    t265_msg.angular.x = now_pxcm;
    t265_msg.angular.y = now_pycm;
    t265_msg.angular.z = now_pzcm;
    pub_t265.publish(t265_msg);
    
    // //发送串口
    // short x_position_cm = (short)(now_pxcm);
    // short y_position_cm = (short)(now_pycm);
    // short z_position_cm = (short)(now_pzcm);
    // short x_velocity_cms = (short)(v_xcms);
    // short y_velocity_cms = (short)(v_ycms);
    // short z_velocity_cms = (short)(v_zcms);

    // uint8_t i = 0;
    // uint8_t sumcheck = 0, add_on_check =0;
    // uint8_t usartBuffer[100] = {0};
    
    // usartBuffer[0] = 0xAA;
    // usartBuffer[1] = 0x62;
    // usartBuffer[2] = 0x91;
    // usartBuffer[3] = 0x0C;
    // //x_position_cm
    // usartBuffer[5] = x_position_cm >> 8;
    // usartBuffer[4] = x_position_cm - (usartBuffer[5] << 8);
    // //y_position_cm
    // usartBuffer[7] = y_position_cm >> 8;
    // usartBuffer[6] = y_position_cm - (usartBuffer[7] << 8);
    // //z_position_cm
    // usartBuffer[9] = z_position_cm >> 8;
    // usartBuffer[8] = z_position_cm - (usartBuffer[9] << 8);
    // //x_velocity_cms
    // usartBuffer[11] = x_velocity_cms >> 8;
    // usartBuffer[10] = x_velocity_cms - (usartBuffer[11] << 8);
    // //y_velocity_cms
    // usartBuffer[13] = y_velocity_cms >> 8;
    // usartBuffer[12] = y_velocity_cms - (usartBuffer[13] << 8);
    // //z_velocity_cms
    // usartBuffer[15] = z_velocity_cms >> 8;
    // usartBuffer[14] = z_velocity_cms - (usartBuffer[15] << 8);

    // for(i = 0; i<= 15; i++)
    // {
    //     sumcheck += usartBuffer[i];
    //     add_on_check += sumcheck;
    // }
    // sumcheck %= 256;
    // add_on_check %= 256;

    // usartBuffer[16] = sumcheck;
    // usartBuffer[17] = add_on_check;

    // sp.write(usartBuffer, 18);
}
 
int main(int argc, char **argv)
{
   // 初始化ROS节点
    ros::init(argc, argv, "slam_pose_transfer");
     // 创建节点句柄
    ros::NodeHandle n;

    // //创建timeout
    // serial::Timeout time_out = serial::Timeout::simpleTimeout(100);
    // //设置要打开的串口名称
    // sp.setPort("/dev/ttyTHS1");
    // //设置串口通信的波特率
    // sp.setBaudrate(115200);
    // //串口设置timeout
    // sp.setTimeout(time_out);

    // try
    // {   //打开串口
    //     sp.open();
    // }
    // catch(serial::IOException& e)
    // {
    //     ROS_ERROR_STREAM("Unable to open port.");
    //     return -1;
    // }
    
    // //判断串口是否打开成功
    // if(sp.isOpen())
    // {
    //     ROS_INFO_STREAM("/dev/ttyTHS1 is opened.");
    // }
    // else
    // {
    //     return -1;
    // }

    // 创建一个Subscriber，订阅名为/camera/odom/sample的topic，注册回调函数poseCallback
    ros::Subscriber pose_sub = n.subscribe("/camera/odom/sample", 1000, poseCallback);
    printf("Subscrib /camera/odom/sample\n");

    //创建一个publisher，创建/t265_pose的topic
    ros::NodeHandle m;
    pub_t265 = m.advertise<geometry_msgs::Twist>("/t265_pose", 1000);
    printf("Publish /t265_pose\n");

    // 设置执行频率
    ros::Rate loop_rate(50);
    // 循环等待回调函数
    ros::spin();
    return 0;
}
