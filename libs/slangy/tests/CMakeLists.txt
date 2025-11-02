add_slang_target(computeMain FILES test_shader.slang NAMESPACE test_shader)

add_executable(test_slangy test_runner.cpp
                           test_build_api.cpp
                           test_Implements.cpp)

target_link_libraries(test_slangy PRIVATE slangy::slangy
                                          test_shader::computeMain
                                          GTest::gtest
                                          GTest::gtest_main
                                          GTest::gmock
                                          GTest::gmock_main)
