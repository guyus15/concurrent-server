#pragma once

#include "asset.h"

#include <common/utils/logging.h>

#include <type_traits>
#include <unordered_map>

/**
 * \brief Manages the loading, unloading and efficient retrieveal of resources.
 * \tparam T The type of resource to manage.
 */
template <typename T>
class AssetManager
{
    static_assert(std::is_base_of_v<Asset, T>, "AssetManager type must derive from Asset.");

public:
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    AssetManager(AssetManager&&) noexcept = default;
    AssetManager& operator=(AssetManager&&) noexcept = default;

    /**
     * \brief Loads a resource with the given parameters if the resource has not already been loaded.
     * If the resource already exists in the asset registry, it will return that instead.
     * \tparam Args The types of the parameters passed into the resource's constructor.
     * \param args The values of the parameters passed into the resource's constructor.
     * \return The loaded/retrieved resource.
     */
    template <typename... Args>
    [[nodiscard]] static T LoadOrRetrieve(Args... args)
    {
        // Compute the hash of the given arguments.
        size_t hash = 0;
        (..., (hash ^= std::hash<Args>{}(args)));

        // Return existing asset.
        if (Get().m_assets[hash].loaded)
        {
            AssetEntry& entry = Get().m_assets[hash];
            ++entry.reference_count;
            return entry.asset;
        }

        // Create and return new asset.
        AssetEntry new_entry{};
        new_entry.asset = T{ std::forward<Args>(args)... };
        new_entry.asset.Load();
        new_entry.asset.SetAssetId(hash);
        new_entry.loaded = true;
        new_entry.reference_count = 1;

        Get().m_assets[hash] = new_entry;

        return Get().m_assets[hash].asset;
    }

    /**
     * \brief Unloads a resource by the use of a reference counter. When the reference
     * counter reaches zero, the resource is officially unloaded an its entry is removed
     * from the registry.
     * \param asset_id The ID of the asset to unload.
     */
    static void Unload(size_t asset_id)
    {
        if (!Get().m_assets[asset_id].loaded)
            return;

        AssetEntry& entry = Get().m_assets[asset_id];
        --entry.reference_count;

        if (entry.reference_count <= 0)
        {
            entry.asset.Unload();
            const size_t erase_success = Get().m_assets.erase(asset_id);
            if (!erase_success)
                SCX_CORE_ERROR("Failed to remove asset entry from asset registry.");
        }
    }

    /**
     * \brief Gets an asset in the registry from its ID.
     * \param asset_id The ID of the asset in the registry.
     * \return The corresponding asset.
     */
    [[nodiscard]] static T GetAssetById(size_t asset_id)
    {
        if (!Get().m_assets[asset_id].loaded)
            SCX_CORE_ERROR("Faield to load asset with ID of {0}.", asset_id);

        AssetEntry& entry = Get().m_assets[asset_id];
        ++entry.reference_count;

        return entry.asset;
    }

private:
    struct AssetEntry
    {
        T asset;
        bool loaded{};
        int reference_count{};
    };

    std::unordered_map<size_t, AssetEntry> m_assets;

    AssetManager() = default;
    ~AssetManager() = default;

    static AssetManager<T> s_instance;
    static AssetManager<T>& Get() { return s_instance; }
};
