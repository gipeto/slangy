#pragma once

#include <slang.h>

#include <slangy/com/ComPtr.hpp>
#include <slangy/error.hpp>

#include <cstddef>
#include <string>
#include <vector>

namespace slangy
{

/// Create a compile global session
/// @param globalSessionDesc the slang global session configuration parameters
Expected<ComPtr<slang::IGlobalSession>> makeGlobalSession(const SlangGlobalSessionDesc& globalSessionDesc = {});

/// Create a compile session from a global session
/// @param globalSession the global session from which the session is created
/// @param sessionDescription the slang session configuration parameters
Expected<ComPtr<slang::ISession>> makeSession(const ComPtr<slang::IGlobalSession>& globalSession,
                                              const slang::SessionDesc& sessionDescription);

/// Compiles pre-compiled modules into a shader program binary for each target defined in \p sessionDescription
/// @param sessionDescription the slang session configuration parameters
/// @param globalSession the global session used for compilation
/// @param name name for the compiled program
/// @param entryPointName the name of program entry poin
/// @param precompiledModules the pre-compiled modules to link for creating the program
/// @return a compiled shader program for each target defined in \p sessionDescription
template <typename... PreCompiledModule>
Expected<std::vector<std::vector<std::byte>>> buildShaderProgram(const slang::SessionDesc& sessionDescription,
                                                                 const ComPtr<slang::IGlobalSession>& globalSession,
                                                                 std::string name, std::string entryPointName,
                                                                 PreCompiledModule&&... precompiledModules);

}  // namespace slangy

#include "slangy.inl"
