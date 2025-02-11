
function(SLANG_COMPILE)

   set(noValues "")
   set(singleValues INFILE OUTFILE NAMESPACE)
   set(multiValues "")

   cmake_parse_arguments(
      PARSE_ARGV 0
      SLANG_COMPILE
      "${noValues}" "${singleValues}" "${multiValues}")

     cmake_path(GET SLANG_COMPILE_INFILE FILENAME SLANG_SHADER_FILENAME)

     set(SLANG_SHADER "${CMAKE_CURRENT_SOURCE_DIR}/${SLANG_COMPILE_INFILE}")
     set(SLANG_MODULE "${CMAKE_CURRENT_BINARY_DIR}/${SLANG_SHADER_FILENAME}-module")
     set(SLANG_MODULE_HEADER "${CMAKE_CURRENT_BINARY_DIR}/${SLANG_COMPILE_OUTFILE}")

     # .slang -> .slang-module
     add_custom_command(OUTPUT "${SLANG_MODULE}"
                        COMMAND "${slang_COMPILER}"
                        ARGS "${SLANG_SHADER}"
                             "-o"
                             "${SLANG_MODULE}"
                        COMMENT "Compile slang shader from file '${SLANG_SHADER}' to '${SLANG_MODULE}'.")

     # .spv -> .hpp
      add_custom_command(OUTPUT "${SLANG_MODULE_HEADER}"
                        COMMAND ${CMAKE_COMMAND}
                        ARGS "-DINPUT_SHADER_FILE=${SLANG_MODULE}"
                             "-DOUTPUT_HEADER_FILE=${SLANG_MODULE_HEADER}"
                             "-DHEADER_NAMESPACE=${SLANG_COMPILE_NAMESPACE}"
                             "-P"
                             "${slangCompilerCmakeDir}/bin_file_to_header.cmake"
                        WORKING_DIRECTORY "${slangCompilerCmakeDir}"
                        COMMENT "Converting compiled shader '${SLANG_MODULE}' to header file '${SLANG_MODULE_HEADER}'."
                        DEPENDS ${SLANG_MODULE})

endfunction()
