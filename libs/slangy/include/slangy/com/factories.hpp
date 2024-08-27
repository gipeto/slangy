#pragma once

#include <slangy/com/ComPtr.hpp>
#include <utility>

namespace slangy::com
{

template <typename Interface, typename Implementation, typename... Args>
ComPtr<Interface> makeFromImpl(Args&&... args)
{
    ComPtr<Interface> interface;
    interface.attach(new Implementation(std::forward<Args>(args)...));
    return interface;
}
}  // namespace slangy::com
