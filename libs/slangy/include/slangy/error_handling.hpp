#pragma once

#include <slangy/error.hpp>

#include <system_error>

namespace slangy
{

template <typename T>
auto valueOrThrow(const Expected<T>& result)
{
    if (!result)
    {
        throw std::system_error(result.error().code, result.error().message);
    }

    return result.value();
}

inline void throwIfError(SlangResult slangResult)
{
    if (slangResult < 0)
    {
        throw std::system_error(static_cast<slangy::ErrorCode>(slangResult));
    }
}

inline Unexpected makeUnexpected(int32_t errorCode, std::string message = "")
{
    return Unexpected{SlangyError{static_cast<slangy::ErrorCode>(errorCode), std::move(message)}};
}

inline Unexpected makeUnexpected(ErrorCode errorCode, std::string message = "")
{
    return Unexpected{SlangyError{errorCode, std::move(message)}};
}

}  // namespace slangy
