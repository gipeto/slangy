#pragma once

#include <slangy/com/ComPtr.hpp>

#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>

namespace slangy::utils
{

class DLog
{
public:
    explicit DLog(std::string preamble = "")
          : DLog([preamble = std::move(preamble), this](std::string diagnosticMsg)
                 { std::cout << (preamble.empty() ? "" : preamble + " ") << diagnosticMsg << std::endl; })
    {
    }

    explicit DLog(std::function<void(std::string)> logFunction)
          : m_logFunction(std::move(logFunction))
    {
        if (!m_logFunction)
        {
            throw std::invalid_argument("Uninitialized logFunction");
        }
    }

    slang::IBlob** operator()()
    {
        return m_diagnosticBlob.getAddressOf();
    }

    ~DLog()
    {
        if (m_diagnosticBlob && m_diagnosticBlob->getBufferSize() != 0)
        {
            std::string diagnosticMsg(reinterpret_cast<const char*>(m_diagnosticBlob->getBufferPointer()),
                                      m_diagnosticBlob->getBufferSize());
            m_logFunction(std::move(diagnosticMsg));
        }
    }

private:
    std::function<void(std::string)> m_logFunction;
    com::ComPtr<slang::IBlob> m_diagnosticBlob;
};

}  // namespace slangy::utils
