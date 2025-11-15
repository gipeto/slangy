#include <slangy/slangy.hpp>

#include <slangy/com/helpers.hpp>
#include <slangy/error_handling.hpp>
#include <slangy/utils/DiagnosticLogger.hpp>
#include <slangy/utils/SlangBlob.hpp>

namespace slangy
{

inline Expected<ComPtr<slang::IGlobalSession>> makeGlobalSession(const SlangGlobalSessionDesc& globalSessionDesc)
{
    ComPtr<slang::IGlobalSession> gSession;
    if (const auto result = createGlobalSession(&globalSessionDesc, gSession.put()); result < 0)
    {
        return makeUnexpected(result);
    }

    return gSession;
}

inline Expected<ComPtr<slang::ISession>> makeSession(const ComPtr<slang::IGlobalSession>& globalSession,
                                                     const slang::SessionDesc& sessionDesc)
{
    ComPtr<slang::ISession> session;
    if (const auto result = globalSession->createSession(sessionDesc, session.put()); result < 0)
    {
        return makeUnexpected(result);
    }

    return session;
}

template <typename... PreCompiledModule>
Expected<std::vector<std::vector<std::byte>>> buildShaderProgram(const slang::SessionDesc& sessionDescription,
                                                                 const ComPtr<slang::IGlobalSession>& globalSession,
                                                                 std::string name, std::string entryPointName,
                                                                 PreCompiledModule&&... precompiledModules)
{
    ComPtr<slang::ISession> session;
    if (auto result = makeSession(globalSession, sessionDescription))
    {
        session = std::move(result.value());
    }
    else
    {
        return Unexpected(std::move(result.error()));
    }

    std::vector<ComPtr<slang::IBlob>> blobs;
    (blobs.push_back(makeFromImpl<slang::IBlob, SlangBlob>(std::forward<PreCompiledModule>(precompiledModules))), ...);

    std::vector<ComPtr<slang::IModule>> modules(blobs.size());
    std::vector<slang::IComponentType*> componentTypes;
    ComPtr<slang::IEntryPoint> entryPoint;

    for (size_t i = 0; i < blobs.size(); ++i)
    {
        std::string moduleName = name + std::to_string(i);
        std::string logPrefix = std::string("Load module ") + moduleName;
        // The interface pointer returned by session->loadModuleFromIRBlob is not AddRef, hece the usage
        // of copyFrom instead of attach to initialize the modules
        modules[i].copyFrom(
            session->loadModuleFromIRBlob(moduleName.c_str(), "", blobs[i].get(), DLog(std::move(logPrefix))()));

        if (!modules[i])
        {
            return makeUnexpected(ErrorCode::FailedToOpenResource, "Failed to load module " + moduleName);
        }

        if (!entryPoint)
        {
            if (SLANG_OK == modules[i]->findEntryPointByName(entryPointName.c_str(), entryPoint.put()))
            {
                componentTypes.push_back(entryPoint.get());
            }
        }

        componentTypes.push_back(modules[i].get());
    }

    ComPtr<slang::IComponentType> composedProgram;
    if (auto result = session->createCompositeComponentType(
            componentTypes.data(), componentTypes.size(), composedProgram.put(), DLog("Create composite component")());
        result < 0)
    {
        return makeUnexpected(result, "Failed to create composite program component");
    }

    // Link the composed program
    ComPtr<slang::IComponentType> linkedProgram;
    if (auto result = composedProgram->link(linkedProgram.put(), DLog("Link program")()); result < 0)
    {
        return makeUnexpected(result, "Failed to link composite program");
    }

    std::vector<std::vector<std::byte>> perTargetPrograms;
    perTargetPrograms.reserve(sessionDescription.targetCount);

    for (SlangInt targetIndex = 0; targetIndex < sessionDescription.targetCount; ++targetIndex)
    {
        ComPtr<slang::IBlob> shaderBlob;
        if (auto result =
                linkedProgram->getTargetCode(targetIndex, shaderBlob.put(), DLog("Retrieve shader program")());
            result < 0)
        {
            return makeUnexpected(
                result, "Failed retrieve compiled shader program for targetIndex " + std::to_string(targetIndex));
        }

        perTargetPrograms.push_back(std::vector<std::byte>(
            reinterpret_cast<const std::byte*>(shaderBlob->getBufferPointer()),
            reinterpret_cast<const std::byte*>(shaderBlob->getBufferPointer()) + shaderBlob->getBufferSize()));
    }

    return perTargetPrograms;
}

}  // namespace slangy
