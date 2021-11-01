#! /bin/bash
#刷新ros环境
source /opt/ros/melodic/setup.bash
# 刷新功能包环境
source /home/ubuntu/catkin_ws/devel/setup.bash
# 运行相机节点
nohup roslaunch realsense2_camera rs_t265.launch &

#运行数据节点
nohup rosrun slampose slampose &

