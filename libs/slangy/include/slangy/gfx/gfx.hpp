#pragma once

#include <slang-gfx.h>

#include <slangy/com/ComPtr.hpp>
#include <slangy/error.hpp>
#include <slangy/gfx/DebugCallback.hpp>

#include <slangy/utils/results.hpp>

#include <expected>

namespace slangy::gfx
{

std::expected<com::ComPtr<::gfx::IDevice>, slangy::ErrorCode> makeDevice(const ::gfx::IDevice::Desc& deviceDesc = {},
                                                                         DebugCallback* debugCall = nullptr)
{
    if (debugCall)
    {
        ::gfx::gfxEnableDebugLayer();
        if (const auto result = ::gfx::gfxSetDebugCallback(debugCall); result < 0)
        {
            return utils::makeUnexpected(result);
        }
    }

    com::ComPtr<::gfx::IDevice> gDevice;
    if (const auto result = ::gfx::gfxCreateDevice(&deviceDesc, gDevice.getAddressOf()); result < 0)
    {
        return utils::makeUnexpected(result);
    }

    return gDevice;
}

std::expected<com::ComPtr<::gfx::ICommandQueue>, slangy::ErrorCode> makeCommandQueue(
    const com::ComPtr<::gfx::IDevice>& device,
    const ::gfx::ICommandQueue::Desc& cmdQueueDesc = {::gfx::ICommandQueue::QueueType::Graphics})
{
    com::ComPtr<::gfx::ICommandQueue> cmdQueue;
    if (const auto result = device->createCommandQueue(cmdQueueDesc, cmdQueue.getAddressOf()); result < 0)
    {
        return utils::makeUnexpected(result);
    }
    return cmdQueue;
}

std::expected<com::ComPtr<::gfx::ITransientResourceHeap>, slangy::ErrorCode> makeTransientResourceHeap(
    const com::ComPtr<::gfx::IDevice>& device, const ::gfx::ITransientResourceHeap::Desc& transientHeapDesc)
{
    com::ComPtr<::gfx::ITransientResourceHeap> transientHeap;
    if (const auto result = device->createTransientResourceHeap(transientHeapDesc, transientHeap.getAddressOf());
        result < 0)
    {
        return utils::makeUnexpected(result);
    }
    return transientHeap;
}

}  // namespace slangy::gfx
