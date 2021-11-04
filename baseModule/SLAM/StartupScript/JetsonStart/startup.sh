#! /bin/bash
#刷新ros环境
source /opt/ros/melodic/setup.bash
# 刷新功能包环境
source /home/jason/catkin_ws/devel/setup.bash
# 启动ROS
nohup roscore &
sleep 10
# 运行相机节点
nohup roslaunch realsense2_camera rs_t265.launch enable_fisheye1:=true enable_fisheye2:=true &
#运行数据节点
nohup rosrun slampose slampose &
nohup rosrun showpath showpath &
# sleep 10
# cd /home/hcm/catkin_ws/bag
# nohup rosbag record -a &