#!/bin/bash 
cd ~/catkin_ws
source ~/.bashrc
source devel/setup.bash
source /opt/ros/melodic/setup.bash
gnome-terminal -- bash -c "cd ~/catkin_ws;source ~/.bashrc;source devel/setup.bash;source /opt/ros/melodic/setup.bash;roscore"
sleep 5
gnome-terminal -- bash -c "cd ~/catkin_ws;source ~/.bashrc;source /opt/ros/melodic/setup.bash;source devel/setup.bash;roslaunch realsense2_camera rs_t265.launch"
gnome-terminal -- bash -c "cd ~/catkin_ws;source ~/.bashrc;source /opt/ros/melodic/setup.bash;source devel/setup.bash;rosrun slampose slampose"
