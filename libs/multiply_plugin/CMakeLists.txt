add_library(mul_plugin SHARED multiply.cpp)
target_include_directories(mul_plugin PRIVATE ${CMAKE_SOURCE_DIR}/src)
set_target_properties(mul_plugin PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins)