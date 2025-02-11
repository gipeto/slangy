
include(slang_compiler)

# Pre-compile the input shader modules and store the resulting IR blobs into header files.
# The generated header are grouped into an interface target.
#
# Usage: add_slang_target(targetName
#                         FILES module1.slang [module2.slang ...]
#                         NAMESPACE targetNamespace)
#
# targetName: the name of the generate interface target. An alias targetNamespace::targetName will also be defined.
# FILES:      the list of module sources. For each source headers named module1.h [module2.h ...] will be generated.
#             Each header file will contain an array of bytes named MODULE1_SLANG_MODULE [MODULE2_SLANG_MODULE ...]
#             The arrays are wrapped into namespace targetNamespace
# NAMESPACE:  the namespace used for the target and the generated header files
#
function(ADD_SLANG_TARGET)

    set(noValues "")
    set(singleValues NAMESPACE)
    set(multiValues FILES)
    set(ARG_TARGET_NAME ${ARGV0})
    
    cmake_parse_arguments(
          PARSE_ARGV 1
          ARG
          "${noValues}" "${singleValues}" "${multiValues}")
  
     set(SHADER_HEADERS)
     foreach(shaderFile IN LISTS ARG_FILES)

       cmake_path(GET shaderFile PARENT_PATH SHADER_PATH)
       cmake_path(GET shaderFile STEM SHADER_NAME)

       set(SHADER_HEADER ${ARG_NAMESPACE}/${SHADER_NAME}.h)
       list(APPEND SHADER_HEADERS ${CMAKE_CURRENT_BINARY_DIR}/${SHADER_HEADER})

       slang_compile(INFILE ${shaderFile}
          OUTFILE ${SHADER_HEADER}
          NAMESPACE ${ARG_NAMESPACE})

     endforeach()

   add_library(${ARG_TARGET_NAME} INTERFACE ${SHADER_HEADERS})
   target_include_directories(${ARG_TARGET_NAME} INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/${ARG_NAMESPACE}> )

   add_library(${ARG_NAMESPACE}::${ARG_TARGET_NAME}  ALIAS ${ARG_TARGET_NAME})

            
endfunction()
