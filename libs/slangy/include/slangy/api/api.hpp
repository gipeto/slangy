#pragma once

#include <slang.h>

#include <slangy/com/ComPtr.hpp>
#include <slangy/error.hpp>
#include <slangy/utils/results.hpp>

#include <expected>

namespace slangy::api
{

inline std::expected<com::ComPtr<slang::IGlobalSession>, slangy::ErrorCode> makeGlobalSession()
{
    com::ComPtr<slang::IGlobalSession> gSession;
    if (const auto result = createGlobalSession(gSession.getAddressOf()); result < 0)
    {
        return utils::makeUnexpected(result);
    }

    return gSession;
}

inline std::expected<com::ComPtr<slang::ISession>, slangy::ErrorCode> makeSession(
    const com::ComPtr<slang::IGlobalSession>& globalSession, const slang::SessionDesc& desc)
{
    com::ComPtr<slang::ISession> session;
    if (const auto result = globalSession->createSession(desc, session.getAddressOf()); result < 0)
    {
        return utils::makeUnexpected(result);
    }

    return session;
}

}  // namespace slangy::api
