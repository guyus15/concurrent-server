#pragma once

#include <glm/vec2.hpp>

#include <array>

namespace Collision
{
    struct AABB
    {
        std::array<glm::vec2, 4> vertices;

        AABB();
        AABB(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, glm::vec2 v4);
    };

    struct Box2d
    {
        std::array<glm::vec2, 4> vertices;

        Box2d();
        Box2d(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, glm::vec2 v4);
    };

    bool AABBtoAABB(AABB b1, AABB b2);
    bool AABBtoAABB(AABB b1, AABB b2, bool collision_locations[4]);
    bool ByDistance(glm::vec2 p1, glm::vec2 p2, float distance);
}

glm::vec2 GetNormal(Collision::Box2d box, size_t vertex_index);
float FindMinimumSeparation(Collision::Box2d b1, Collision::Box2d b2);