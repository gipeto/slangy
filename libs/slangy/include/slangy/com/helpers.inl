
#include <slangy/com/helpers.hpp>

#include <utility>

namespace slangy
{

namespace detail
{

template <typename Interface>
auto uuidof(const ComPtr<Interface>& p)
{
    if constexpr (std::is_base_of_v<ISlangUnknown, Interface>)
    {
        return Interface::getTypeGuid();
    }
#ifdef _WIN32
    else
    {
        return __uuidof(Interface);
    }
#endif
}

template <auto memberFunctionPtr, typename T>
auto memberFun(T&& obj)
{
    return [&obj](auto&&... args)
    { return (std::forward<T>(obj).*memberFunctionPtr)(std::forward<decltype(args)>(args)...); };
}

}  // namespace detail

template <typename Interface, typename Implementation, typename... Args>
ComPtr<Interface> makeFromImpl(Args&&... args)
{
    ComPtr<Interface> implementationInterface;
    implementationInterface.attach(new Implementation(std::forward<Args>(args)...));
    return implementationInterface;
}

template <typename Interface, typename Fun, typename... Args>
ComPtr<Interface> asComPtr(Fun&& fun, Args&&... args)
{
    ComPtr<Interface> result;
    fun(std::forward<Args>(args)..., detail::uuidof(result), result.put_void());
    return result;
}

template <typename Interface, auto memberFunctionPtr, typename T, typename... Args>
ComPtr<Interface> asComPtr(T&& obj, Args&&... args)
{
    return asComPtr<Interface>(detail::memberFun<memberFunctionPtr>(obj), std::forward<Args>(args)...);
}

}  // namespace slangy
