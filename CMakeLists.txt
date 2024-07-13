cmake_minimum_required(VERSION 3.3)
project(ascii_video_player)
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
    opencv
    GIT_REPOSITORY https://github.com/opencv/opencv.git
    GIT_TAG 4.10.0
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(opencv)

FetchContent_Declare(
    argparse
    GIT_REPOSITORY https://github.com/p-ranav/argparse.git
    GIT_TAG v3.0
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(argparse)

add_executable(ascii_video_player src/main.cpp src/ascii_player.cpp)
target_include_directories(ascii_video_player PRIVATE
    ${OPENCV_CONFIG_FILE_INCLUDE_DIR}
    ${OPENCV_MODULE_opencv_core_LOCATION}/include
    ${OPENCV_MODULE_opencv_highgui_LOCATION}/include
)
target_link_libraries(ascii_video_player PRIVATE opencv_core opencv_videoio opencv_imgproc)
target_link_libraries(ascii_video_player PRIVATE argparse)