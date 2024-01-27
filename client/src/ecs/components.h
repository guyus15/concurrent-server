#pragma once

#include "rendering/sprite.h"
#include "rendering/transform.h"

#include <common/utils/uuid.h>

#include <memory>

/**
 * \brief A component used to uniquely identify an entity.
 */
struct IdComponent
{
    UUID uuid;
};

/**
 * \brief A component representing the world-space transformation of the entity.
 */
struct TransformComponent
{
    Transform transform;
};

/**
 * \brief A component used to provide an entity with a renderable sprite.
 */
struct SpriteRendererComponent
{
    std::unique_ptr<Sprite> sprite;
    glm::vec3 colour{ 1.0f, 1.0f, 1.0f };
};
