#!/bin/bash

run_dir=`pwd`
cur_dt=`date +%Y-%m-%d:%H:%M:%S`

mkdir -p $run_dir'/OutputFiles/'

save_dir=$run_dir'/OutputFiles/'$cur_dt

vid_save_dir=$save_dir'/'
image_width='640'
image_height='480'
video_fps='30.0'
imageFormat='jpeg'

numCam='1' # 0 Put number of camera connected to the Odroid. 0 if no camera connected or dont want to record images
bIMU='1' # 0 if dont want to record IMU . 1 if want to record IMU
uartPort='/dev/ttyACM0'
baudRate='921600'
recordTime='0' ##in second. Put 0 if want to end the record by pressing 'ENTER' button


mkdir $save_dir
echo $save_dir
mkdir $save_dir'/camera1'
mkdir $save_dir'/camera2'

cd build
./recordVideoIMU $vid_save_dir $image_width $image_height $video_fps $numCam $imageFormat $uartPort $baudRate $recordTime $bIMU

