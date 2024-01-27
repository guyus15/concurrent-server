#pragma once

#include "ecs/system_manager.h"

#include <entt/entity/registry.hpp>

class Entity;

class Scene
{
public:
    Scene();
    ~Scene();

    Scene(const Scene& other) = delete;
    Scene& operator=(const Scene& other) = delete;

    Scene(Scene&& other) noexcept;
    Scene& operator=(Scene&& other) noexcept;

    /**
     * \brief Updates the scene using the given delta time.
     * \param dt The delta time.
     */
    void Update(double dt);

    /**
     * \brief Creates an entity in the scene with the given name and returns a copy.
     * \param name The name of the entity.
     * \return A copy of the entity.
     */
    Entity CreateEntity(const std::string& name = "Game Object");

    /**
     * \brief Destroys the given entity from the scene.
     * \param entity The entity to destroy.
     */
    void DeleteEntity(Entity& entity);

private:
    entt::registry m_registry;
    SystemManager m_system_manager;

    friend class Entity;
    friend class RenderingSystem;
};
