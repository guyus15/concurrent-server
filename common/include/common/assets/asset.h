#pragma once

#include <cstddef>

template <typename T>
class AssetManager;

class Asset
{
public:
    Asset()
        : m_asset_id{}
    {
    }

    Asset(const Asset&) = default;
    Asset& operator=(const Asset&) = default;

    Asset(Asset&&) noexcept = default;
    Asset& operator=(Asset&&) noexcept = default;

    virtual ~Asset() = default;

    virtual void Load() = 0;
    virtual void Unload() = 0;

    [[nodiscard]] size_t GetAssetId() const
    {
        return m_asset_id;
    }

protected:
    size_t m_asset_id;

    void SetAssetId(const size_t id)
    {
        m_asset_id = id;
    }

    template <typename T>
    friend class AssetManager;
};
