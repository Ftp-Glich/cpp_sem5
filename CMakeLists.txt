cmake_minimum_required(VERSION 3.28.1)
project(clalculator)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)


include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(gtest_disable_pthreads ON CACHE BOOL "" FORCE) 

FetchContent_MakeAvailable(googletest)

target_compile_features(gtest PUBLIC cxx_std_20)
target_compile_features(gtest_main PUBLIC cxx_std_20)
target_compile_features(gmock PUBLIC cxx_std_20)
target_compile_features(gmock_main PUBLIC cxx_std_20)

if (MSVC OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  target_compile_options(gtest PRIVATE /W0) 
  target_compile_options(gtest_main PRIVATE /W0)
  target_compile_options(gmock PRIVATE /W0)
  target_compile_options(gmock_main PRIVATE /W0)
endif()

add_subdirectory(libs/power_plugin)
add_subdirectory(libs/sinus_plugin)
add_subdirectory(libs/log_plugin)
add_subdirectory(src)

add_subdirectory(tests)
