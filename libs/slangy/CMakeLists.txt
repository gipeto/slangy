add_library(slangy INTERFACE)

target_sources(slangy
      INTERFACE
          FILE_SET install
          TYPE HEADERS
          BASE_DIRS
           include
          FILES
           include/slangy/api/api.hpp
           include/slangy/com/factories.hpp
           include/slangy/com/ComPtr.hpp
           include/slangy/com/ComPtr.inl
           include/slangy/com/Implements.hpp
           include/slangy/com/Implements.inl
           include/slangy/gfx/DebugCallback.hpp
           include/slangy/gfx/gfx.hpp
           include/slangy/impl/SlangIRBlob.hpp
           include/slangy/utils/DiagnosticLogger.hpp
           include/slangy/utils/results.hpp
           include/slangy/error.hpp
           include/slangy/slangy.hpp
)

target_link_libraries(slangy INTERFACE slang::slang slang::gfx)

install(TARGETS slangy
        EXPORT slangyTargets
        FILE_SET install
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

add_library(slangy::slangy ALIAS slangy)

install(EXPORT slangyTargets
DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/slangy
NAMESPACE slangy::
FILE slangyTargets.cmake)

include(CMakePackageConfigHelpers)

set(slangCompilerCMakeInstallDir ${CMAKE_INSTALL_LIBDIR}/cmake/slangy)

configure_package_config_file(cmake/slangyConfig.cmake.in
                              slangyConfig.cmake
                              INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/slangy
                              PATH_VARS slangCompilerCMakeInstallDir
                              NO_SET_AND_CHECK_MACRO
                              NO_CHECK_REQUIRED_COMPONENTS_MACRO)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/slangyConfig.cmake
DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/slangy)

if(BUILD_TESTS)
  add_subdirectory(tests)
endif()
