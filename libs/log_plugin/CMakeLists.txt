add_library(log_plugin SHARED log.cpp)
target_include_directories(log_plugin PRIVATE ${PROJECT_SOURCE_DIR}/include)
set_target_properties(log_plugin PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins)