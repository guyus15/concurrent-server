#pragma once

#include "scene.h"

#include <common/utils/assertion.h>

#include <entt/entity/registry.hpp>


/**
 * \brief An Entt entity wrapper class.
 */
class Entity
{
public:
    Entity(const entt::entity handle, Scene* scene)
        : m_handle{ handle },
          m_scene{ scene },
          m_destroyed{ false }
    {
    }

    /**
     * \brief Adds a component to the entity.
     * \tparam T The type of component to add.
     * \tparam Args The types of parameters to be passed to the \code T\endcode constructor.
     * \param args The parameters to be passed to the \code T\endcode constructor.
     * \return A reference top the entity component.
     */
    template <typename T, typename... Args>
    T& AddComponent(Args... args)
    {
        SCX_ASSERT(!m_destroyed, "Trying to get component from a destroyed entity.");
        return m_scene->m_registry.emplace<T>(m_handle, std::forward<Args>(args)...);
    }

    /**
     * \brief Gets a component of the given type from the entity.
     * \tparam T The type of component to get.
     * \return A reference to the entity component.
     */
    template <typename T>
    T& GetComponent()
    {
        SCX_ASSERT(!m_destroyed, "Trying to get a component from a destroyed entity.");
        return m_scene->m_registry.get<T>(m_handle);
    }

    /**
     * \brief Removes a component from of the given type from the entity.
     * \tparam T THe type of component to remove.
     */
    template <typename T>
    void RemoveComponent() const
    {
        SCX_ASSERT(!m_destroyed, "Trying to remove component from a destroyed entity.");
        m_scene->m_registry.erase<T>(m_handle);
    }

    /**
     * \brief Determines whether this entity has a component of type \code T\endcode.
     * \tparam T The type of component to check for.
     * \return A true or false value indicating whether the entity has the component \code T\endcode.
     */
    template <typename T>
    [[nodiscard]] bool HasComponent() const
    {
        SCX_ASSERT(!m_destroyed, "Trying to check for a component from a destroyed entity.");
        return m_scene->m_registry.all_of<T>(m_handle);
    }

    /**
     * \brief Gets the Entt handle of the entity.
     * \return The entity handle.
     */
    entt::entity& GetHandle()
    {
        SCX_ASSERT(!m_destroyed, "Trying to get the handle of a destroyed entity.");
        return m_handle;
    }

    /**
     * \brief Destroys an entity by setting its destroyed flag to true.
     */
    void Destroy()
    {
        SCX_ASSERT(!m_destroyed, "Trying to destroy an entity which has already been destroyed.");
        m_destroyed = true;
    }

private:
    entt::entity m_handle;
    Scene* m_scene;
    bool m_destroyed;
};
