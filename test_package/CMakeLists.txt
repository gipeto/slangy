cmake_minimum_required(VERSION 3.15)
project(PackageTest CXX)

find_package(slangy CONFIG REQUIRED)

include(add_slang_target)

set(SHADER_SOURCES shaders/test_shader.slang)

add_slang_target(computeMain FILES ${SHADER_SOURCES} NAMESPACE shaders)

add_executable(testPackageApp src/testPackageApp.cpp)
target_link_libraries(testPackageApp slangy::slangy shaders::computeMain)
