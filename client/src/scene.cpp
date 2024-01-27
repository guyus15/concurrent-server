#include "scene.h"

#include "ecs/entity.h"

#include "ecs/systems/rendering_system.h"

void RegisterSystems(SystemManager& manager, Scene* scene)
{
    manager.RegisterSystem<RenderingSystem>(scene);
}

Scene::Scene()
{
    RegisterSystems(m_system_manager, this);
}

Scene::~Scene()
{
    m_system_manager.ClearSystems();
}

Scene::Scene(Scene&& other) noexcept
{
    m_registry = std::move(other.m_registry);
    m_system_manager = std::move(other.m_system_manager);

    // Re-register existing system with the new Scene address.
    m_system_manager.ClearSystems();
    RegisterSystems(m_system_manager, this);
}

Scene& Scene::operator=(Scene&& other) noexcept
{
    if (this == &other)
        return *this;

    m_registry = std::move(other.m_registry);
    m_system_manager = std::move(other.m_system_manager);

    // Re-register existing system with the new Scene address.
    m_system_manager.ClearSystems();
    RegisterSystems(m_system_manager, this);

    return *this;
}

void Scene::Update(const double dt)
{
    m_system_manager.Update(dt);
}

Entity Scene::CreateEntity(const std::string& name)
{
    Entity entity{ m_registry.create(), this };

    entity.AddComponent<IdComponent>();

    return entity;
}

void Scene::DeleteEntity(Entity& entity)
{
    m_registry.destroy(entity.GetHandle());
}
