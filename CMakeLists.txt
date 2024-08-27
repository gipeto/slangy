cmake_minimum_required(VERSION 3.25)

# Allows find_package to look for <UPPER_CASE>_ROOT variables
cmake_policy(SET CMP0144 NEW)

project(slangy VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(slangCompilerCmakeDir "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if(APPLE)
  set(basePoint @loader_path)
else()
  set(basePoint $ORIGIN)
endif()

include(GNUInstallDirs)
file(RELATIVE_PATH relDir
  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}
  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})

set(CMAKE_INSTALL_RPATH ${basePoint} ${basePoint}/${relDir})

include(GenerateExportHeader)

include(add_slang_target)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
option(FETCH_SLANG "Fetch slang from the upstream." OFF)
option(BUILD_TESTS "Build the tests." ON)
option(BUILD_EXAMPLES "Build the examples." ON)

if(FETCH_SLANG)
   include(fetchSlang)
endif()

find_package(slang REQUIRED QUIET)

if(BUILD_TESTS)
   find_package(GTest REQUIRED QUIET)
endif()

install(FILES cmake/add_slang_target.cmake
              cmake/bin_file_to_header.cmake
              cmake/bin2h.cmake
              cmake/slang_compiler.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/slangy)

add_subdirectory(libs)

if(BUILD_EXAMPLES)
   add_subdirectory(examples)
endif()
