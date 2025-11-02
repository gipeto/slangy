#include <slangy/com/Implements.hpp>

#include <cstring>

namespace slangy
{

namespace detail
{

inline bool slangUUIDEqual(const SlangUUID& lhs, const SlangUUID& rhs)
{
    return !std::memcmp(&lhs, &rhs, sizeof(SlangUUID));
}

template <typename D>
std::true_type is_base_wrapper(const Base<D>*);
std::false_type is_base_wrapper(...);

template <bool isBaseType, typename I>
struct ExtractInterfaceType
{
};

template <typename I>
struct ExtractInterfaceType<true, I>
{
    using InterfaceType = typename I::InterfaceType;
};

template <typename I>
struct ExtractInterfaceType<false, I>
{
    using InterfaceType = I;
};

template <typename I>
using get_interface_type_t =
    typename ExtractInterfaceType<decltype(is_base_wrapper(std::declval<I*>()))::value, I>::InterfaceType;

}  // namespace detail

template <typename... Interfaces>
template <bool IsFirst, typename Interface, typename... OtherInterfaces>
void* Implements<Interfaces...>::maybeInterfacePtr(SlangUUID const& id) noexcept
{
    using TestedInterface = detail::get_interface_type_t<Interface>;

    if constexpr (IsFirst)
    {
        if (detail::slangUUIDEqual(id, TestedInterface::getTypeGuid())
            || detail::slangUUIDEqual(id, ISlangUnknown::getTypeGuid()))
        {
            return static_cast<TestedInterface*>(this);
        }
    }
    else
    {
        if (detail::slangUUIDEqual(id, TestedInterface::getTypeGuid()))
        {
            return static_cast<TestedInterface*>(this);
        }
    }

    return maybeInterfacePtr<false, OtherInterfaces...>(id);
}

template <typename... Interfaces>
template <int>
void* Implements<Interfaces...>::maybeInterfacePtr(SlangUUID const& id) noexcept
{
    return nullptr;
}

template <typename... Interfaces>
int32_t Implements<Interfaces...>::queryInterface(SlangUUID const& uuid, void** outObject) noexcept
{
    *outObject = maybeInterfacePtr<true, Interfaces...>(uuid);

    if (nullptr == *outObject)
    {
        return SLANG_E_NO_INTERFACE;
    }
    static_cast<ISlangUnknown*>(*outObject)->addRef();
    return SLANG_OK;
}

template <typename... Interfaces>
uint32_t Implements<Interfaces...>::addRef() noexcept
{
    return ++m_ref;
}

template <typename... Interfaces>
uint32_t Implements<Interfaces...>::release() noexcept
{
    auto remaining = --m_ref;
    if (0 == remaining)
    {
        delete this;
    }
    return remaining;
}

}  // namespace slangy
