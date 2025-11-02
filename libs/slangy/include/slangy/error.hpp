#pragma once

#include <slang.h>

#include <expected>
#include <string>
#include <system_error>
#include <type_traits>

namespace slangy
{

enum class ErrorCode : int
{
    GenericFailure = SLANG_FAIL,
    NotImplemented = SLANG_E_NOT_IMPLEMENTED,
    InterfaceNotFound = SLANG_E_NO_INTERFACE,
    OperationAborted = SLANG_E_ABORT,
    InvalidHandle = SLANG_E_INVALID_HANDLE,
    InvalidArgument = SLANG_E_INVALID_ARG,
    OutOfMemory = SLANG_E_OUT_OF_MEMORY,
    BufferTooSmall = SLANG_E_BUFFER_TOO_SMALL,
    UninitializedResult = SLANG_E_UNINITIALIZED,
    AsyncResultPending = SLANG_E_PENDING,
    FailedToOpenResource = SLANG_E_CANNOT_OPEN,
    ResourceNotFound = SLANG_E_NOT_FOUND,
    InternalFailure = SLANG_E_INTERNAL_FAIL,
    UnavailableFeature = SLANG_E_NOT_AVAILABLE,
    OperationTimeout = SLANG_E_TIME_OUT
};

struct SlangyError
{
    ErrorCode code;
    std::string message;
};

template <typename T>
using Expected = std::expected<T, SlangyError>;

using Unexpected = std::unexpected<SlangyError>;

namespace detail
{
class ErrorCategory : public std::error_category
{
public:
    const char* name() const noexcept override
    {
        return "slangy slang error";
    }

    std::string message(int code) const override
    {
        switch (static_cast<ErrorCode>(code))
        {
        case ErrorCode::GenericFailure:
            return "A severe error occurred and the call could not complete.";
        case ErrorCode::NotImplemented:
            return "Functionality not implemented.";
        case ErrorCode::InterfaceNotFound:
            return "Interface not found.";
        case ErrorCode::OperationAborted:
            return "The operation was aborted.";
        case ErrorCode::InvalidHandle:
            return "Invalid input handle.";
        case ErrorCode::InvalidArgument:
            return "Invalid input argument.";
        case ErrorCode::OutOfMemory:
            return "Operation ran out of memeory";
        case ErrorCode::BufferTooSmall:
            return "The provided buffer is too small for the operation to complete";
        case ErrorCode::UninitializedResult:
            return "The operation result has yet to be initialized.";
        case ErrorCode::AsyncResultPending:
            return "The result of an async operation is still pending.";
        case ErrorCode::FailedToOpenResource:
            return "A file or resource cannot be open.";
        case ErrorCode::ResourceNotFound:
            return "Cannot find a file or a resource.";
        case ErrorCode::InternalFailure:
            return "An unhandled internal failure did occur.";
        case ErrorCode::UnavailableFeature:
            return "The operation did not complete because some underlying hardware or software feature is "
                   "missing.";
        case ErrorCode::OperationTimeout:
            return "The operation did not complete because of a timeout.";
        }

        return std::string("Operation failed with unknown error: ") + std::to_string(code);
    }
};

}  // namespace detail

inline const std::error_category& errorCategory()
{
    static detail::ErrorCategory category;
    return category;
}

inline std::error_code make_error_code(ErrorCode error)
{
    return std::error_code(static_cast<int>(error), errorCategory());
}

}  // namespace slangy

namespace std
{
template <>
struct is_error_code_enum<slangy::ErrorCode> : true_type
{
};
}  // namespace std
