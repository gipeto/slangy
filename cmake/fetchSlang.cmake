if(NOT SLANG_VERSION)
   set(SLANG_VERSION "2025.2.2")
endif()

if(WIN32)
    set(SLANG_OS "windows-x86_64")
elseif(APPLE)
    set(SLANG_OS "macos-aarch64")
elseif(UNIX)
    set(SLANG_OS "linux-x86_64")
endif()

include(FetchContent)
FetchContent_Populate(SLANG_SHADER URL https://github.com/shader-slang/slang/releases/download/v${SLANG_VERSION}/slang-${SLANG_VERSION}-${SLANG_OS}.zip)

set(SLANG_ROOT ${slang_shader_SOURCE_DIR})
