#pragma once

#include <slangy/error.hpp>

#include <expected>
#include <system_error>
#include <utility>

namespace slangy::utils
{

template <typename Fun, typename... Args>
auto value_or_throw(Fun&& fun, Args&&... args)
{
    auto result = fun(std::forward<Args>(args)...);
    if (!result)
    {
        throw std::system_error(result.error());
    }

    return result.value();
}

inline void throw_if_error(SlangResult slangResult)
{
    if (slangResult < 0)
    {
        throw std::system_error(static_cast<slangy::ErrorCode>(slangResult));
    }
}

inline std::unexpected<slangy::ErrorCode> makeUnexpected(int32_t errorCode)
{
    return std::unexpected<slangy::ErrorCode>{static_cast<slangy::ErrorCode>(errorCode)};
}

}  // namespace slangy::utils
