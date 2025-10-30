set(TEST_TARGET calculator_tests)

add_executable(${TEST_TARGET} tests.cpp)

target_include_directories(${TEST_TARGET} PRIVATE 
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${TEST_TARGET} PRIVATE
    calculator_lib
    gtest
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(${TEST_TARGET}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)