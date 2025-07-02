#include <slangy/com/ComPtr.hpp>

#include <cassert>

namespace slangy::com
{

template <typename Interface>
ComPtr<Interface>::ComPtr(const ComPtr& other) noexcept
      : m_ptr(other.m_ptr)
{
    internalAddRef();
}

template <typename Interface>
template <typename U>
ComPtr<Interface>::ComPtr(const ComPtr<U>& other) noexcept
      : m_ptr(other.m_ptr)
{
    internalAddRef();
}

template <typename Interface>
template <typename U>
ComPtr<Interface>::ComPtr(ComPtr<U>&& other) noexcept
      : m_ptr(other.m_ptr)
{
    other.m_ptr = nullptr;
}

template <typename Interface>
ComPtr<Interface>::~ComPtr() noexcept
{
    internalRelease();
}

template <typename Interface>
ComPtr<Interface>& ComPtr<Interface>::operator=(const ComPtr& other) noexcept
{
    internalCopy(other.m_ptr);
    return *this;
}

template <typename Interface>
template <typename U>
ComPtr<Interface>& ComPtr<Interface>::operator=(const ComPtr<U>& other) noexcept
{
    internalCopy(other.m_ptr);
    return *this;
}

template <typename Interface>
template <typename U>
ComPtr<Interface>& ComPtr<Interface>::operator=(ComPtr<U>&& other) noexcept
{
    internalMove(other);
    return *this;
}

template <typename Interface>
ComPtr<Interface>::operator bool() const noexcept
{
    return nullptr != m_ptr;
}

template <typename Interface>
Interface** ComPtr<Interface>::getAddressOf() noexcept
{
    assert(m_ptr == nullptr);
    return &m_ptr;
}

template <typename Interface>
NoAddRefRelease<Interface>* ComPtr<Interface>::operator->() const noexcept
{
    return static_cast<NoAddRefRelease<Interface>*>(m_ptr);
}

template <typename Interface>
Interface* ComPtr<Interface>::get() const noexcept
{
    return m_ptr;
}

template <typename Interface>
void ComPtr<Interface>::attach(Interface* value) noexcept
{
    internalRelease();
    m_ptr = value;
}

template <typename Interface>
Interface* ComPtr<Interface>::detach() noexcept
{
    Interface* temp = m_ptr;
    m_ptr = nullptr;
    return temp;
}

template <typename Interface>
ComPtr<Interface>& ComPtr<Interface>::operator=(std::nullptr_t) noexcept
{
    internalRelease();
    return *this;
}

template <typename Interface>
template <typename T>
ComPtr<T> ComPtr<Interface>::as() const noexcept
{
    ComPtr<T> temp;
    m_ptr->QueryInterface(temp.getAddressOf());
    return temp;
}

template <typename Interface>
void ComPtr<Interface>::copyFrom(Interface* other) noexcept
{
    internalCopy(other);
}

template <typename Interface>
void ComPtr<Interface>::copyTo(Interface** other) const noexcept
{
    internalAddRef();
    *other = m_ptr;
}

template <typename Interface>
void ComPtr<Interface>::internalCopy(Interface* other) noexcept
{
    if (m_ptr != other)
    {
        internalRelease();
        m_ptr = other;
        internalAddRef();
    }
}

template <typename Interface>
template <typename T>
void ComPtr<Interface>::internalMove(ComPtr<T>& other) noexcept
{
    if (m_ptr != other.m_ptr)
    {
        internalRelease();
        m_ptr = other.m_ptr;
        other.m_ptr = nullptr;
    }
}

template <typename Interface>
void ComPtr<Interface>::internalAddRef() const noexcept
{
    if (m_ptr)
    {
        m_ptr->addRef();
    }
}

template <typename Interface>
void ComPtr<Interface>::internalRelease() noexcept
{
    Interface* temp = m_ptr;

    if (temp)
    {
        m_ptr = nullptr;
        temp->release();
    }
}

}  // namespace slangy::com
