cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_STANDARD 11)
project( recordVideoIMU )

find_package( OpenCV 3.2.0 REQUIRED )
find_package( Boost 1.40 COMPONENTS program_options filesystem thread REQUIRED )

set(SOURCE_FILES
		main.cpp
		VideoRecorder/src/VideoRecorder.cpp
        IMUrecorder/src/autopilot_interface.cpp
        IMUrecorder/src/IMUrecorder.cpp
        IMUrecorder/src/mavlink_control.cpp
        IMUrecorder/src/serial_port.cpp
		)

set(HEADER_FILES
        VideoRecorder/include/VideoRecorder.h
        IMUrecorder/include/autopilot_interface.h
        IMUrecorder/include/IMUrecorder.h
        IMUrecorder/include/mavlink_control.h
        IMUrecorder/include/serial_port.h
        IMUrecorder/include/mavlink/v1.0/common
		)
include_directories(
		${CMAKE_SOURCE_DIR}/VideoRecorder/include/
        ${CMAKE_SOURCE_DIR}/IMUrecorder/include/
        ${CMAKE_SOURCE_DIR}/IMUrecorder/include/mavlink/v1.0/
		${Boost_INCLUDE_DIR}
)


add_executable( recordVideoIMU ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries( recordVideoIMU ${OpenCV_LIBS} ${Boost_LIBRARIES})
