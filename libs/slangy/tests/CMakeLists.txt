add_executable(test_slangy test_runner.cpp
                           test_Implements.cpp)

target_link_libraries(test_slangy PRIVATE slangy::slangy GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
