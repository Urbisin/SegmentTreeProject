cmake_minimum_required(VERSION 3.14)
project(SegmentTreeQt)

set(CMAKE_CXX_STANDARD 17)

set(Qt6_DIR "/Users/urbisin/Qt/6.7.2/macos/lib/cmake/Qt6")
list(APPEND CMAKE_PREFIX_PATH ${Qt6_DIR})

find_package(Qt6 COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(SegmentTreeQt main.cpp mainwindow.cpp SegmentTree.cpp)

target_link_libraries(SegmentTreeQt Qt6::Widgets)
