add_library(sin_plugin SHARED sin.cpp)
target_include_directories(sin_plugin PRIVATE ${CMAKE_SOURCE_DIR}/src)
set_target_properties(sin_plugin PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins)