#define CLOVE_SUITE_NAME CollisionTests
#include <clove-unit.h>

#include <physics/collision.h>

// Test 1
CLOVE_TEST(TestCollisionByDistance)
{
    /**
     * This test ensures that when two positions are given within the specified distance,
     * the ByDistance() method returns true.
     */

    const glm::vec2 p1 = { 0.0f, 0.0f };
    const glm::vec2 p2 = { 0.0f, 4.0f };

    CLOVE_IS_TRUE(Collision::ByDistance(p1, p2, 5.0f));
}

// Test 2
CLOVE_TEST(TestNoCollisionByDistance)
{
    /**
     * This test ensures that when two positions are given beyond the specified distance,
     * the ByDistance() method returns false.
     */

    const glm::vec2 p1 = { 0.0f, 0.0f };
    const glm::vec2 p2 = { 10.0f, 0.0f };

    CLOVE_IS_FALSE(Collision::ByDistance(p1, p2, 5.0f));
}

// Test 3
CLOVE_TEST(TestAABBtoAABBCollision)
{
    /**
     * This test ensures that when two AABBs intercept one another on both axes, the
     * Collision::AABBtoAABB function returns true.
     */

    const Collision::AABB aabb1{ { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
    const Collision::AABB aabb2{ { 0.5f, 0.5f }, { 1.5f, 0.5f }, { 1.5f, 1.5f }, { 0.5f, 1.5f } };

    CLOVE_IS_TRUE(Collision::AABBtoAABB(aabb1, aabb2));
}

// Test 4
CLOVE_TEST(TestAABBtoAABBCollisionAxisX)
{
    /**
     * This test ensures that when two AABBs intercept one another only on the X axis, the
     * Collision::AABBtoAABB function returns false.
     */

    const Collision::AABB aabb1{ { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
    const Collision::AABB aabb2{ { 0.5f, 2.0f }, { 1.5f, 2.0f }, { 1.5f, 3.0f }, { 0.5f, 3.0f } };

    CLOVE_IS_FALSE(Collision::AABBtoAABB(aabb1, aabb2));
}

// Test 5
CLOVE_TEST(TestAABBtoAABBCollisionAxisY)
{
    /**
     * This test ensures that when two AABBs intercept one another only the Y axis, the
     * Collision::AABBtoAABB function returns false.
     */

    const Collision::AABB aabb1{ { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
    const Collision::AABB aabb2{ { 2.0f, 0.5f }, { 3.0f, 0.5f }, { 2.0f, 1.5f }, { 3.0f, 1.0f } };

    CLOVE_IS_FALSE(Collision::AABBtoAABB(aabb1, aabb2));
}

// Test 6
CLOVE_TEST(TestAABBtoAABBNoCollision)
{
    /**
     * This test ensures taht when two AABBs are not intercepting one another on either axis,
     * the Collision::AABBtoAABB function returns false.
     */

    const Collision::AABB aabb1{ { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
    const Collision::AABB aabb2{ { 1.5f, 1.5f }, { 2.5f, 1.5f }, { 2.5f, 2.5f }, { 1.5f, 2.5f } };

    CLOVE_IS_FALSE(Collision::AABBtoAABB(aabb1, aabb2));
}
