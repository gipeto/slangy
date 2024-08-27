#pragma once

#include <atomic>
#include <slang.h>

namespace slangy::com
{

template <typename Interface>
struct Base
{
    using InterfaceType = Interface;
};

/// Helper class for implementing Slang COM-like interfaces.
///
/// The class provides an implementation of the ISlangUnknown interface methods
/// addRef, release and queryInterface.
/// @note that the reference counter initialized to one, therefore addRef is not to be called
/// on the interface after construction.
///
/// Usage example implementing the ISlangBlob interface:
/// @code {.cpp}
/// struct SlangBlob : Implements<ISlangBlob>
/// {
///    SlangBlob(std::vector<uint8_t> buffer)
///    : m_buffer(std::move(buffer))
///    {}
///
///    void const* SLANG_MCALL getBufferPointer() noexcept override
///    {
///      return reinterpret_cast<const void*>(m_buffer.data());
///    }
///
///    size_t SLANG_MCALL getBufferSize() noexcept override
///    {
///      return m_buffer.size();
///    }
///
///    std::vector<uint8_t> m_buffer;
/// };
/// @endcode
///
/// Note that all interface types for which the queryInterface method must return a valid result must be provided to the
/// @tparam Interfaces pack. For implementing interfaces not deriving directly form ISlangUnknown, the Base interface
/// wrapper should be used for the interfaces situated between the most derived class and ISlangUnknown in the
/// hierarchy.
///
/// Example:
///
/// @code {.cpp}
///
///   class IFoo : public ISlangUnknown;
///   class IBar : public IFoo;
///   class IBar2 : public IBar;
///   class IAnotherFoo : public ISlangUnknown;
///
///   // IBar2 is the most derived class, so it must not be wrapped by Base
///   // IBar and IFoo must be wrapped by Base as in the middle of the hierarchy
///   // IAnotherFoo is unrelated to the other interfaces, and it is the most derived class, so it must not be wrapped
///   by base class FooBarImpl : public Implements<IBar2, Base<IBar>, IAnotherFoo, Base<IFoo>>;
///
/// @endcode
///
/// Adapted from the article @ref https://learn.microsoft.com/en-us/archive/msdn-magazine/2014/special-issue/c-visual-c-2015-brings-modern-c-to-the-windows-api
template <typename... Interfaces>
class Implements : public Interfaces...
{
    template <bool IsFirst, typename Interface, typename... OtherInterfaces>
    void* maybeInterfacePtr(SlangUUID const& id) noexcept;

    template <int = 0>
    void* maybeInterfacePtr(SlangUUID const& id) noexcept;

protected:
    int32_t SLANG_MCALL queryInterface(SlangUUID const& uuid, void** outObject) noexcept override;
    uint32_t SLANG_MCALL addRef() noexcept override;
    uint32_t SLANG_MCALL release() noexcept override;

    virtual ~Implements() noexcept
    {
    }

    std::atomic<uint32_t> m_ref{1};
};

}  // namespace slangy::com

#include "Implements.inl"
