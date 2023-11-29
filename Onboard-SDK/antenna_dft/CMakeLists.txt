# CMakeLists.txt for P5/antenna_dft

cmake_minimum_required(VERSION 2.8)
project(antenna_dft)

add_executable(${PROJECT_NAME} main.c antenna.c antenna.h)
target_link_libraries(${PROJECT_NAME} m)