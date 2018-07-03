#!/bin/bash

run_dir=`pwd`
cur_dt=`date +%Y-%m-%d:%H:%M:%S`

mkdir -p $run_dir'/OutputFiles/'

save_dir=$run_dir'/OutputFiles/'$cur_dt
usb_port='/dev/video1'

vid_save_dir=$save_dir'/'
image_width='640'
image_height='480'
video_fps='30.0'
imageFormat='jpeg'

numCam='0'
uartPort='/dev/ttyUSB0'
baudRate='921600'



mkdir $save_dir
echo $save_dir
mkdir $save_dir'/camera1'
mkdir $save_dir'/camera2'

cd build
./recordVideoIMU $vid_save_dir $image_width $image_height $video_fps $numCam $imageFormat $uartPort $baudRate

