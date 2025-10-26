#pragma once

#include "slang.h"

#include <slangy/com/Implements.hpp>

#include <array>

namespace slangy
{

/// An in-memory implementation of a slang::IBlob
struct SlangBlob : public Implements<slang::IBlob>
{
    template <size_t N>
    SlangBlob(const std::array<uint8_t, N>& blob)
          : m_blob(blob.data())
          , m_size(blob.size()){};

    void const* SLANG_MCALL getBufferPointer() noexcept override
    {
        return reinterpret_cast<const void*>(m_blob);
    }

    size_t SLANG_MCALL getBufferSize() noexcept override
    {
        return m_size;
    }

    const uint8_t* m_blob{nullptr};
    size_t m_size{};
};

}  // namespace slangy
