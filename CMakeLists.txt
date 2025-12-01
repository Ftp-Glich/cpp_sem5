cmake_minimum_required(VERSION 3.28.1)
project(clalculator)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)


include(FetchContent)

set(FETCHCONTENT_QUIET OFF) 
set(FETCHCONTENT_FULLY_DISCONNECTED OFF) 

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE       
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/googletest
)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
    message(STATUS "Downloading GoogleTest...")
    FetchContent_Populate(googletest)
    message(STATUS "GoogleTest downloaded successfully")
endif()

add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})

add_subdirectory(libs/power_plugin)
add_subdirectory(libs/sinus_plugin)
add_subdirectory(libs/log_plugin)
add_subdirectory(src)

add_subdirectory(tests)
