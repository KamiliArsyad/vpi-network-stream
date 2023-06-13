cmake_minimum_required(VERSION 3.5)

project(vpi_network_stream)

find_package(vpi 2.2 REQUIRED)
find_package(OpenCV REQUIRED)
find_package(ZeroMQ REQUIRED)

add_executable(${PROJECT_NAME} main.cpp encoder.cpp decoder.cpp VPINetStream.cpp)
target_link_libraries(${PROJECT_NAME} vpi zmq opencv_imgproc opencv_core opencv_imgcodecs opencv_features2d opencv_videoio)