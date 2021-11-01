#/bin/bash
#startup script
time=`data`
echo "$time start to workflow"

cd /home/hcm/workspace
python3 depth_with_color.py >> workflow.log