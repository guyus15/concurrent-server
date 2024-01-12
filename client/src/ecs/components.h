#pragma once

#include "rendering/sprite.h"
#include "rendering/transform.h"

#include <common/utils/uuid.h>


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
struct SpriteComponent
{
    Sprite sprite;
};
