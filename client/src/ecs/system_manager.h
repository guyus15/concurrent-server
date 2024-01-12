#pragma once

#include "ecs/system.h"

#include <memory>
#include <ranges>
#include <unordered_map>

class Scene;

/**
 * \brief Manages the systems used to act upon components.
 */
class SystemManager
{
public:
    /**
     * \brief Updates each of the registered systems using the given delta time.
     * \param dt The delta time.
     */
    void Update(const double dt)
    {
        for (const auto& value : m_registry | std::views::values)
        {
            value.get()->Update(dt);
        }
    }

    /**
     * \brief Registers a system to the system registry.
     * \tparam T The type of system to register.
     * \param scene A pointer to the scene of which the system will act upon.
     */
    template <typename T>
    void RegisterSystem(Scene* scene)
    {
        const std::shared_ptr<ISystem> type_ptr = std::make_shared<T>(scene);
        m_registry[typeid(T).hash_code()] = type_ptr;
    }

    /**
     * \brief Removes a system from the system registry.
     * \tparam T The type of system to remove.
     */
    template <typename T>
    void RemoveSystem()
    {
        m_registry.erase(typeid(T).hash_code());
    }

    /**
     * \brief Removes every system from the system registry.
     */
    void ClearSystems()
    {
        m_registry.clear();
    }

    /**
     * \brief Gets a reference to the given system in the registry.
     * \tparam T The type of systme to retrieve.
     * \return The retrieved system.
     */
    template <typename T>
    T& GetSystem()
    {
        return *(static_cast<T*>(m_registry.at(typeid(T).hash_code()).get()));
    }

private:
    std::unordered_map<size_t, std::shared_ptr<ISystem>> m_registry;
};
