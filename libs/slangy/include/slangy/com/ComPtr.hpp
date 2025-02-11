
#pragma once

#include <slang.h>

namespace slangy::com
{

template <typename T>
class NoAddRefRelease : public T
{
    uint32_t SLANG_MCALL addRef() noexcept override;
    uint32_t SLANG_MCALL release() noexcept override;
};

/// COM smart pointer adapted from
/// https://github.com/kennykerr/modern/blob/3f88e52801146513df804acf55870ad34b13a2bd/10.0.10240.complete/modern/comptr.h
/// MIT licensed: https://github.com/kennykerr/modern/blob/3f88e52801146513df804acf55870ad34b13a2bd/License.txt
/// Based on the article:
/// @ref
/// https://learn.microsoft.com/en-us/archive/msdn-magazine/2015/february/windows-with-c-com-smart-pointers-revisited
///
template <typename Interface>
class ComPtr
{
public:
    using Type = Interface;

    ComPtr() noexcept = default;

    ComPtr(const ComPtr& other) noexcept;

    template <typename U>
    ComPtr(const ComPtr<U>& other) noexcept;

    template <typename U>
    ComPtr(ComPtr<U>&& other) noexcept;

    ~ComPtr() noexcept;

    ComPtr& operator=(const ComPtr& other) noexcept;

    template <typename U>
    ComPtr& operator=(const ComPtr<U>& other) noexcept;

    template <typename U>
    ComPtr& operator=(ComPtr<U>&& other) noexcept;

    explicit operator bool() const noexcept;

    Interface** getAddressOf() noexcept;

    NoAddRefRelease<Interface>* operator->() const noexcept;

    Interface* get() const noexcept;

    void attach(Interface* value) noexcept;

    Interface* detach() noexcept;

    ComPtr& operator=(std::nullptr_t) noexcept;

    template <typename T>
    ComPtr<T> as() const noexcept;

    void copyFrom(Interface* other) noexcept;

    void copyTo(Interface** other) const noexcept;

private:
    void internalCopy(Interface* other) noexcept;

    template <typename T>
    void internalMove(ComPtr<T>& other) noexcept;

    void internalAddRef() const noexcept;

    void internalRelease() noexcept;

    template <typename T>
    friend class ComPtr;

    Interface* m_ptr = nullptr;
};

template <typename T>
void swap(ComPtr<T>& left, ComPtr<T>& right) noexcept
{
    T* temp = left.m_ptr;
    left.m_ptr = right.m_ptr;
    right.m_ptr = temp;
}

template <typename T>
bool operator==(ComPtr<T> const& left, ComPtr<T> const& right) noexcept
{
    return left.get() == right.get();
}

template <typename T>
bool operator!=(ComPtr<T> const& left, ComPtr<T> const& right) noexcept
{
    return !(left == right);
}

template <typename T>
bool operator<(ComPtr<T> const& left, ComPtr<T> const& right) noexcept
{
    return left.get() < right.get();
}

template <typename T>
bool operator>(ComPtr<T> const& left, ComPtr<T> const& right) noexcept
{
    return right < left;
}

template <typename T>
bool operator<=(ComPtr<T> const& left, ComPtr<T> const& right) noexcept
{
    return !(right < left);
}

template <typename T>
bool operator>=(ComPtr<T> const& left, ComPtr<T> const& right) noexcept
{
    return !(left < right);
}

}  // namespace slangy::com

#include "ComPtr.inl"
