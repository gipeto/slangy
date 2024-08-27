
include(FindPackageHandleStandardArgs)

if(slang_FOUND)
  return()
endif()

set(SLANG_LIB_DIRS ${SLANG_ROOT}/lib ${SLANG_ROOT}/bin)
set(SLANG_INCLUDE_DIRS  ${SLANG_ROOT}/include)
set(SLANG_TOOL_DIRS ${SLANG_ROOT}/tools/shader-slang ${SLANG_ROOT}/bin)

find_library(slang_LIBRARY NAMES slang PATHS ${SLANG_LIB_DIRS} NO_DEFAULT_PATH)
find_library(slang-rt_LIBRARY NAMES slang-rt PATHS ${SLANG_LIB_DIRS} NO_DEFAULT_PATH)
find_library(gfx_LIBRARY NAMES gfx PATHS ${SLANG_LIB_DIRS} NO_DEFAULT_PATH)

find_path(slang_INCLUDE_DIR NAMES slang.h PATHS ${SLANG_INCLUDE_DIRS} NO_DEFAULT_PATH)

find_program(slang_COMPILER slangc PATHS ${SLANG_TOOL_DIRS} NO_DEFAULT_PATH)

find_package_handle_standard_args(slang DEFAULT_MSG slang_LIBRARY slang-rt_LIBRARY gfx_LIBRARY slang_INCLUDE_DIR slang_COMPILER)

mark_as_advanced(slang_LIBRARY slang-rt_LIBRARY gfx_LIBRARY slang_INCLUDE_DIR slang_COMPILER)

add_library(slang::slang UNKNOWN IMPORTED)
set_target_properties(slang::slang PROPERTIES
  IMPORTED_LOCATION "${slang_LIBRARY}"
  INTERFACE_INCLUDE_DIRECTORIES "${slang_INCLUDE_DIR}"
)

add_library(slang::slang-rt UNKNOWN IMPORTED)
set_target_properties(slang::slang-rt PROPERTIES
  IMPORTED_LOCATION "${slang-rt_LIBRARY}"
  INTERFACE_INCLUDE_DIRECTORIES "${slang_INCLUDE_DIR}"
)

add_library(slang::gfx UNKNOWN IMPORTED)
set_target_properties(slang::gfx PROPERTIES
  IMPORTED_LOCATION "${gfx_LIBRARY}"
  INTERFACE_INCLUDE_DIRECTORIES "${slang_INCLUDE_DIR}"
)
message("Slang include folder: ${slang_INCLUDE_DIR}")
message("Slang library folder: ${slang_LIBRARY}")
message("Slang compiler: ${slang_COMPILER}")
