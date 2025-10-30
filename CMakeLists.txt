cmake_minimum_required(VERSION 3.28.1)
project(clalculator)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(googletest)

add_subdirectory(libs/power_plugin)
add_subdirectory(libs/sinus_plugin)
add_subdirectory(libs/log_plugin)
add_subdirectory(src)

add_subdirectory(tests)
