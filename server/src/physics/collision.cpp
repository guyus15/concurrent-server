#include "collision.h"

#include <glm/ext/quaternion_geometric.hpp>

Collision::AABB::AABB()
    : vertices{}
{
}

Collision::AABB::AABB(const glm::vec2 v1, const glm::vec2 v2, const glm::vec2 v3, const glm::vec2 v4)
    : vertices{}
{
    vertices[0] = v1;
    vertices[1] = v2;
    vertices[2] = v3;
    vertices[3] = v4;
}

Collision::Box2d::Box2d()
    : vertices{}
{
}

Collision::Box2d::Box2d(const glm::vec2 v1, const glm::vec2 v2, const glm::vec2 v3, const glm::vec2 v4)
    : vertices{}
{
    vertices[0] = v1;
    vertices[1] = v2;
    vertices[2] = v3;
    vertices[3] = v4;
}

bool Collision::AABBtoAABB(const AABB b1, const AABB b2)
{
    return b1.vertices[0].x < b2.vertices[1].x &&
        b1.vertices[1].x > b2.vertices[0].x &&
        b1.vertices[0].y < b2.vertices[2].y &&
        b1.vertices[3].y > b2.vertices[0].y;
}

bool Collision::AABBtoAABB(const AABB b1, const AABB b2, bool collision_locations[4])
{
    bool has_collision = false;

    for (size_t i = 0; i < b1.vertices.size(); i++)
    {
        const glm::vec2 v = b1.vertices[i];

        if (v.x > b2.vertices[0].x &&
            v.x < b2.vertices[1].x &&
            v.y < b2.vertices[0].y &&
            v.y > b2.vertices[3].y)
        {
            if (collision_locations)
                collision_locations[i] = true;
            has_collision = true;
        }
    }

    return has_collision;
}

bool Collision::ByDistance(const glm::vec2 p1, const glm::vec2 p2, const float distance)
{
    const glm::vec2 distance_vector = p1 - p2;
    return glm::length(distance_vector) <= distance;
}
