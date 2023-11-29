#pragma once

#include "iapplication.h"

class IPacketDispatcher
{
public:
    explicit IPacketDispatcher(const IApplication* handle)
        : m_handle{ handle }
    {
    }

    IPacketDispatcher(const IPacketDispatcher&) = default;
    IPacketDispatcher& operator=(const IPacketDispatcher&) = default;

    IPacketDispatcher(IPacketDispatcher&&) noexcept = default;
    IPacketDispatcher& operator=(IPacketDispatcher&&) noexcept = default;

    virtual ~IPacketDispatcher() = default;

protected:
    const IApplication* m_handle;
};
