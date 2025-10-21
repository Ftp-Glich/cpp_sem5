add_library(deg_plugin SHARED degree.cpp)
target_include_directories(deg_plugin PRIVATE ${CMAKE_SOURCE_DIR}/src)
set_target_properties(deg_plugin PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins)