#pragma once

#include <slangy/com/ComPtr.hpp>

namespace slangy
{

template <typename Interface, typename Implementation, typename... Args>
ComPtr<Interface> makeFromImpl(Args&&... args);

template <typename Interface, typename Fun, typename... Args>
ComPtr<Interface> asComPtr(Fun&& fun, Args&&... args);

template <typename Interface, auto memberFunctionPtr, typename T, typename... Args>
ComPtr<Interface> asComPtr(T&& obj, Args&&... args);

}  // namespace slangy

#include "helpers.inl"
