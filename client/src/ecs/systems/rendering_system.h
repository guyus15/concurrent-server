#pragma once

#include "asset_manager.h"
#include "scene.h"

#include "ecs/components.h"
#include "ecs/system.h"

class Scene;

class RenderingSystem final : public ISystem
{
public:
    explicit RenderingSystem(Scene* scene)
        : m_scene{ scene }
    {
        m_sprite_shader = AssetManager<Shader>::LoadOrRetrieve("resources/shaders/vertex.glsl",
                                                               "resources/shaders/fragment.glsl");
    }

    void Update(const double dt) override
    {
        const auto renderable_sprite_view = m_scene->m_registry.view<TransformComponent, SpriteComponent>();

        for (const auto entity : renderable_sprite_view)
        {
            auto& [transform] = m_scene->m_registry.get<TransformComponent>(entity);
            auto& [sprite] = m_scene->m_registry.get<SpriteComponent>(entity);

            sprite.Draw(transform, m_sprite_shader);
        }
    }

private:
    Scene* m_scene;
    Shader m_sprite_shader;
};
