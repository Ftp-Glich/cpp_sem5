add_library(calculator_lib STATIC
    application.cpp 
    calculator.cpp 
    plugin_manager.cpp 
)

add_executable(calculator main.cpp)

target_link_libraries(calculator PRIVATE calculator_lib)

target_include_directories(calculator_lib PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_include_directories(calculator PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_include_directories(calculator_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)
