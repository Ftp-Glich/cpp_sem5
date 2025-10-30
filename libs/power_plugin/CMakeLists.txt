add_library(power_plugin SHARED power.cpp)
target_include_directories(power_plugin PRIVATE ${PROJECT_SOURCE_DIR}/include)
set_target_properties(power_plugin PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins)