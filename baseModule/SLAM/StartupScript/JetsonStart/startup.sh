#! /bin/bash
#刷新ros环境
source /opt/ros/melodic/setup.bash
# 刷新功能包环境
source /home/jason/catkin_ws/devel/setup.bash
# 启动ROS
nohup roscore &
sleep 10
# 运行相机节点
nohup roslaunch realsense2_camera rs_t265.launch &
# 运行数据节点
nohup rosrun slampose slampose &
# 显示运动路径
nohup rosrun showpath showpath &
# 运行视觉节点
nohup python /home/jason/catkin_ws/src/cvtask/scripts/talker.py &
# 运行控制节点
nohup rosrun control control &
# wifi
nohup sudo usb_modeswitch -KW -v 0bda -p 1a2b &
# sleep 10
# cd /home/hcm/catkin_ws/bag
# nohup rosbag record -a &

