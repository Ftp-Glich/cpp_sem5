add_library(add_plugin SHARED add.cpp)
target_include_directories(add_plugin PRIVATE ${CMAKE_SOURCE_DIR}/src)
set_target_properties(add_plugin PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins)